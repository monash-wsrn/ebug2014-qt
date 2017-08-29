/************************************************************************/
/*          Written by Erwin Mochtar Wijaya on 05/02/2014              */
/**********************************************************************/

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QDialog>

namespace Ui {
class ImageView;
}

class ImageView : public QDialog
{
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = 0);
    ~ImageView();

private:
    Ui::ImageView *ui;

private slots:
    void updateFrame(const QImage &frame);
};

#endif // IMAGEVIEW_H
