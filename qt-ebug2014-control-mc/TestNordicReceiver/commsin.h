#ifndef COMMSIN_H
#define COMMSIN_H

#include <QObject>
#include <QtSerialPort>

#include "ThreadableQObject.h"
#include "messageTranslator.h"
#include "dataRobotLocation.h"

class CommsIn : public ThreadableQObject
{
    Q_OBJECT

private:
    QSerialPort* port;
    MessageTranslator *messageTranslator;
    QMutex mutexBuffer;
    QList<char> buffer;
    bool fillingBuffer;
    qint32 numLeds;

public:
    explicit CommsIn(QString strPortName);
    ~CommsIn();
    QStringList getPortInfo();

private slots:
    void read();
    void dataReadyToSend(QList<dataRobotLocation> listRobotLocations);


signals:
    void newRobotData(QList<dataRobotLocation> listRobotLocations);
public slots:
    void run();

};

#endif // COMMSIN_H
