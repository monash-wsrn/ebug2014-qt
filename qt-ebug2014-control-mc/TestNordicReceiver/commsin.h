#ifndef COMMSIN_H
#define COMMSIN_H

#include <QObject>
#include <QtSerialPort>

#include "ThreadableQObject.h"
#include "messageTranslator.h"
#include "dataRobotLocation.h"

class CommsIn : public QThread
{
    Q_OBJECT

private:
    QString strPortName;
    QMutex mutexPortName;
    int timeoutWait=10;

    MessageTranslator *messageTranslator;
    QMutex mutexBuffer;
    QList<char> buffer;
    bool fillingBuffer;
    bool quit;
    qint32 numLeds;

public:
    CommsIn(QObject *parent = 0);
    ~CommsIn();
    QStringList getPortInfo();

private slots:
    void read();
    void dataReadyToSend(QList<dataRobotLocation> listRobotLocations);


signals:
    void newRobotData(QList<dataRobotLocation> listRobotLocations);
    void newRawData(QByteArray bytarRead);
public slots:
    void run();

};

#endif // COMMSIN_H
