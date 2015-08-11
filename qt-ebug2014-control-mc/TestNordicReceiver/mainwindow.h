#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "commsin.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CommsIn comms;
    void fill_lstbxPortsAvailable();
    void fill_portInfoLabels(int index);

public slots:
    void newMsg(QList<dataRobotLocation>);
    void newRawMsg(QByteArray);
    void newLedMsg(QList<QString>);

private slots:
    void on_lstbxPortsAvailable_currentIndexChanged(int index);
    void on_btnDetect_clicked();
};

#endif // MAINWINDOW_H
