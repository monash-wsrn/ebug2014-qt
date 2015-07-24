#ifndef DIALOGCOMMSINADD_H
#define DIALOGCOMMSINADD_H

#include <QDialog>

namespace Ui {
class dialogCommsInAdd;
}

class dialogCommsInAdd : public QDialog
{
    Q_OBJECT

public:
    explicit dialogCommsInAdd(QWidget *parent = 0);
    ~dialogCommsInAdd();

private:
    Ui::dialogCommsInAdd *ui;
};

#endif // DIALOGCOMMSINADD_H
