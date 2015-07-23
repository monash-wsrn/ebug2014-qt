/************************************************************************/
/*          Written by Erwin Mochtar Wijaya on 05/02/2014              */
/**********************************************************************/

#include "ImageView.h"
#include "ui_ImageView.h"

ImageView::ImageView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageView)
{
    ui->setupUi(this);
}

ImageView::~ImageView()
{
    delete ui;
}

void ImageView::updateFrame(const QImage &frame)
{
    //Display the new frame
    ui->label->setPixmap(QPixmap::fromImage(frame));
}
