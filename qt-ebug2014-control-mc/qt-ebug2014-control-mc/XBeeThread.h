#ifndef XBEETHREAD_H
#define XBEETHREAD_H

// Qt
#include <QThread>
#include <QDebug>
#include <QMutex>
// QtSerialPort
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
// Local
#include <Buffer.h>
#include <XBeeConnectDialog.h>
#include <Structures.h>

#include <ebug-api-cpp/eBugAPI.h>

class XBeeThread : public QThread
{
    Q_OBJECT

    public:
        XBeeThread(Buffer<QByteArray> *txPacketBuffer, XBeeConnectDialog::Settings);
        ~XBeeThread();
        QString getPortErrorString();
        bool getPortIsOpen();
        void stop();
        QList<XBeeNode> getXBeeNodeList();
        int getXBeeNodeListSize();
        void xBeeNodeListClear();

    private:
        QSerialPort *port;
        QList<XBeeNode> xBeeNodeList;

    private:
        Buffer<QByteArray> *txPacketBuffer;
        QMutex doStopMutex;
        QMutex protectXBeeNodeList;
        volatile bool doStop;
        int threadSleep_ms;
        char calculateChecksum(QByteArray *packet);
        void readPacket(unsigned char data);
        void handleATCommandResponse(unsigned char *packet);

    protected:
        void run();

    public slots:
        void doNodeDiscovery();
        void nodeDiscoveryTimeout();

    private slots:
        void slotRead();
        void write(const QByteArray &data);

    signals:
        void writeData(const QByteArray &data);
        void newXBeeNodeData(QList<XBeeNode> xBeeNodeList);
};

#endif // XBEETHREAD_H
