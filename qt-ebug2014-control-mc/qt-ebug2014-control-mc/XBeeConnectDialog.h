// $Revision: 422 $
// $Date: 2013-08-11 20:09:38 +1000 (Sun, 11 Aug 2013) $
// $Author: nickdademo@gmail.com $
/************************************************************************/
/* qt-opencv-multithreaded:                                             */
/* A multithreaded OpenCV application using the Qt framework.           */
/*                                                                      */
/* XBeeConnectDialog.h                                                  */
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

#ifndef XBEECONNECTDIALOG_H
#define XBEECONNECTDIALOG_H

// Qt
#include <QDialog>
#include <QIntValidator>
#include <QLineEdit>
// QtSerialPort
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
// Local
#include <Config.h>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class XBeeConnectDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class XBeeConnectDialog : public QDialog
{
    Q_OBJECT
    
    public:
        struct Settings {
            QString name;
            qint32 baudRate;
            QString stringBaudRate;
            QSerialPort::DataBits dataBits;
            QString stringDataBits;
            QSerialPort::Parity parity;
            QString stringParity;
            QSerialPort::StopBits stopBits;
            QString stringStopBits;
            QSerialPort::FlowControl flowControl;
            QString stringFlowControl;
            int threadSleep_ms;
            int threadPriority;
            int txPacketBufferSize;
            bool dropPacket;
        };
        explicit XBeeConnectDialog(QWidget *parent = 0);
        ~XBeeConnectDialog();
        Settings settings() const;
        void updateSettings();

    private:
        Ui::XBeeConnectDialog *ui;
        Settings currentSettings;
        QIntValidator *intValidator;
        void fillPortsParameters();

    private slots:
        void fillPortsInfo();
        void showPortInfo(int idx);
        void checkCustomBaudRatePolicy(int idx);
        void setUIDefaults();
};

#endif // XBEECONNECTDIALOG_H
