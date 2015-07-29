#ifndef COMMSIN_H
#define COMMSIN_H

#include <QObject>
#include <QtSerialPort>

#include "ThreadableQObject.h"

class CommsIn : public ThreadableQObject
{
    Q_OBJECT

private:
    QSerialPort* port;

public:
    explicit CommsIn(QString strPortName);
    ~CommsIn();
    QStringList getPortInfo();

private slots:
    void read();

signals:
    void newMsg(QByteArray);
public slots:
    void run();

};

#endif // COMMSIN_H
