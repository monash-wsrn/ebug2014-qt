include "threadCommunication.h"



ThreadCommunication::ThreadCommunication(QObject *parent) : QThread(parent){
    qDebug("new comms thread spawned");
}

ThreadCommunication::~ThreadCommunication()
{
    qDebug("closing comms thread");
}

void ThreadCommunication::run(){

}

