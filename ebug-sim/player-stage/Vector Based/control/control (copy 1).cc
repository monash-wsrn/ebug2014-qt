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
	
	//Print column titles for std output
	std::cout << "Explored (%)\tElapsed Simulation Time (s)\tRobot Number" << std::endl;
	
	waitKey(0); //Pause here until keyboard press (gives user time to arrange windows manually)	
	
	int pressedKey=-1;	
	//Control
	while(-1==pressedKey)
	{
		for(int i=0; i<NO_ROBOTS; i++)
		{
			//Update sensor data
			ebugs[i]->Read();
			
			//Variables for robot goal vector
			double x=0;
			double y=0;
			
			//Mark map with ranger data
			markMap(maps[i], ranger[i], pos[i]);
			
			//Communicate maps
			copyMapIntoGlobal(maps[i], globalMap);
			//copyGlobalIntoMap(maps[i], globalMap);
						
			//Make adjustments to avoid obsticles
			avoidWall(&x, &y, ranger[i]);
			//avoidCollision(i, &x, &y, pos);
			
			//Seek frontier points from map to create virtual attraction force
			seekFrontier(&x, &y, maps[i], pos[i]);
			
			//Normalise drive vector
			double magnitude=sqrt(pow(x,2)+pow(y,2));
			x=(magnitude>0.001 ? x/magnitude : 0);
			y=(magnitude>0.001 ? y/magnitude : 0);	
			
			//Apply speeds to motors
			pos[i]->SetSpeed(x*K_FWD+B_FWD, y*K_ROT+B_ROT);
			
			
			//Correct robot position if moved through wall
			if(pos[i]->GetXPos()>5 || pos[i]->GetXPos()<-5 || pos[i]->GetYPos()>5 || pos[i]->GetYPos()<-5)
				pos[i]->GoTo(0,0, pos[i]->GetYaw()); //Aim for centre of map
				
				
			//Display current map
			if(showLocalMaps)
				imshow(windowName[i],maps[i]+10);
			
			//Output exploration data and time
			double explored = getExplored(globalMap);
			std::cout << explored << "\t" << pos[i]->GetDataTime() << "\t" << i << std::endl;			
		}
		
		imshow(globalWindow,40*globalMap);
		pressedKey=waitKey(1);
		
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



