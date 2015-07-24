#include "commsInDeviceText.h"
#include "ui_commsInDeviceText.h"

commsInDeviceText::commsInDeviceText(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::commsInDeviceText)
{
    ui->setupUi(this);
}

commsInDeviceText::~commsInDeviceText()
{
    delete ui;
}
