// $Revision: 422 $
// $Date: 2013-08-11 20:09:38 +1000 (Sun, 11 Aug 2013) $
// $Author: nickdademo@gmail.com $
/************************************************************************/
/* qt-opencv-multithreaded:                                             */
/* A multithreaded OpenCV application using the Qt framework.           */
/*                                                                      */
/* XBeeThread.h                                                         */
/*                                                                      */
/* Nick D'Ademo <nickdademo@gmail.com>                                  */
/*                                                                      */
/* Copyright (c) 2012-2013 Nick D'Ademo                                 */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* The above copyright notice and this permission notice shall be       */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  */
/* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   */
/* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     */
/* SOFTWARE.                                                            */
/*                                                                      */
/************************************************************************/

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
        QSerialPort *port;
        QList<XBeeNode> xBeeNodeList;
        void stop();

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
