
//Standard C libraries
#include <iostream>
#include <unistd.h> //For sleep()
#include <math.h> //For M_PI and sqrt

//Player library plugin
#include <libplayerc++/playerc++.h>

//OpenCv Library for mapping
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#define NO_ROBOTS 5
#define MAP_SIZE 500
#define MAP_SCALE 50

#define DRAW_LINE_THICKNESS 4
#define DRAW_POINT_THICKNESS 8


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
	if(midAverage < detectionDistance)
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
	double tooClose = 0.6;
				
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
					*speedRotate = (heading>0 ? +180 : -180 );
					return;
				}
			
			}
		}
	}
				
}

/*
* Mark the local map with collected information from rangers. -1 for empty space and +1 for a wall.
*/
void markMap(cv::Mat map, PlayerCc::RangerProxy *ranger, PlayerCc::Position2dProxy *pos)
{
	using namespace cv;
	
	//Update configuration data of ranger
	ranger->RequestConfigure();
	//Angular Res calculation seems to be wrong from ranger->GetAngularRes()
	//Use this instead (note the -1)
	double getAngularRes = (ranger->GetMaxAngle()-ranger->GetMinAngle())/(ranger->GetRangeCount()-1);
	
	//Set point at robot position
	Point ptRobot = Point( (pos->GetXPos()+5)*MAP_SCALE, (pos->GetYPos()-5)*(-1)*MAP_SCALE);
	
	//Get robot pose angle (radians)
	double aRobot = pos->GetYaw();
	
	//Mark ranger data on map
	for(int i=0; i<ranger->GetRangeCount(); i++)
	{
		//Get orientation and intensity of ranger reading
		double aRanger = ranger->GetMinAngle() + (i*getAngularRes);
		double rRanger = ranger->GetRange(i);
			
		//Convert reading to coordinates
		double xRanger = pos->GetXPos() + rRanger*cos(aRanger+aRobot);
		double yRanger = pos->GetYPos() + rRanger*sin(aRanger+aRobot);
		
		//Set point at the end of the ranger reading
		Point ptRanger = Point((xRanger+5)*MAP_SCALE, (yRanger-5)*(-1)*MAP_SCALE);
		
		//Draw line from robot to end of ranger reading (green)
		line(map, ptRobot, ptRanger, -1,DRAW_LINE_THICKNESS);
		
		if(ranger->GetRange(i)<ranger->GetMaxRange())
		{
			//Mark seen walls
			line(map, ptRanger, ptRanger, 1, DRAW_POINT_THICKNESS);
		}else{
			//Mark unexplored fronteer
			//line(map, ptRanger, ptRanger, -1, DRAW_POINT_THICKNESS);
		}
		
		
	
	}
	
	//Draw point showing robot's path
	//line(map, ptRobot, ptRobot, CV_RGB(0, 0, 255),4);
	 
}

/*
* Copy selected map into the global map database and calculate percentage of explored area
*/
double shareMaps(cv::Mat map, cv::Mat globalMap)
{
	using namespace cv;
	
	globalMap = globalMap + map;
	//Stop overflow of values beyond +/-100
	for(int row=0; row<MAP_SIZE; row++)
		for(int col=0; col<MAP_SIZE; col++)
		{
			globalMap.at<char>(row,col)=(globalMap.at<char>(row,col)>100 ? 100 : globalMap.at<unsigned char>(row,col));
			globalMap.at<unsigned char>(row,col)=(globalMap.at<char>(row,col)<-100 ? -100 : globalMap.at<char>(row,col));
		}
	
	
	//Print percentage of explored area
	double exploredPxls = countNonZero(globalMap);
	double explored = 100*exploredPxls/(MAP_SIZE*MAP_SIZE);
	
	return explored;
	
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
	using namespace cv;

	bool showLocalMaps = false;
  try
  {
	//Create list of ebugs and their proxies
	int ports[NO_ROBOTS];
	PlayerClient* ebugs[NO_ROBOTS];
	Position2dProxy* pos[NO_ROBOTS];
	RangerProxy* ranger[NO_ROBOTS];
	Mat maps[NO_ROBOTS];
	char windowName[NO_ROBOTS][10];	
	
	//Create global map
	char const * globalWindow = "Global Map";
	Mat globalMap = Mat::zeros(MAP_SIZE, MAP_SIZE, CV_8SC1);
	namedWindow( globalWindow, WINDOW_AUTOSIZE );
	
	
	//Initialise each ebug, proxy and map
	for(int i=0; i<NO_ROBOTS; i++)
	{
		//Assign to ports 6665, 6666, etc
		ports[i] = 6665+i;
		ebugs[i] = new PlayerClient("localhost", ports[i]);
		pos[i] = new Position2dProxy(ebugs[i], 0);
		ranger[i] = new RangerProxy(ebugs[i], 0);
		maps[i] = Mat::zeros(MAP_SIZE, MAP_SIZE, CV_8SC1);//Signed chars, Scalar(0,0,0));
		sprintf( windowName[i], "MAP %d", i);		
		
		std::cout << "Created robot #" << i << std::endl;
		std::cout << "\tPort:\t" << ports[i] << std::endl;
		std::cout << "\tClient:\t\t" << ebugs[i] << std::endl;
		std::cout << "\tPos2dProxy:\t" << pos[i] << std::endl;
		std::cout << "\tRanger:\t\t" << ranger[i] << std::endl<< std::endl;
		
		//Show map
		if(showLocalMaps)
			namedWindow( windowName[i], WINDOW_AUTOSIZE );
		
		
		//Enable motor function
		pos[i]->SetMotorEnable(1);
		
	}
	
	//To hold percentage explored
	double explored;
	std::cout << "Explored (%)\tElapsed Simulation Time (s)" << std::endl;

	//Control
	while(true)
	{
		for(int i=0; i<NO_ROBOTS; i++)
		{
	
			//Update sensor data
			ebugs[i]->Read();
			
			//Variables for robot motion in m/s, degrees/s
			double speedForward, speedRotate;
			
			//Mark map with ranger data
			markMap(maps[i], ranger[i], pos[i]);
			
			//Communicate maps
			explored = shareMaps(maps[i], globalMap);			
			
			//Apply RandomWalk or Straigh Forward effect on speed variables
			//RandomWalk(&speedForward, &speedRotate);
			StraightForward(&speedForward, &speedRotate);

			//Make adjustments to avoid obsticles
			avoidWall(&speedForward, &speedRotate, ranger[i]);
			avoidCollision(i, &speedForward, &speedRotate, pos);
			
						
			//Apply speeds to motors
			pos[i]->SetSpeed(speedForward, dtor(speedRotate));
			//pos[i]->SetSpeed(0, dtor(0));
			
					
			//Display current map
			if(showLocalMaps)
				imshow(windowName[i],127*maps[i]);
			//waitKey(1);
			
			//Output exploration data and time
			std::cout << explored << "\t" << pos[i]->GetDataTime() << std::endl;			
		}
		
		imshow(globalWindow,127*globalMap);
		waitKey(1);
		
		//Wait one second
		sleep(0.01);
		
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



