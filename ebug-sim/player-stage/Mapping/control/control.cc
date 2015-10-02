
//Standard C libraries
#include <stdio.h>
#include <unistd.h> //For sleep()

//Player Library
#include <libplayerc++/playerc++.h>

//OpenCV Library
#include <opencv/cv.h>

#define NO_ROBOTS 2

void RandomWalk(double *speedForward, double *speedRotate);
void StraightForward(double *speedForward, double *speedRotate);
void AvoidWal(double *speedForward, double *speedRotate, PlayerCc::RangerProxy &ranger);


/*
* Main control to initialise all robots and related proxies
* then loop through control code to read sensors and react.
* This algorithm lets robots with one ranger and diff wheels move about randomly
* and back up when a wall is detected.
*/
int main(int argc, char *argv[])
{
	using namespace PlayerCc;
	
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
				
			//Apply RandomWalk effect on speed variables
			StraightForward(&speedForward, &speedRotate);
		
			//Apply speeds to motors
			pos[i]->SetSpeed(speedForward, dtor(speedRotate));
		
		
			//Print some output
			//for(int i=0; i<ebugs[0].ranger.GetRangeCount(); i++)
			//	std::cout << i << ":" << ebugs[0].ranger.GetIntensity(i) << "\n" ;
			std::cout << "\n\n\n";
		
		   }
		   
		   
		//Wait one second
		sleep(1);
		
	}
		
	return 0;
}


/*
* RandomWalk: randomly assigns forward and rotation speeds
* between 0 and internal maximums.
*/
void RandomWalk(double *speedForward, double *speedRotate)
{
	int forwardMax = 1; //Maximum forward speed
	int rotateMax = 90; //Maximum turning speed in + and - directions
	
	//Variables to hold chosen speeds, later copied into args
	double forwardSelect, rotateSelect;
	
	//Select forward speed between 0 and forwardMax with 1/100 granuality
	forwardSelect = ((rand()%101)/100)*(double)forwardMax;
		
	//Select rotational speed between -rotateMax and +rotateMax
	rotateSelect = rotateMax-(rand()%(2*rotateMax));
	
	//Assign selected speeds to arguments
	*speedForward = forwardSelect;
	*speedRotate = rotateSelect;

}


void StraightForward(double *speedForward, double *speedRotate){
	double speed = 1;
	double rotation = 0;
	
	//Assign selected speeds to arguments
	*speedForward = speed;
	*speedRotate = rotation;
}
/*
* Reads from ranger proxy and sets speeds to make robot drive backward
* and turn away from a wall.
*/
void AvoidWal(double *speedForward, double *speedRotate, PlayerCc::RangerProxy &ranger){
	
	//Distance to wall that triggers a reaction
	double detectionDistance = 0.5;
	
	//Angle speed to turn away from an obsticle in degrees/sec
	if( ranger[1] > detectionDistance)
	{
		*speedForward = 0;
		*speedRotate = (-1);
	}
	//TODO
	
}
