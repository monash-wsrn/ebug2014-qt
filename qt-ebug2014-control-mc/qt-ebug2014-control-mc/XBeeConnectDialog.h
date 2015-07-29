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
