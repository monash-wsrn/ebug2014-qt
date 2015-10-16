//Number of robots in simulation
#define NO_ROBOTS 12

//Size of stored map memory
#define MAP_SIZE 500
//MAP_SCALE = MAP_SIZE (pixels) / Simulation size (metres)
#define MAP_SCALE 50

//Share maps to neighbours within this radius (metres)
#define MAP_SHARING_DIST 1
//Only consider frontiers if within radius (metres)
#define CONSIDER_FRONT_DIST 3

//Charges of virtual point forces
#define Q_WALL (-100)
#define Q_ROBOT (-100)
#define Q_FRONTIER (100)

//Dissipation factors of virtual forces
#define D_WALL (2)
#define D_ROBOT (2)
#define D_FRONTIER (2)

//Movement bias factors
#define K_FWD 0.1
#define B_FWD 0.15
#define K_ROT 0.5*M_PI
#define B_ROT 0

//Drawing thickness parameters
#define DRAW_LINE_THICKNESS 6
#define DRAW_POINT_THICKNESS 4



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
			double rForce = (rRanger>0.01) ? Q_WALL / pow(rRanger,D_WALL) : Q_WALL*10000;
			
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
			double rForce = (dist>0.001) ? Q_ROBOT / pow(dist,D_ROBOT) : Q_ROBOT*10000;
			
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
* Only consider points within distance of CONSIDER_FRONT_DIST
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
				if(dist<CONSIDER_FRONT_DIST)
				{
					//Calculate relative orientation to neighbour
					double angle=atan2(yPos-yThis,xPos-xThis)-aThis;
			
			
					//Calculate virtual force
					double rForce = (dist>0.001) ? Q_FRONTIER/pow(dist,D_FRONTIER) : Q_FRONTIER*10000;
			
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
			if((-1)!=map.at<signed char>(ptRanger)) //check if unexplored point
			{
				//Draw line from robot to end of ranger reading
				line(map, ptRobot, ptRanger, -1,DRAW_LINE_THICKNESS);
				line(map, ptRanger, ptRanger, -2, 1); //Mark fronteir with 1pxl 
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
* Share map of robot i with neighbours within MAP_SHARING_DIST distance
*/
void shareMaps(int i, cv::Mat *maps, PlayerCc::Position2dProxy **pos)
{
	using namespace cv;
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
			
			if(dist<MAP_SHARING_DIST)
			{
				for(int row=0; row<MAP_SIZE; row++)
				{
					for(int col=0; col<MAP_SIZE; col++)
					{
						schar pixel=maps[i].at<schar>(row,col);
						if(pixel!=0)
							maps[k].at<schar>(row,col)=pixel;
					}
				}
			}
		}
	}
	
}

/*
* Copy selected map into the global map database
* Keep original global data if new data is unexplored or
* if new data is frontier and old data is not unexplored
*/
void copyMapIntoGlobal(cv::Mat map, cv::Mat globalMap)
{
	using namespace cv;
	
	
	for(int row=0; row<MAP_SIZE; row++)
	{
		for(int col=0; col<MAP_SIZE; col++)
		{
			schar pixel=map.at<schar>(row,col);
			if(pixel==1 || pixel==-1 || (pixel==-2 && globalMap.at<schar>(row,col)==0))
				globalMap.at<schar>(row,col)=pixel;
		}
	}
	
}

/*
* Return the percentage of the global map that has been explored
*/
double getExplored(cv::Mat globalMap)
{
	using namespace cv;

	//Calculate percentage of explored area
	double exploredPxls = countNonZero(globalMap);
	double explored = 100*exploredPxls/(MAP_SIZE*MAP_SIZE);
	
	return explored;
	
}

/*
* Copy the global map directly into this local map
*/
void copyGlobalIntoMap(cv::Mat map, cv::Mat globalMap)
{
	using namespace cv;
	globalMap.copyTo(map);
}

