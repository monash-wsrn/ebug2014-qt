// $Revision: 422 $
// $Date: 2013-08-11 20:09:38 +1000 (Sun, 11 Aug 2013) $
// $Author: nickdademo@gmail.com $

#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

// Qt
#include <QtCore/qmath.h>
#include <QtCore/QThread>
#include <QDebug>
#include <QMutex>
// Local
#include <Buffer.h>
#include "Structures.h"
#include "EBugData.h"
#include "Config.h"
#include <control-algorithms/Demo.h>
// eBug API
#include <ebug-api-cpp/eBugAPI.h>

class ControlThread : public QThread
{
    Q_OBJECT

    public:

        ControlThread(Buffer<QByteArray> *txPacketBuffer,QList<XBeeNode> XBeeNodeList);
        void stop();
        void setNodeList(QList<XBeeNode> nodeList);
        void initializeEbugDataVector();
        void setEbugDataVector(QList<EBugData> eBugData);
        QList<int> sortEBugData(int nodeSize, double xtarget, double ytarget, QList<EBugData> eBugData); //perform sorting algorithm based on eBug distances to target

    public slots:
        void updateTarget(int x, int y);//update new target generated by RenderThread

    private:
        Demo *demo;
        QMutex doStopMutex;
        Buffer<QByteArray> *txPacketBuffer;
        volatile bool doStop;
        QList<XBeeNode> xBeeNodeList;
        QList<EBugData> ebugData;
        QList<bool> eBugDetected;
        XBeeNodeData convertToNodeData(XBeeNode node);
        int stopFlag;
        double distance,xdiff,ydiff,x1,x2,y1,y2;
        QList<double> CollisionAngle,targetAngle,minDistance;
        QList<int> CollisionNodes;
        int xTarget, yTarget; //main target from RenderThread
        QList <int> TargetX;
        QList <int> TargetY;
        QList <double> AngleTargetX;
        QList <double> AngleTargetY;
        QList<int> SortedID;       
        QList<int> collDist;
        QList<int> undetectedCounter;
        QList<bool> eBugOutsideArena;
        EBugData minEbug;
        double minDist;
        int controlThreadSleep;
        QList<double> forceX, forceY;
    protected:
        void run();

    signals:
        void controlThreadStopped();
        void targetReached();
};

#endif // CONTROLTHREAD_H