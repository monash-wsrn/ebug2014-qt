/*****************************
 * Defines a CommsIn QObject that can be used as an incomming
 * connection. It requires the commsInMsgNew(dataRobot) signal
 * to be defined.
 *
 * Use this template to form CommsIn objects such as camera
 * input drivers.
 * ****************************************************/



#ifndef COMMSIN_H
#define COMMSIN_H

#include <QObject>
#include "ThreadableQObject.h"
#include "dataRobotLocation.h"

 class CommsIn : public ThreadableQObject
 {
     Q_OBJECT
 signals:
     void commsInMsgNew(dataRobotLocation dataRobotMsg);

 };



#endif // COMMSIN_H
