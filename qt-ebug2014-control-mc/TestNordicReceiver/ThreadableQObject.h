/*****************************
 * Defines a Threadable QObject that extends QObject and
 * adds signals start() and quit() and virtual slot run().
 * Constructor will create QThread and move object to QThread.
 *
 * Use this type to define a QObject you wish to thread by
 * extending and overwriting run() function.
 * ****************************************************/



#ifndef THREADABLEQOBJECT_H
#define THREADABLEQOBJECT_H

#include <QObject>
#include <QThread>

class ThreadableQObject : public QObject
{
    Q_OBJECT
public:
    ThreadableQObject() : QObject(){
        threadMoveConnect();
    }

    /**
     * @brief threadMoveConnect
     * Create a thread, move this object to the thread connecting
     * start, stop, quit. Start thread.
     */
private:    void threadMoveConnect(){
        QThread* thread = new QThread();
        this->moveToThread(thread);

        //Transfer object signals to thread
        connect(this, SIGNAL(start()), thread, SLOT(start()));
        connect(this, SIGNAL(quit()), thread, SLOT(quit()));
        //Connect object's run slot with thread
        connect(thread, SIGNAL(started()), this, SLOT(run()));

        //Start object
        this->start();
    }
signals:
    void start();
    void quit();
public slots:
    virtual void run(){}

};

#endif // THREADABLEQOBJECT_H
