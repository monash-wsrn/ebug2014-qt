#include "commsin.h"

CommsIn::CommsIn(QString strPortName) : ThreadableQObject()
{
    port = new QSerialPort(strPortName);
    port->open(QIODevice::ReadOnly);
    connect(port, SIGNAL(readyRead()), this, SLOT(read()));
}

CommsIn::~CommsIn()
{
    qDebug("Closing port");
    port->close();
}

void CommsIn::run(){
}

void CommsIn::read(){
    qDebug("Sending new message");
    emit newMsg(port->readAll());
}

QStringList CommsIn::getPortInfo(){
    QSerialPortInfo infoPort(*port);
    QStringList strlstInfoPort;
    strlstInfoPort << infoPort.portName()
         << infoPort.description()
         << infoPort.manufacturer()
         << infoPort.systemLocation()
         << (infoPort.hasVendorIdentifier() ? QString::number(infoPort.vendorIdentifier(), 16) : QString())
         << (infoPort.hasProductIdentifier() ? QString::number(infoPort.productIdentifier(), 16) : QString());

    return strlstInfoPort;
}
