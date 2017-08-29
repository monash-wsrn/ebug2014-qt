/************************************************************************/
/*          Written by Erwin Mochtar Wijaya on 05/02/2014              */
/**********************************************************************/
#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

// Qt
#include <QtCore/QThread>
#include <QtCore/QTime>
#include <QtGui/QImage>
// OpenCV
#include "opencv/highgui.h"
#include "EBugData.h"

#define MAX_BLOBS 256

using namespace cv;

class ImageBuffer;

class RenderThread : public QThread
{
    Q_OBJECT

    public:
    RenderThread(int eBugNodeSize);
    QList<EBugData> getEBugData() { return eBugDataStruct; } // return the copy of eBug Data

    private:        
        QImage image;
        Mat grabbedFrame;
        u_int32_t n;
        u_int32_t blob[MAX_BLOBS];
        QMutex updateData;
        //EBugData *eBugDataStructPtr; //2nd option with dynamic mem allocation
        QList<EBugData> eBugDataStruct;//data passed to ControlThread
        QList<EBugData> eBugDetected;//data updated based on camera detection
        int xTarget; // actual x point of target = xTarget+20; (Top left of square)
        int yTarget; // actual y point of target = yTarget-20; (Top left of square)
        int x,y;
        int nodeSize;
        bool xDirection; //if true increase(go right), else decrease(go left)
        bool yDirection; //if true decrease(go up), else increase(go down)
        bool firstTarget;
        int dest; //indicates 4 corners in the arena

    public slots:
    void updateTargetPosition(); //update target position

    protected:
        void run();

    signals:
        void newFrame(const QImage &frame);
        void updateTrackingData();
        void newTargetLocation(int x, int y);
};

#endif // RENDERTHREAD_H
