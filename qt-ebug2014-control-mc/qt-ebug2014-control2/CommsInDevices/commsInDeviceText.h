#ifndef COMMSINDEVICETEXT_H
#define COMMSINDEVICETEXT_H

#include <QDialog>

#include "CommsIn.h"

namespace Ui {
class commsInDeviceText;
}

class commsInDeviceText : public QDialog
{
    Q_OBJECT

public:
    explicit commsInDeviceText(QWidget *parent = 0, CommsIn *commsInOutput=0);
    ~commsInDeviceText();

private:
    Ui::commsInDeviceText *ui;
    CommsIn *threadCommsIn;

signals:
    void sendButtonPressed();

private slots:
    void newMessage(dataRobotLocation dataRobotMsg);
    void parseInput();
};

#endif // COMMSINDEVICETEXT_H
