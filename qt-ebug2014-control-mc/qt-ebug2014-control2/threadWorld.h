#ifndef THREADWORLD_H
#define THREADWORLD_H

#include <QObject>
#include <QImage>

#include "ThreadableQObject.h"


class ThreadWorld : ThreadableQObject
{
    Q_OBJECT
public:
    explicit ThreadWorld();

private:
    //QList<ThreadCommsIn> listCommsIn;
    //QList<ThreadCommsOut> listCommsOut;
    //QList<ThreadRobots> listRobots;

signals:
    void uiWorldRenderUpdate(QImage img);
    void uiRobotListUpdate(QString listRobots);
    void uiCommsInListUpdate(QString listCommsIn);
    void uiCommsOutListUpdate(QString listCommsIn);
    void uiCommsInMsgNew(QString strMsg);
    void uiCommsOutMsgNew(QString strMsg);
    void uiWorldEventNew(QString strWorldEventInfo);

public slots:
    void run();
    void uiCommsInAddConnection();
    void uiCommsInDeleteConnection(int idCommsIn);
    void uiCommsOutAddConnection();
    void uiCommsOutDeleteConnection(int idCommsIn);


};

#endif // THREADWORLD_H
