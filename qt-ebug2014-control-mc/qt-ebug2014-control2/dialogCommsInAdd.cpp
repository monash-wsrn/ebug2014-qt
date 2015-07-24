#include "dialogcommsinadd.h"
#include "ui_dialogcommsinadd.h"

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
