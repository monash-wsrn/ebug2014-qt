#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QMainWindow>

#include "threadWorld.h"

namespace Ui {
class WindowMain;
}

class WindowMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindowMain(QWidget *parent = 0);
    ~WindowMain();

private slots:

private:
    Ui::WindowMain *ui;
    ThreadWorld *thrdWorld;
};

#endif // WINDOWMAIN_H
