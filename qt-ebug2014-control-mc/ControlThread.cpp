// $Revision: 422 $
// $Date: 2013-08-11 20:09:38 +1000 (Sun, 11 Aug 2013) $
// $Author: nickdademo@gmail.com $

#include "ControlThread.h"

ControlThread::ControlThread(Buffer<QByteArray> *txPacketBuffer,QList<XBeeNode> XBeeNodeList): QThread(), txPacketBuffer(txPacketBuffer)
{
  doStop=false;
  distance = 2000;
  xdiff = 500;
  ydiff = 500;
  x1 = 0;
  y1 = 0;
  x2 = 640;
  y2 = 480;
  this->xBeeNodeList = XBeeNodeList;
  
  for (int a = 0; a < this->xBeeNodeList.size(); a++) {
    // Initialize members
    CollisionAngle.append(0.0);
    targetAngle.append(0.0);
    minDistance.append(0.0);
    CollisionNodes.append(0);
    eBugDetected.append(false);
    TargetX.append(0);
    TargetY.append(0);
    AngleTargetX.append(0.0);
    AngleTargetY.append(0.0);
    collDist.append(0);
    undetectedCounter.append(0);
    eBugOutsideArena.append(false);
  } // for

  controlThreadSleep = 120/xBeeNodeList.size();

  xTarget = 120; //initial target -> bottom left
  yTarget = 420; //initial target -> bottom left
  
  int windowSize = 5;
  
  for (int b = 0; b < windowSize; b++) {
    forceX.append(0.0);
    forceY.append(0.0);
  }
} // ControlThread()

void ControlThread::run()
{

  demo = new Demo(this->xBeeNodeList, this->txPacketBuffer);
  demo->Reset();
  // wait 1 second to ensure camera detects new LED sequences assigned to eBug
  sleep(1); 

  SortedID =
    sortEBugData(this->xBeeNodeList.size(),xTarget,yTarget,this->ebugData);
  
  QString filename =
    "/home/ebugcontrol/Desktop/Forcewith4ebugunder20percNoise.txt";
  QFile file(filename);
  
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    while (1) {
        // Stop thread if doStop=TRUE 
      doStopMutex.lock();
      if(doStop) {
	doStop=false;
	doStopMutex.unlock();
	break;
      } // if
      doStopMutex.unlock();

      /***** TARGET ENTRAPMENT WITH MORSE POTENTIAL ******/
#if 1 // From Erwin. 
      for (int i = 0; i < xBeeNodeList.size(); i++) {
	double vec_x = 0.0;
	double vec_y = 0.0;
	double Fx = 0.0;
	double Fy = 0.0;
	double Fx_norm = 0.0;
	double Fy_norm = 0.0;
	double avg_angle,Fmag,Fmag_norm;
	double noise_percentage = ((qrand()%2)*2 - 1)*0; //noise level 0%
	                                                 // (FOR DEMONSTRATION)

	vec_y += sin(ebugData[i].angle*PI/180.0);
	vec_x += cos(ebugData[i].angle*PI/180.0);
	
	// Calculate the Morse Potential resulting from Neighbouring eBugs
	for (int n = 0; n < xBeeNodeList.size(); n++) {
	  if(n == i)
	    continue;
	  x1 = ebugData[i].position.x();
	  y1 = ebugData[i].position.y();
	  
	  if (true) {
	    x2 = ebugData[n].position.x() +
	      noise_percentage*ebugData[n].position.x();
	    y2 = ebugData[n].position.y() +
	      noise_percentage*ebugData[n].position.y();
	  } else {
	    x2 = ebugData[n].position.x();
	    y2 = ebugData[n].position.y();
	  } // else
	  double di0 = fabs(x1-x2);
	  double di1 = fabs((x1-1.0)-x2);
	  double di2 = fabs((x1+1.0)-x2);
	  
	  double dj0 = fabs(y1-y2);
	  double dj1 = fabs((y1-1.0)-y2);
	  double dj2 = fabs((y1+1.0)-y2);
	  
	  double dist = qSqrt(qPow(di0,2) + qPow(dj0,2));
	  
	  double dij1 = qSqrt(qPow(di1,2)+qPow(dj0,2));
	  double dij2 = qSqrt(qPow(di2,2)+qPow(dj0,2));
	  double dji1 = qSqrt(qPow(di0,2)+qPow(dj1,2));
	  double dji2 = qSqrt(qPow(di0,2)+qPow(dj2,2));
	  
	  double Ux1 = (-Ca*exp(-dij1/la))+(Cr*exp(-dij1/lr));
	  double Ux2 = (-Ca*exp(-dij2/la))+(Cr*exp(-dij2/lr));
	  
	  double Uy1 = (-Ca*exp(-dji1/la))+(Cr*exp(-dji1/lr));
	  double Uy2 = (-Ca*exp(-dji2/la))+(Cr*exp(-dji2/lr));
	  
	  double Uxgrad = (Ux1-Ux2)/2; //+x to the right
	  double Uygrad = (Uy2-Uy1)/2; //+y pointing downwards
	  
	  Fx += Uxgrad;
	  Fy += Uygrad;
	  
	  Fx_norm += (Uxgrad*dist/40);
	  Fy_norm += (Uygrad*dist/40);
	  
	  vec_y += sin(ebugData[n].angle*PI/180.0);
	  vec_x += cos(ebugData[n].angle*PI/180.0);
	  
	} // for
	Fmag = qSqrt(qPow(Fx,2) + qPow(Fy,2));
	Fmag_norm = qSqrt(qPow(Fx_norm,2) + qPow(Fy_norm,2));
	
	stream << Fmag << "\t" <<Fmag_norm<<"\t";
	stream.flush();
	
	avg_angle = atan2(vec_y,vec_x)*180.0/PI;
	// passing the ID and variable
	demo->updateEBugData(this->ebugData[i].id,this->ebugData[i]);
	demo->Start(i,0,0,0,0,0,Fx_norm,Fy_norm,avg_angle);
	/*****TARGET FOLLOWING ROBOTS******/
#else
	// Removed Erwin's #if 1 else block
#endif
	
	/***** RANDOM WALK WITH COLLISION AVOIDANCE ****/
#if 0
	// Removed Erwin's if 0 code block
#endif
	msleep(controlThreadSleep);
      } // end for loop
      stream << "\n";
    } // end while loop
  } //end of file open
  file.close();

  demo->StopAll();   //added by Erwin 22/02/14
  delete demo;    //added by Erwin 22/02/14
  qDebug() << "\n" << "Stopping control thread..." << "\n";
  emit controlThreadStopped();    //added by Erwin 22/02/14
} // run()

void ControlThread::setEbugDataVector(QList<EBugData> eBugData)
{
    ebugData = eBugData;
} // setEbugDataVector()

QList<int> ControlThread::sortEBugData(int nodeSize,double xtarget, double ytarget, QList<EBugData> eBugData)
{
  /* Determine eBug ID sequences based on distance from the first target */
  QList<EBugData> eBugTemp;
  QList<double> eBugDist;
  int closestID; //storing the closest ID position in QList
  QList<int> SortedEbugID; //output
  
  //initialise eBugTemp to contain all eBugData
  for (int a = 0; a < nodeSize; a++) {
    eBugTemp.append(eBugData.at(a)); //insert distance at position according
                                     // to eBug ID

    x1 = eBugData.at(a).position.x();
    y1 = eBugData.at(a).position.y();
    x2 = xtarget;
    y2 = ytarget;
    
    xdiff = (x1 - x2); //x+ to the right
    ydiff = (y2 - y1); //y+ running downwards
    
    eBugDist.append(qSqrt(qPow(xdiff,2)+qPow(ydiff,2)));
  } // for 

  // Perform Sorting Algorithm
  for (int i = 0; i < nodeSize; i++) {
    minDist = eBugDist.at(0); // initialise min distance to the first element
    closestID = 0;
    
    for (int j = 0; j < eBugTemp.size(); j++) {
      if(j == 0) continue; // Skip comparing with itself
      if (minDist > eBugDist.at(j)) {
	closestID = j;
	minEbug = eBugTemp.at(j);
	minDist = eBugDist.at(j); //update the min distance value
      } // if
    } // for
    // Store eBug ID closest to target
    SortedEbugID.append(eBugTemp.at(closestID).id); 
    eBugTemp.removeAt(closestID);
    eBugDist.removeAt(closestID);
    
    // Last j iteration updated here as max j = 0 (ebugTemp.size = 1) 
    if (eBugTemp.size() == 1) {
      closestID = 0;
      minDist = eBugDist.at(0); //update the min distance value
    } // if
  } // for
  return SortedEbugID;
} // sortEBugData()

void ControlThread::updateTarget(int x, int y)
{
  xTarget = x;
  yTarget = y;
  SortedID = sortEBugData(this->xBeeNodeList.size(),x,y,this->ebugData);
} // updateTarget()

void ControlThread::setNodeList(QList<XBeeNode> nodeList)
{
  qDebug() << "node list size: "<< nodeList.size();
  this->xBeeNodeList = nodeList;
} // setNodeList()

XBeeNodeData ControlThread::convertToNodeData(XBeeNode node)
{
  XBeeNodeData result;
  result.commandStatus = node.commandStatus;
  result.deviceType = node.deviceType;
  result.manufacturerID = node.manufacturerID;
  result.MY = node.MY;
  result.NI = node.NI.toUtf8().constData();
  result.parentNetworkAddress = node.parentNetworkAddress;
  result.profileID = node.profileID;
  result.SH_high = node.SH_high;
  result.SH_low = node.SH_low;
  result.SL_high = node.SL_high;
  result.SL_low = node.SL_low;
  result.status = node.status;
  return result;
} // convertToNodeData()

void ControlThread::initializeEbugDataVector()
{
  //ebugData = new QList<EBugData*>();
  int node_n = this->xBeeNodeList.size();
  QPointF pos(120.0f, 60.0f);
  for (int i = 0; i < node_n; i++) {
    EBugData ebugdata;
    ebugdata.id = i;
    ebugdata.angle = 0.0f;
    ebugdata.position = (i+1)*pos;
    ebugData.append(ebugdata);
  } // for
} // initializeEbugDataVector()

void ControlThread::stop()
{
  doStopMutex.lock();
  doStop=true;
  doStopMutex.unlock();
} // stop()


// Storage area below.
// 
// This is Erwin's if 0 block, stored here.
#if 0
 for(int i = 0;i<xBeeNodeList.size();i++){

                CollisionAngle[i] = 0;
                CollisionNodes[i] = 0;
                targetAngle[i] = 0;
                minDistance[i] = 130.0;

                double vec_x = 0.0;
                double vec_y = 0.0;



               /***************Compare with all nodes whereby Collision is possible************/
               for(int n = 0;n<xBeeNodeList.size();n++) {

                    if(n == i)  continue;

                     x1 = ebugData[i].position.x();
                     y1 = ebugData[i].position.y();
                     x2 = ebugData[n].position.x();
                     y2 = ebugData[n].position.y();

                     xdiff = (x1 - x2); //x+ to the right
                     ydiff = (y2 - y1); //y+ running downwards

                     distance = qSqrt(qPow(xdiff,2)+qPow(ydiff,2));
                     targetAngle[i] = (atan2(ydiff,xdiff)*180.0/PI);
                     if(targetAngle[i] < 0.0) targetAngle[i] += 360.0;
                     //distance = qSqrt(qPow(xdiff,2)+qPow(ydiff,2));


                     if(distance < minDistance[i]) { //check whether collision has occured
                         qDebug() << "possible collision between ebug id " << i << " and " << n << endl;
                         demo->collisionDetected(i);
                         CollisionNodes[i]++;
                         minDistance[i] += 20.0;
                         vec_x += cos(targetAngle[i]*PI/180.0);
                         vec_y += sin(targetAngle[i]*PI/180.0);
                         demo->resetEbugTimer(i);//start timer
                        }
                    }
                CollisionAngle[i] = atan2(vec_y,vec_x)*180.0/PI;
                if(CollisionAngle[i] < 0.0) CollisionAngle[i] += 360.0;
                /*******************************************************************************/

               /**************************************Control of Ebug based on its State*********************************/
               if(CollisionNodes[i] < 1) {
                   //qDebug()<<"EBUG ID = "<< i << " detects no collision\n";
                   demo->ebugTrapped(i,false);
                   demo->noCollision(i);
               }

               else if(CollisionNodes[i] < 4) {
                    demo->ebugTrapped(i,false);
                    demo->setCollisionAngle(i,CollisionAngle[i]);
                   }

               else demo->ebugTrapped(i,true);

                /******************************************************************************************************/
                demo->updateEBugData(this->ebugData[i].id,this->ebugData[i]);//passing the ID and variable
                demo->Start(i,xTarget,yTarget);
 
#endif

		// Erwin's if 1 else block moved here. 		
#if 1
		// Do nothing
#else
		/********************Test Control of eBug movement from any point to centre of arena (320,240)************************/
        for(int j = 0;j<xBeeNodeList.size();j++){

            int i = SortedID.at(j);
            CollisionNodes[i] = 0;

            // Check for collision with leader ID excluding local leader
            for (int n = 0;n <= (j-2);n++) { //main and second leader skip collision check
                int k = SortedID.at(n); //leader of LL
                int l = SortedID.at(j-1); //LL (local leader)
                double a1 = this->ebugData[k].position.x();
                double b1 = this->ebugData[k].position.y();
                double a2 = this->ebugData[i].position.x();
                double b2 = this->ebugData[i].position.y();
                double a3 = this->ebugData[l].position.x();
                double b3 = this->ebugData[l].position.y();

                double adiff = (a1 - a2); //x+ to the right
                double bdiff = (b2 - b1); //y+ running downwards

                double adiff2 = (a3 - a2);
                double bdiff2 = (b2 - b3);

                double d1 = qSqrt(qPow(adiff,2)+qPow(bdiff,2));
                double d2 = qSqrt(qPow(adiff2,2)+qPow(bdiff2,2));

                if(d1 < d2 && d1 < 100) // leader of LL closer than LL --> possible collision
                    CollisionNodes[i]++;
            }

            if(CollisionNodes[i]==0) demo->noCollision(i);
            else demo->collisionDetected(i);

            double Kp_lin = 0.45*Ku_lin;//with error_thres = 0(PD Controller)
            double Ki_lin = 1*Kp_lin/Pu;
            double Kd_lin = Kp_lin*Pu/3;//reduce overshoot
            double lin_errorThreshold = 100; //50 pixels error to activate integral component
            double lin_integral_error = 10;
            u_int16_t eBugRadius = 50; //set distance from other eBug

            //update target location
            if(i == SortedID.at(0)) { //eBug closest to destination
                collDist[i] = 0;
                TargetX[i] = xTarget;
                TargetY[i] = yTarget;
                AngleTargetX[i] = xTarget;
                AngleTargetY[i] = yTarget;
                demo->speedController[i].updateParameters((0.33*Ku_lin),0,(0.33*Ku_lin*Pu/3),0,800,0.120);//Leader eBug controlled without I component
            }
            else if(i == SortedID.at(1)) {
                collDist[i] = 100;
                int k = SortedID.at(j-1);
                TargetX[i] = this->ebugData[k].position.x();
                TargetY[i] = this->ebugData[k].position.y();
                AngleTargetX[i] = this->ebugData[k].position.x() + (eBugRadius*cos(this->ebugData[k].angle*PI/180));
                AngleTargetY[i] = this->ebugData[k].position.y() - (eBugRadius*sin(this->ebugData[k].angle*PI/180));
                demo->speedController[i].updateParameters(Kp_lin,Ki_lin,Kd_lin,lin_errorThreshold,lin_integral_error,0.120);
            }
            else {
                collDist[i] = 100;
                int k = SortedID.at(j-1);
                TargetX[i] = this->ebugData[k].position.x();
                TargetY[i] = this->ebugData[k].position.y();
                AngleTargetX[i] = this->ebugData[k].position.x() - (0.5*eBugRadius*cos(this->ebugData[k].angle*PI/180));
                AngleTargetY[i] = this->ebugData[k].position.y() + (0.5*eBugRadius*sin(this->ebugData[k].angle*PI/180));
                demo->speedController[i].updateParameters(Kp_lin,Ki_lin,Kd_lin,lin_errorThreshold,lin_integral_error,0.120);//Leader eBug controlled without I component
            }
            /****************************************************************************************************/

            demo->updateEBugData(this->ebugData[i].id,this->ebugData[i]);//passing the ID and variable
            demo->Start(i,TargetX[i],TargetY[i],AngleTargetX[i],AngleTargetY[i],collDist[i]);

            if(demo->reachedTarget(i) && i == SortedID.at(0) ) {
                emit targetReached();
            }
#endif

		
