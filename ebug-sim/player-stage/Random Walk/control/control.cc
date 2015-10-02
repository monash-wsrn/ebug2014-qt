
//Standard C libraries
#include <iostream>
#include <unistd.h> //For sleep()


#include <libplayerc++/playerc++.h>

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
	
	//Establish Player Client for each robot and its proxies
	PlayerClient ebug1("localhost", 6665);
	Position2dProxy ebug1_pos(&ebug1, 0);
	RangerProxy ebug1_ranger(&ebug1, 0);
	
	PlayerClient ebug2("localhost", 6666);
	Position2dProxy ebug2_pos(&ebug2, 0);
	RangerProxy ebug2_ranger(&ebug2, 0);
	
	//Enable motor function
	ebug1_pos.SetMotorEnable(1);
	
	
	//Control
	while(true)
	{
		
		//Update sensor data
		ebug1.Read();
		
		//Variables for robot motion in m/s, degrees/s
		double speedForward, speedRotate;
				
		//Apply RandomWalk effect on speed variables
		StraightForward(&speedForward, &speedRotate);
		
		//Apply speeds to motors
		ebug1_pos.SetSpeed(speedForward, dtor(speedRotate));
		
		
		//Print some output
		for(int i=0; i<ebug1_ranger.GetRangeCount(); i++)
			std::cout << i << ":" << ebug1_ranger.GetIntensity(i) << "\n" ;
		std::cout << "\n\n\n";
		
		
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
	double forwardMax = 0.5; //Maximum forward speed
	int rotateMax = 90; //Maximum turning speed in + and - directions
	
	//Variables to hold chosen speeds, later copied into args
	double forwardSelect, rotateSelect;
	
	//Select forward speed between 0 and forwardMax with 1/100 granuality
	forwardSelect = ((rand()%101)/100)*(double)forwardMax;
		
	//Select rotational speed between -rotateMax and +rotateMax
	rotateSelect = rotateMax-(rand()%(2*rotateMax));
	
	std::cout << "Speed set to: " << forwardSelect << "\tRotation set to: " << rotateSelect << std::endl;

	//Assign selected speeds to arguments
	*speedForward = forwardSelect;
	*speedRotate = rotateSelect;

}


void StraightForward(double *speedForward, double *speedRotate){
	double speed = 0.5;
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
