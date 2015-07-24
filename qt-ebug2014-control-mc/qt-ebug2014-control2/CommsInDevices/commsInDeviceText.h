#ifndef COMMSINDEVICETEXT_H
#define COMMSINDEVICETEXT_H

#include <QDialog>

namespace Ui {
class commsInDeviceText;
}

class commsInDeviceText : public QDialog
{
    Q_OBJECT

public:
    explicit commsInDeviceText(QWidget *parent = 0);
    ~commsInDeviceText();

private:
    Ui::commsInDeviceText *ui;
};

#endif // COMMSINDEVICETEXT_H
