include "threadRobot.h"



ThreadRobot::ThreadRobot(QObject *parent) : QThread(parent){
    qDebug("new robot thread spawned");
}

ThreadRobot::~ThreadRobot()
{
    qDebug("closing robot thread");
}

void ThreadRobot::run(){

}
void ThreadRobot::updateEnvironmentKnowledge(QString sensorData){
    qDebug("updating robot's knowledge");
    objectEnvironmentKnowledge = sensorData;
}

void ThreadRobot::robotStateUpdated(){
    qDebug("internal state updated");
}
