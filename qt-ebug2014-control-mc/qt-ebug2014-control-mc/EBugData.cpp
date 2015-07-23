// $Revision: 422 $
// $Date: 2013-08-11 20:09:38 +1000 (Sun, 11 Aug 2013) $
// $Author: nickdademo@gmail.com $
#if 0
#include <EBugData.h>
/*
 * EBugData.cpp
 *
 *  Created on: May 22, 2013
 *      Author: sherry
 */

EBugData::EBugData(int eBug_id, QPointF eBug_Global, float eBug_orientation)
{
    id = eBug_id;
    angle = eBug_orientation; //between 0-360 degree
    position = eBug_Global;
    leadersID[0] = -1;
    leadersID[1] = -1;
    formationRole = -1;
}

EBugData::~EBugData()
{

}


int EBugData::getID(){
    return id;
}

QPointF EBugData::getPosition(){
    return position;
}

float EBugData::getAngle(){
    return angle;
}

void EBugData::setPosition(QPointF point){
    this->position = point;
}

void EBugData::setAngle(float newangle)
{
    this->angle = newangle;
}

void EBugData::setID(int ID)
{
    this->id = ID;
}
#endif
