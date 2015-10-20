#include "common.h"

/*
* Main program for printing data with NO movements for testing functions
*/
int main(int argc, char *argv[])
{
	using namespace PlayerCc;
	using namespace cv;

	bool showLocalMaps = true;
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
		std::cout << "  Memory locations:" << std::endl;
		std::cout << "\tPort:\t" << ports[i] << std::endl;
		std::cout << "\tClient:\t\t" << ebugs[i] << std::endl;
		std::cout << "\tPos2dProxy:\t" << pos[i] << std::endl;
		std::cout << "\tRanger:\t\t" << ranger[i] << std::endl;
		std::cout << "\tMap:\t\t" << &maps[i] << std::endl<< std::endl;
		
		//Show map
		if(showLocalMaps)
			namedWindow( windowName[i], WINDOW_AUTOSIZE );
		
		
		//Enable motor function
		pos[i]->SetMotorEnable(1);
		
	}
	
	std::cout << "Press any key to begin, and q to stop." << std::endl;
	char pressedKey=waitKey(0); //Pause here until keyboard press (gives user time to arrange windows manually)	
	
	
	//Print column titles for std output
	std::cout << "Explored (%)\tElapsed Simulation Time (s)\tRobot Number" << std::endl;
	
	//Control
	while(pressedKey!='q')
	{
		for(int i=0; i<NO_ROBOTS; i++)
		{
			//Update sensor data
			ebugs[i]->Read();
			
				
			//Mark map with ranger data
			markMap(maps[i], ranger[i], pos[i]);
			
			//Communicate maps
			copyMapIntoGlobal(maps[i], globalMap);
			//copyGlobalIntoMap(maps[i], globalMap);
						
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
			seekFrontier(&xFront, &yFront, maps[i], pos[i]);
			

			//Normalise drive vector
		//	double magnitude=sqrt(pow(x,2)+pow(y,2));
		//	x=(magnitude>0.001 ? x/magnitude : 0);
		//	y=(magnitude>0.001 ? y/magnitude : 0);	
			
			//Apply speeds to motors
			//pos[i]->SetSpeed(x*K_FWD+B_FWD, y*K_ROT+B_ROT);
			
			
			//Correct robot position if moved through wall
			//if(pos[i]->GetXPos()>5 || pos[i]->GetXPos()<-5 || pos[i]->GetYPos()>5 || pos[i]->GetYPos()<-5)
			//	pos[i]->GoTo(0,0, pos[i]->GetYaw()); //Aim for centre of map
				
				
			//Display current map
			if(showLocalMaps)
				imshow(windowName[i],SHOW_MAP(maps[i]));
			
			//Output exploration data and time
			double explored = getExplored(globalMap);
			//std::cout << explored << "\t" << pos[i]->GetDataTime() << "\t" << i << std::endl;		
			
			
			//Get some stats
			double mWall = sqrt(pow(xWall,2)+pow(yWall,2));
			double mRobots = sqrt(pow(xRobots,2)+pow(yRobots,2));
			double mFront = sqrt(pow(xFront,2)+pow(yFront,2)) ;
			
			double mTotal = mWall+mRobots+mFront;
			
			//Print some stats
			std::cout<< "Robot Index\t" << i << std::endl;
			std::cout<<"Frontier:\t"<<xFront<<"\t"<<yFront<<"\t\t"<<sqrt(pow(xFront,2)+pow(yFront,2))<<std::endl;
			std::cout<<"Wall:\t\t" << 100*mWall/mTotal << std::endl;
			std::cout<<"Robots:\t\t" << 100*mRobots/mTotal << std::endl;
			std::cout<<"Frontier:\t" << 100*mFront/mTotal << std::endl<<std::endl<<std::endl;
			
			
//			std::cout<<"Wall:\t\t"<<xWall<<"\t"<<yWall<<"\t\t"<<sqrt(pow(xWall,2)+pow(yWall,2))<<std::endl;
//			std::cout<<"Robots:\t\t"<<xRobots<<"\t"<<yRobots<<"\t\t"<<sqrt(pow(xRobots,2)+pow(yRobots,2))<<std::endl;
			
//						
//			std::cout<< "Explored %\t"<<explored<<std::endl;
//			std::cout<< "Time (s)\t"<< pos[i]->GetDataTime()<<std::endl;
//			std::cout<< "Robot Index\t" << i << std::endl;	
		}
		
		imshow(globalWindow,SHOW_MAP(globalMap));
		pressedKey=waitKey(0);
		
		std::cout<<"Pressed Key:\t"<<pressedKey<<std::endl;
		
		
		//Gives time for windows to update
		sleep(0.001);
		
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



