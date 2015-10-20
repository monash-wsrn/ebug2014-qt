#include "common.h"

/*
* Main control to initialise all robots and related proxies
* then loop through control code to read sensors and react.
* This algorithm uses virtual forces from robots, walls and local frontiers
* to influence movement.
* There is no map sharing between robots but all robots copy map into local map.
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
//	Mat maps[NO_ROBOTS];
	char windowName[NO_ROBOTS][10];
		
	//Create global map
	char const * globalWindow = "Global Map";
	Mat globalMap = Mat::zeros(MAP_SIZE, MAP_SIZE, CV_8SC1);
//	namedWindow( globalWindow, WINDOW_AUTOSIZE );

	//Keep track of timestep size and explored map
	double time=0;
	double explored=0;
	
	//Initialise each ebug, proxy and map
	for(int i=0; i<NO_ROBOTS; i++)
	{
		//Assign to ports 6665, 6666, etc
		ports[i] = 6665+i;
		ebugs[i] = new PlayerClient("localhost", ports[i]);
		pos[i] = new Position2dProxy(ebugs[i], 0);
		ranger[i] = new RangerProxy(ebugs[i], 0);
//		maps[i] = Mat::zeros(MAP_SIZE, MAP_SIZE, CV_8SC1);//Signed chars, Scalar(0,0,0));
		sprintf( windowName[i], "MAP %d", i);		
		
		std::cout << "Created robot #" << i << std::endl;
		std::cout << "  Memory locations:" << std::endl;
		std::cout << "\tPort:\t" << ports[i] << std::endl;
		std::cout << "\tClient:\t\t" << ebugs[i] << std::endl;
		std::cout << "\tPos2dProxy:\t" << pos[i] << std::endl;
		std::cout << "\tRanger:\t\t" << ranger[i] << std::endl;
//		std::cout << "\tMap:\t\t" << &maps[i] << std::endl<< std::endl;
		
		//Show map
//		if(showLocalMaps)
//			namedWindow( windowName[i], WINDOW_AUTOSIZE );
		
		
		//Enable motor function
		pos[i]->SetMotorEnable(1);
		
	}
	
	std::cout << "Press any key to begin, and q to stop." << std::endl;
	char pressedKey=waitKey(0); //Pause here until keyboard press (gives user time to arrange windows manually)	
	
	
	//Print column titles for std output
	std::cout << "Explored (%)\tSimulation Timestep (s)" << std::endl;
	
	//Control
	while(explored<100)
	{
		for(int i=0; i<NO_ROBOTS; i++)
		{
			//Update sensor data
			ebugs[i]->Read();
			
			
			
			/*** MAP MARKING AND SWAPPING ***/
			
			//Mark map with ranger data
			markMap(globalMap, ranger[i], pos[i]);
			
			//Communicate maps
//			copyMapIntoGlobal(maps[i], globalMap);
//			shareMaps(i, maps, pos);
			
			
			
			/*** CALCULATE VIRTUAL FORCES ***/
			
			//Calculate wall force vector
			double xWall=0;
			double yWall=0;
			avoidWall(&xWall, &yWall, ranger[i]);
			
			
			//Calculate robot collision force vector
			double xRobots=0;
			double yRobots=0;
			avoidRobots(i, &xRobots, &yRobots, pos);
			
			
			//Seek frontier points from map to create virtual attraction force
			double xFront=0;
			double yFront=0;
			seekFrontier(&xFront, &yFront, globalMap, pos[i]);
			
			
			
			
			
			/*** ROBOT MOTION ***/
			
			//Set up drive vector as force totals
			double x=xWall+xRobots+xFront;
			double y=yWall+yRobots+yFront;
			
			//Normalise drive vector
			double magnitude=sqrt(pow(x,2)+pow(y,2));
			x=(magnitude>NEARLY_ZERO*NEARLY_ZERO ? x/magnitude : 0);
			y=(magnitude>NEARLY_ZERO*NEARLY_ZERO ? y/magnitude : 0);	
			
			//Apply speeds to motors
			pos[i]->SetSpeed(x*K_FWD+B_FWD, y*K_ROT);
			
			
			//Correct robot position if moved through wall
			if(pos[i]->GetXPos()>(SIM_SIZE/2) || pos[i]->GetXPos()<(-1)*(SIM_SIZE/2) || pos[i]->GetYPos()>(SIM_SIZE/2) || pos[i]->GetYPos()<(-1)*(SIM_SIZE/2))
				pos[i]->GoTo(0,0, pos[i]->GetYaw()); //Aim for centre of map
			
			
			
			/*** VISUAL OUTPUTS ***/
				
			//Display current map
		//	if(showLocalMaps)
		//		imshow(windowName[i],SHOW_MAP(maps[i]));
			
			

				
		}

		//Output exploration data and time
		explored = getExplored(globalMap);
		std::cout<<explored<<"\t"<<pos[0]->GetDataTime()-time<< std::endl;	
		time=pos[0]->GetDataTime(); //Update timestep data

		//Show global map
		//imshow(globalWindow,SHOW_MAP(globalMap));
		
		//Gives time for windows to update
	//	pressedKey=waitKey(1);
		//sleep(0.001);
		
	}
	destroyAllWindows(); //Close opencv windows

  }
  catch (PlayerCc::PlayerError & e)
  {
    // Output the error
    std::cerr << "PLAYER ERROR: " << e << std::endl;
    return -1;
  }
		
	return 0;
}



