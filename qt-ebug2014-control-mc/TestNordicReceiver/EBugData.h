/***************************
 * Simple struct container to
 * hold robot data from world
 * perspective. Includes:
 * position, id, angle and a
 * dataValid flag.
 * *************************/

#ifndef EBUGDATA_H
#define EBUGDATA_H
#include <QtCore>

struct EBugData {
    QPointF position;
    int id;
    float angle;
    bool dataValid;
};


//#include <QtCore>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
// //#include <map>
//#include <iostream>
//#include <Structures.h>

//using namespace cv;
//using namespace std;

//class EBugData{

//public:
//        EBugData(int eBug_id, QPointF eBug_Global, float eBug_angle);
//        // angle in degree [0,360)
//        ~EBugData();

//        vector<relativePos> neighboursPos;
//        int leadersID[2];
//        int formationRole; // -1 for formation leader
//        void setPosition(QPointF point);
//        void setAngle(float newangle);
//        void setID(int ID);
//        QPointF getPosition();
//        float getAngle();
//        int getID();

//private:
//        QPointF position;
//        int id;
//        float angle;
//        vector<EBugData*> neighbours;

//};
#endif // EBUGDATA_H
