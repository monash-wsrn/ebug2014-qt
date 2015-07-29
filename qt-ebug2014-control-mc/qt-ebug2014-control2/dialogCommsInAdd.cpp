#include "dialogCommsInAdd.h"
#include "ui_dialogCommsInAdd.h"

dialogCommsInAdd::dialogCommsInAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogCommsInAdd)
{
    ui->setupUi(this);
}

dialogCommsInAdd::~dialogCommsInAdd()
{
    delete ui;
}
