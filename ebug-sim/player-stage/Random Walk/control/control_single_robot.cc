
//Standard C libraries
#include <iostream>
#include <unistd.h> //For sleep()
#include <math.h> //For M_PI


#include <libplayerc++/playerc++.h>

void RandomWalk(double *speedForward, double *speedRotate);
void StraightForward(double *speedForward, double *speedRotate);

/*
* Measure the rangers around the centre and 1/4 and 3/4 indexes and move away from detected
* walls.
*/
void avoidWall(double *speedForward, double *speedRotate, PlayerCc::RangerProxy *ranger)
{

	double speed = 0.3;
	double rotation = 0;
	
	int availableRangers = ranger->GetRangeCount();
	std::cout << "Rangers available: " << availableRangers << std::endl;
	
	//Index of rangers to check for walls
	int midRanger = availableRangers/2;
	int leftRanger = 3*availableRangers/4;
	int rightRanger = availableRangers/4;
	
	
	//Average range readings from these directions
	double midAverage = (ranger->GetRange(midRanger)+ranger->GetRange(midRanger+1)+ranger->GetRange(midRanger-1))/3;
	double leftAverage = (ranger->GetRange(leftRanger)+ranger->GetRange(leftRanger-1)+ranger->GetRange(leftRanger-2))/3;
	double rightAverage = (ranger->GetRange(rightRanger)+ranger->GetRange(rightRanger+1)+ranger->GetRange(rightRanger+2))/3;
	
	
	//Print readings
	std::cout << "Average range reading from front: " << midAverage << std::endl;
	std::cout << "Average range reading from left: " << leftAverage << std::endl;
	std::cout << "Average range reading from right: " << rightAverage << std::endl;
	
	
	//Distance to wall that triggers a reaction
	double detectionDistance = 0.5;

	//Angle speed to turn away from an obsticle in degrees/sec
	int avoidRotateSpeed = 50;

	//Compare readings to threshold
	if(leftAverage < detectionDistance)
	{	//Obsticle at front left
		std::cout << "Wall at left" << std::endl;
		speed = 0.1;
		rotation = (-1)*avoidRotateSpeed;
	}
	if(rightAverage < detectionDistance)
	{	//Obsticle at front right
		std::cout << "Wall at right" << std::endl;
		speed = 0.1;
		rotation = (1)*avoidRotateSpeed;
	}
	if(midAverage < detectionDistance)
	{	//Obsticle directly in front
		std::cout << "Wall at front" << std::endl;
		speed = 0; //Stop
		if(rotation==0)
			rotation=(-1)*avoidRotateSpeed; //Spin right if not spinning
	}
	
	//Assign selected speeds to arguments
	*speedForward = speed;
	*speedRotate = rotation;

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
	//Establish Player Client for each robot and its proxies
	PlayerClient ebug1("localhost", 6665);
	Position2dProxy ebug1_pos(&ebug1, 0);
	RangerProxy ebug1_ranger(&ebug1, 0);
	
	//Enable motor function
	ebug1_pos.SetMotorEnable(1);

	//Control
	while(true)
	{
		
		//Update sensor data
		ebug1.Read();
		
		int availableRangers = ebug1_ranger.GetRangeCount();
	std::cout << "OUT:: Rangers available: " << availableRangers << std::endl;
				
		//Variables for robot motion in m/s, degrees/s
		double speedForward, speedRotate;
				
		//Apply RandomWalk effect on speed variables
		//RandomWalk(&speedForward, &speedRotate);
		StraightForward(&speedForward, &speedRotate);

		//Make adjustments to avoid obsticles
		avoidWall(&speedForward, &speedRotate, &ebug1_ranger);
			
		//Apply speeds to motors
		ebug1_pos.SetSpeed(speedForward, dtor(speedRotate));
		
		
		//Print motion values
		//std::cout << "Forward speed: " << speedForward << "\tRotation speed: " << speedRotate << std::endl;

		//Wait one second
		sleep(1);
		
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


void StraightForward(double *speedForward, double *speedRotate){
	double speed = 0.3;
	double rotation = 0;
	
	//Assign selected speeds to arguments
	*speedForward = speed;
	*speedRotate = rotation;
}

