/************************************************************************/
/*           Written by Erwin Mochtar Wijaya on 05/02/2014              */
/************************************************************************/

#include "RenderThread.h"
#include "LedDetection.h"
#include "MatToQImage.h"
#include <QPainter>
#include <QDebug>

using namespace std;
using namespace cv;

RenderThread::RenderThread(int eBugNodeSize)
{
    nodeSize = eBugNodeSize;
    grabbedFrame=Mat(HEIGHT*SCALE,WIDTH*SCALE,CV_8UC3); //initialise the image
    QPointF pos(120.0f,60.0f);

    for (int i=0;i < nodeSize;i++) {
        EBugData InitEbugData;
        InitEbugData.angle = 0;
        InitEbugData.id = i;
        InitEbugData.position = (i+1)*pos;
        InitEbugData.dataValid = false;
        eBugDataStruct.append(InitEbugData);
        eBugDetected.append(InitEbugData);
    }

    xDirection = false;
    yDirection = true;
    firstTarget = true;    
    xTarget = 100; //initial target pos x
    yTarget = 400; //initial target pos y
    dest = 0; //initial target

    x = 1;
    y = 0;

}

void RenderThread::updateTargetPosition()
{
    qDebug() << xDirection << yDirection << firstTarget << endl;
#if 0
    if(xTarget > 540) { //reached final dest, reset everything
        xDirection = false;//go right
        yDirection = true;//go up
        xTarget = 0; //initial target pos x
        yTarget = 440; //initial target pos y
        firstTarget = true;
    }

    else {
        if (xDirection) xTarget -= 80;
        else xTarget += 80;

        if(yDirection) yTarget -= 40;
        else yTarget += 40;

        qDebug() << xTarget+20 << yTarget+20 << endl;

        if(yTarget > 420) {
            yDirection = true;
            xDirection = !(xDirection);
            qDebug() << "reached top boundary" << endl;
        }
        else if(yTarget < 60) {
            yDirection = false;
            xDirection = !(xDirection);
            qDebug() << "reached bottom boundary" << endl;
        }

        else {
            yDirection = yDirection;
        }

        //if (!firstTarget) {
            xDirection = !(xDirection);
        //}

        firstTarget = false;

    }
#else
/*
    if(x<9) x++; //start counting x from 1
    else x = 1;
    xTarget = x*60;

    if(y<5) y++; //start counting y from 0
    else y = 1;

    yTarget = (y % 2)*320 + 60;
*/
    xTarget = 60 + qrand()%521;
    yTarget = 60 + qrand()%361;

  //GENERATE RANDOM TARGET
/*    int a,b;
    double adiff,bdiff,d;
do {
    a = 60 + qrand()%521; //target between 60-580
    b = 60 + qrand()%361; //target between 60-420

    adiff = (a - xTarget); //x+ to the right
    bdiff = (b - yTarget); //y+ running downwards

    d = qSqrt(qPow(adiff,2)+qPow(bdiff,2));
}
    while(d < 400);

    xTarget = a;
    yTarget = b;
*/

 //CREATE TARGET AT 4 CORNERS
/*
    if(dest < 4) {
        dest = qrand()%5;
    }
    else dest = 0; //reset back to zero

    switch(dest) {
        case 0:
        xTarget = 100; //initial target pos x
        yTarget = 400; //initial target pos y
        break;

        case 1:
        xTarget = 100; //initial target pos x
        yTarget = 40; //initial target pos y
        break;

        case 2:
        xTarget = 500; //initial target pos x
        yTarget = 40; //initial target pos y
        break;

        case 3:
        xTarget = 500; //initial target pos x
        yTarget = 400; //initial target pos y
        break;

        case 4:
        xTarget = 300; //initial target pos x
        yTarget = 220; //initial target pos y
        break;
    }
*/


/*
 //CREATE A ZIGZAG TARGET

    if(xTarget > 580) { //do reset the goal
        xTarget = 20; //initial target pos x
        yTarget = 400; //initial target pos y
    }

    if(yTarget == 440) {
            yDirection = true;
            if(!firstTarget) xTarget += 40;
        }
        else if(yTarget == 0) {
            yDirection = false;
            if(!firstTarget) xTarget += 40;
        }
        else {
            yDirection = yDirection;
        }

        if(yDirection) yTarget -= 20;
        else yTarget += 20;
        xTarget +=6;
        firstTarget = false;
*/

#endif
    //update target position
    emit newTargetLocation(xTarget+20,yTarget+20);
}

void RenderThread::run()
{
    ifstream cam("/dev/ttyACM0"); //get the camera ready

        while(1)
        {
            if(cam.good()) {
                vector<eBug> eBugs;

                int count = 0; //initialise to 0 every iteration
                cam.read((char*)& n,sizeof n);
                if(n>0xff) continue;
                cam.read((char*)blob,n*4);


                if(n==1 and (blob[0]>>23)==500) {cout<<"died\n";break;}


                grabbedFrame=Scalar(255,255,255);
                for(u_int i=0;i<n;i++)
                   {
                      float x=blob[i]<<21>>21;
                      float y=blob[i]<<11>>22;
                      u_int8_t colour=blob[i]<<9>>30;
                      float radius=blob[i]>>23;
                      points[i]={x,y,radius,colour};
                      if(radius and colour<3) circle(grabbedFrame,Point2f(WIDTH-x,y)*SCALE,radius/4*SCALE,colours[colour],-1,CV_AA);
                   }

                if(n>=5) knn_graph_partition(n,eBugs,count);
                image = MatToQImage(grabbedFrame); // convert the image format from cv::Mat to QImage


                QPainter p(&image);
                p.setRenderHint(QPainter::Antialiasing);
                QRect currentTarget = QRect(xTarget,yTarget,40,40);
                p.setPen(Qt::red);
                p.drawRect(currentTarget);

                p.setPen(Qt::black);
                for (u_int i = 0;i < nodeSize; i++) {

                    eBugDetected[i].dataValid = false; //always initialise to zero every iteration

                    /**************************Render the image****************************/
                    if ( i < eBugs.size() ) {
                        QString id = QString::number(eBugs[i].ID);                                      
                        p.drawEllipse(eBugs[i].centre,eBugs[i].radius.x(),eBugs[i].radius.y());
                        p.drawText(eBugs[i].centre,id);
                        p.drawLine(eBugs[i].centre,eBugs[i].direction);

                    /*********************************************************************/

             /*********Stores vectors of Pointers to Struct EBugData to be passed onto Control Thread**********/
#if 0
                    EBugData eBugData;
                    eBugData.id = eBugs[i].ID;
                    eBugData.position = eBugs[i].centre;
                    eBugData.angle = eBugs[i].angle;
                    eBugDataTemp.push_back(&eBugData);
#else
                    if (eBugs[i].ID < nodeSize) { //only update for valid eBug ID
                        qDebug() << "I am updating here in renderthread" << endl;
                            eBugDetected[eBugs[i].ID].dataValid = true;
                            eBugDetected[eBugs[i].ID].id = eBugs[i].ID;
                            eBugDetected[eBugs[i].ID].position = eBugs[i].centre;
                            eBugDetected[eBugs[i].ID].angle = eBugs[i].angle;
                        }
                    }// closing outer if
#endif
            /***************************************************************************************************/
                }
                p.end();

                emit newFrame(image);

#if 1
                updateData.lock();
                eBugDataStruct = eBugDetected;
                updateData.unlock();

                updateData.lock();
                emit updateTrackingData();
                updateData.unlock();
#else          
                if(eBugDataPtr.size() > 0) {
                    emit updateTrackingData();
                    qDeleteAll(eBugDataPtr);
                    eBugDataPtr.clear();
                }               
#endif
            }//end if
            msleep(10);
        }//end while

}//end void run()
