
//Standard C libraries
#include <iostream>
#include <unistd.h> //For sleep()
#include <math.h> //For M_PI and sqrt


#include <libplayerc++/playerc++.h>


#define NO_ROBOTS 3


/*
* RandomWalk: randomly assigns forward and rotation speeds
* between 0 and internal maximums.
*/
void RandomWalk(double *speedForward, double *speedRotate)
{
	double forwardMax = 0.3; //Maximum forward speed
	int rotateMax = 60; //Maximum turning speed in + and - directions
	
	//Variables to hold chosen speeds, later copied into args
	double forwardSelect, rotateSelect;
	
	//Select forward speed between 0 and forwardMax with 1/100 granuality
	forwardSelect = ((float)(rand()%101)/100)*forwardMax;
		
	//Select rotational speed between -rotateMax and +rotateMax
	rotateSelect = rotateMax-(rand()%(2*rotateMax));
	
	//std::cout << "Speed set to: " << forwardSelect << "\tRotation set to: " << rotateSelect << std::endl;

	//Assign selected speeds to arguments
	*speedForward = forwardSelect;
	*speedRotate = rotateSelect;

}

/*
* Set movement variables to make robot drive straight forward.
*/
void StraightForward(double *speedForward, double *speedRotate){
	double speed = 0.3;
	double rotation = 0;
	
	//Assign selected speeds to arguments
	*speedForward = speed;
	*speedRotate = rotation;
}


/*
* Measure the rangers around the centre and 1/4 and 3/4 indexes and move away from detected
* walls.
*/
void avoidWall(double *speedForward, double *speedRotate, PlayerCc::RangerProxy *ranger)
{
	double speed = 0.3;
	double rotation = 0;
	
	int availableRangers = ranger->GetRangeCount();
	//std::cout << "Rangers available: " << availableRangers << std::endl;
	
	//Sometimes the ranger elements have not yet loaded which will cause segmentation fault if accessed.
	if(availableRangers==0)
		return;

	//Index of rangers to check for walls
	int midRanger = availableRangers/2;
	int leftRanger = 3*availableRangers/4;
	int rightRanger = availableRangers/4;
	
	
	//Average range readings from these directions
	double midAverage = (ranger->GetRange(midRanger)+ranger->GetRange(midRanger+1)+ranger->GetRange(midRanger-1))/3;
	double leftAverage = (ranger->GetRange(leftRanger)+ranger->GetRange(leftRanger-1)+ranger->GetRange(leftRanger-2))/3;
	double rightAverage = (ranger->GetRange(rightRanger)+ranger->GetRange(rightRanger+1)+ranger->GetRange(rightRanger+2))/3;
	
	
	//Print readings
	//std::cout << "Average range reading from front: " << midAverage << std::endl;
	//std::cout << "Average range reading from left: " << leftAverage << std::endl;
	//std::cout << "Average range reading from right: " << rightAverage << std::endl;
	
	
	//Distance to wall that triggers a reaction
	double detectionDistance = 0.5;

	//Angle speed to turn away from an obsticle in degrees/sec
	int avoidRotateSpeed = 50;

	//Compare readings to threshold
	if(leftAverage < detectionDistance)
	{	//Obsticle at front left
		//std::cout << "Wall at left" << std::endl;
		speed = 0.1;
		rotation = (-1)*avoidRotateSpeed;
	}
	if(rightAverage < detectionDistance)
	{	//Obsticle at front right
		//std::cout << "Wall at right" << std::endl;
		speed = 0.1;
		rotation = (1)*avoidRotateSpeed;
	}
	if(midAverage < detectionDistance || ranger->GetRange(midRanger) < detectionDistance )
	{	//Obsticle directly in front
		//std::cout << "Wall at front" << std::endl;
		speed = 0; //Stop
		if(rotation==0)
			rotation=(-1)*avoidRotateSpeed; //Spin right if not spinning
	}
	
	//Assign selected speeds to arguments
	*speedForward = speed;
	*speedRotate = rotation;
}

/*
* Make adjustments to heading to avoid collision with another robot
*/
void avoidCollision(int i, double *speedForward, double *speedRotate, PlayerCc::Position2dProxy** pos)
{
	double tooClose = 0.3;
				
	//Check distance and heading to each neighbour and take action if collision incoming
	for(int k=0; k<NO_ROBOTS; k++)
	{
		if(k!=i)
		{
			//Calculate Euler distance to neighbours
			double dist=sqrt(pow(pos[i]->GetXPos()-pos[k]->GetXPos(),2)+pow(pos[i]->GetYPos()-pos[k]->GetYPos(),2));
			//std::cout << "Distance from "<<i<<" to " << k << ":\t"<<dist<<std::endl;
			if(dist<tooClose)
			{
				//Slow down
				*speedForward = 0.1;
				
				//Angle to close neighbour
				double angle=atan2(pos[k]->GetYPos()-pos[i]->GetYPos(),pos[k]->GetXPos()-pos[i]->GetXPos())*180/M_PI;
				//Direction this robot is heading
				double yaw = pos[i]->GetYaw()*180/M_PI;
				//Relative angle to close neighbour
				double heading = yaw-angle;
				
				//std::cout << "angle:\t"<< angle <<std::endl;
				//std::cout << "yaw:\t"<< yaw <<std::endl;
				//std::cout << "heading:\t"<< heading <<std::endl;
				
				if(abs(heading)<90)
				{
					//std::cout << "COLLISION INCOMING!" << std::endl;
					//Driving towards collision!
					*speedForward = 0;
					*speedRotate = (heading>0 ? +90 : -90 );
					return;
				}
			
			}
		}
	}
				
}


/*
* Main control to initialise all robots and related proxies
* then loop through control code to read sensors and react.
* This algorithm lets robots with one ranger and diff wheels move about randomly
* and back up when a wall is detected.
*/
int main(int argc, char *argv[])
{
	using namespace PlayerCc;
  try
  {
	//Initialise list of ebugs and their proxies
	int ports[NO_ROBOTS];
	PlayerClient* ebugs[NO_ROBOTS];
	Position2dProxy* pos[NO_ROBOTS];
	RangerProxy* ranger[NO_ROBOTS];
	
	for(int i=0; i<NO_ROBOTS; i++)
	{
		//Assign to ports 6665, 6666, etc
		ports[i] = 6665+i;
		ebugs[i] = new PlayerClient("localhost", ports[i]);
		pos[i] = new Position2dProxy(ebugs[i], 0);
		ranger[i] = new RangerProxy(ebugs[i], 0);
		
		std::cout << "Created robot #" << i << std::endl;
		std::cout << "\tPort:\t" << ports[i] << std::endl;
		std::cout << "\tClient:\t\t" << ebugs[i] << std::endl;
		std::cout << "\tPos2dProxy:\t" << pos[i] << std::endl;
		std::cout << "\tRanger:\t\t" << ranger[i] << std::endl<< std::endl;
		
		
		//Enable motor function
		pos[i]->SetMotorEnable(1);
		
	}

	//Control
	while(true)
	{
		for(int i=0; i<NO_ROBOTS; i++)
		{
	
			//Update sensor data
			ebugs[i]->Read();
			
			//Variables for robot motion in m/s, degrees/s
			double speedForward, speedRotate;
			
			//Apply RandomWalk or Straigh Forward effect on speed variables
			//RandomWalk(&speedForward, &speedRotate);
			StraightForward(&speedForward, &speedRotate);

			//Make adjustments to avoid obsticles
			avoidWall(&speedForward, &speedRotate, ranger[i]);
			avoidCollision(i, &speedForward, &speedRotate, pos);
			
			//Apply speeds to motors
			pos[i]->SetSpeed(speedForward, dtor(speedRotate));
		}
		//Wait one second
		//sleep(1);
		
	}

  }
  catch (PlayerCc::PlayerError & e)
  {
    // Output the error
    std::cerr << "PLAYER ERROR: " << e << std::endl;
    return -1;
  }
		
	return 0;
}



