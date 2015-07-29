#include "commsInDeviceText.h"
#include "ui_commsInDeviceText.h"

commsInDeviceText::commsInDeviceText(QWidget *parent, CommsIn commsInOutput) :
    QDialog(parent),
    ui(new Ui::commsInDeviceText)
{
    threadCommsIn = commsInOutput;
    connect(ui->btnSend, SIGNAL(released()), ui, SLOT(parseInput()));
    ui->setupUi(this);
}

commsInDeviceText::~commsInDeviceText()
{
    delete ui;
}

void commsInDeviceText::parseInput(){


}
