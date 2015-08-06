#ifndef DATAROBOTLOCATION_H
#define DATAROBOTLOCATION_H

/***********************
 * Datatype for storing absolute position
 * of robot data including position, orientation and ID
 * from the perspective of the World.
 * **************************************/

#include <QPointF>

struct dataRobotLocation{
    QPointF worldPossition;
    int robotId;
    float worldOrientation;
};

#endif // DATAROBOTLOCATION_H
