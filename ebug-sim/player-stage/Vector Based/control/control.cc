
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
#define MAP_SIZE 100
//MAP_SCALE = MAP_SIZE (pixels) / Simulation size (metres)
#define MAP_SCALE 10


#define Q_WALL (-1)
#define Q_ROBOT (-1)
#define Q_FRONTIER (1)

#define K_FWD 0.1
#define B_FWD 0.15
#define K_ROT 0.5*M_PI
#define B_ROT 0

#define DRAW_LINE_THICKNESS 2
#define DRAW_POINT_THICKNESS 2


/*
* Apply virtual force from each ranger sensor returning a value lower than the maximum.
*/
void avoidWall(double *x, double *y, PlayerCc::RangerProxy *ranger)
{
	int availableRangers = ranger->GetRangeCount();
	//std::cout << "Rangers available: " << availableRangers << std::endl;
	
	//Sometimes the ranger elements have not yet loaded which will cause segmentation fault if accessed.
	if(availableRangers==0)
		return;
	
	
	//Update configuration data of ranger
	ranger->RequestConfigure();
	
	//Angular Res calculation seems to be wrong from ranger->GetAngularRes() (Player/Stage BUG)
	//Use this instead (note the -1)
	double getAngularRes = (ranger->GetMaxAngle()-ranger->GetMinAngle())/(ranger->GetRangeCount()-1);
	
	//For each ranger
	for(int i=0; i<ranger->GetRangeCount(); i++)
	{
		//Read ranger as polar coordinates
		double aRanger = ranger->GetMinAngle() + (i*getAngularRes);
		double rRanger = ranger->GetRange(i);
		
		if(rRanger<ranger->GetMaxRange() )
		{
			//Wall has been seen
			
			//Calculate virtual force
			double rForce = (rRanger>0.1) ? Q_WALL / pow(rRanger,2) : Q_WALL*100;
			
			//Convert to cartesian
			double xForce = rForce*cos(aRanger);
			double yForce = rForce*sin(aRanger);
			
			//Apply forces
			(*x) += xForce;
			(*y) += yForce;
			
		}
	}
	
}

/*
* Make adjustments to heading to avoid collision with another robot
*/
void avoidCollision(int i, double *x, double *y, PlayerCc::Position2dProxy** pos)
{
	//Get x,y global position and orientation of robot in focus (i)
	double xThis = pos[i]->GetXPos();
	double yThis = pos[i]->GetYPos();
	double aThis = pos[i]->GetYaw();
	
	//For each other robot calculate the virtual force
	for(int k=0; k<NO_ROBOTS; k++)
	{
		if(k!=i) //Exculde this robot
		{
			//Global position of neighbour
			double xPos = pos[k]->GetXPos();
			double yPos = pos[k]->GetYPos();
			
			//Calculate Euler distance to neighbour
			double dist=sqrt(pow(xThis-xPos,2)+pow(yThis-yPos,2));
			//Calculate relative orientation to neighbour
			double angle=atan2(yPos-yThis,xPos-xThis)-aThis;
			
			
			//Calculate virtual force
			double rForce = (dist>0.1) ? Q_ROBOT / pow(dist,2) : Q_ROBOT*100;
			
			//Convert to cartesian
			double xForce = rForce*cos(angle);
			double yForce = rForce*sin(angle);
			
			//Apply forces
			(*x) += xForce;
			(*y) += yForce;
		}
	}
				
}

/*
* Create virtual attraction forces to frontier points marked on map with -2
*/
void seekFrontier(double *x, double *y, cv::Mat map,  PlayerCc::Position2dProxy* pos)
{
	//Get x,y global position and orientation of robot in focus
	double xThis = pos->GetXPos();
	double yThis = pos->GetYPos();
	double aThis = pos->GetYaw();
	
	//Check each cell for frontier marking and apply force
	for(int row=0; row<MAP_SIZE; row++)
		for(int col=0; col<MAP_SIZE; col++)
			if((int)map.at<schar>(row,col)==-2)
			{
				//Frontier mark on map
				//Scale to a global coordinate
				double xPos = (double)row/MAP_SCALE - 5;
				double yPos = (double)col/(-1*MAP_SCALE) +5;
				
				//Calculate Euler distance to frontier
				double dist=sqrt(pow(xThis-xPos,2)+pow(yThis-yPos,2));
				//Calculate relative orientation to neighbour
				double angle=atan2(yPos-yThis,xPos-xThis)-aThis;
			
			
				//Calculate virtual force
				double rForce = (dist>0.1) ? Q_FRONTIER / pow(dist,2) : Q_FRONTIER*100;
			
				//Convert to cartesian
				double xForce = rForce*cos(angle);
				double yForce = rForce*sin(angle);
			
				//Apply forces
				(*x) += xForce;
				(*y) += yForce;
			}
	
}

/*
* Mark the local map with collected information from rangers. -1 for empty space and +1 for a wall
* and -2 for a frontier.
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
		
		if(ranger->GetRange(i)<ranger->GetMaxRange())
		{
			//Draw line from robot to end of ranger reading
			line(map, ptRobot, ptRanger, -1,DRAW_LINE_THICKNESS);
			//Mark seen walls
			line(map, ptRanger, ptRanger, 1, DRAW_POINT_THICKNESS);
		}else{
			//Mark unexplored frontier (make sure to to overwrite -2)
			if((-1)!=map.at<char>(ptRanger)) //check if unexplored point
			{
				//Draw line from robot to end of ranger reading
				line(map, ptRobot, ptRanger, -1,DRAW_LINE_THICKNESS);
				map.at<char>(ptRanger)=-2;
			}else{
				//Draw line from robot to end of ranger reading
				line(map, ptRobot, ptRanger, -1,DRAW_LINE_THICKNESS);
			}
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
	
	
	for(int row=0; row<MAP_SIZE; row++)
	{
		for(int col=0; col<MAP_SIZE; col++)
		{
			schar pixel=map.at<schar>(row,col);
			std::cout<<"Skip "<<pixel<<" into ("<<row<<","<<col<<")"<<std::endl;
			if(pixel!=0)
			{
				std::cout<<"Copying "<<pixel<<" into ("<<row<<","<<col<<")"<<std::endl;
				globalMap.at<schar>(row,col)=pixel;
			}
		}
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

	//Set aside space for each local map
	Mat maps[NO_ROBOTS];
	char windowName[NO_ROBOTS][10];
		
	//Create global map
	char const * globalWindow = "Global Map";
	Mat globalMap = Mat::zeros(MAP_SIZE, MAP_SIZE, CV_8SC1);
	namedWindow( globalWindow, WINDOW_NORMAL );
	
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
			namedWindow( windowName[i], WINDOW_NORMAL );
		
		
		//Enable motor function
		pos[i]->SetMotorEnable(1);
		
	}
	
	//To hold percentage explored
	double explored;
	std::cout << "Explored (%)\tElapsed Simulation Time (s)" << std::endl;


		waitKey(1);
		std::cout<<"DONE"<<std::endl;
		//Wait one second
		sleep(1);
		
	//Control
	while(true)
	{
		for(int i=0; i<NO_ROBOTS; i++)
		{
			//Update sensor data
			ebugs[i]->Read();
			
			//Variables for robot goal vector
			double x=0;
			double y=0;
			std::cout<<"MARKING MAP"<<std::endl;
			//Mark map with ranger data
			markMap(maps[i], ranger[i], pos[i]);
			std::cout<<"DONE"<<std::endl;
			std::cout<<"SHARING"<<std::endl;
			//Communicate maps
			//explored = shareMaps(i, maps[i], globalMap, pos);
			std::cout<<"DONE"<<std::endl;
			
			//Make adjustments to avoid obsticles
			avoidWall(&x, &y, ranger[i]);
			avoidCollision(i, &x, &y, pos);
			
			//Seek frontier points from map to create virtual attraction force
			seekFrontier(&x, &y, maps[i], pos[i]);
			
			//Normalise drive vector
			double magnitude=sqrt(pow(x,2)+pow(y,2));
			x=(magnitude>0.00001 ? x/magnitude : 0);
			y=(magnitude>0.00001 ? y/magnitude : 0);	
			
			//std::cout << "FORCE:" << x*K_FWD+B_FWD << "\t" << y*K_ROT*180/M_PI << std::endl;
				
			//Apply speeds to motors
			pos[i]->SetSpeed(x*K_FWD+B_FWD, y*K_ROT+B_ROT);
			
			
			//Correct robot position if moved through wall
			if(pos[i]->GetXPos()>5 || pos[i]->GetXPos()<-5 || pos[i]->GetYPos()>5 || pos[i]->GetYPos()<-5)
				pos[i]->GoTo(0,0, pos[i]->GetYaw()); //Aim for centre of map
				
				
			//Display current map
			if(showLocalMaps)
			{
				std::cout<<"SHOWING LOCAL MAP"<<std::endl;
				imshow(windowName[i],maps[i]+10);
				std::cout<<"DONE"<<std::endl;
			}
			//waitKey(5);
	
			//Output exploration data and time
			//std::cout << explored << "\t" << pos[i]->GetDataTime() << std::endl;			
		}
		
		std::cout<<"SHOWING GLOBAL MAP"<<std::endl;
		imshow(globalWindow,globalMap+10);
		waitKey(20);
		std::cout<<"DONE"<<std::endl;
		//Wait one second
		//sleep(0.1);
		
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



