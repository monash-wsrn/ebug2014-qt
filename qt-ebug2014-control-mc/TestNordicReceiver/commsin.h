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
    int timeoutWait=10;
    QSerialPort port;

    MessageTranslator messageTranslator;
    QMutex mutexBuffer;
    QList<char> buffer;
    bool fillingBuffer;
    bool quit;
    qint32 numLeds;

public:
    CommsIn(QObject *parent = 0);
    ~CommsIn();
    QStringList getPortInfo();
    void startComms(const QString &portName);

private slots:
    void dataReadyToSend(QList<dataRobotLocation> listRobotLocations);
    void dataLedsTranslated(QList<QString>);

signals:
    void newRobotData(QList<dataRobotLocation> listRobotLocations);
    void newLedData(QList<QString>);
    void newRawData(QByteArray bytarRead);
public slots:
    void run();

};

#endif // COMMSIN_H
