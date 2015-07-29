#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "commsin.h"
#include "messagetranslator.h"

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
    CommsIn *comms;
    void fill_lstbxPortsAvailable();
    void fill_portInfoLabels(int index);
    MessageTranslator *translate;

public slots:
    void newMsg(QByteArray);

private slots:
    void on_lstbxPortsAvailable_currentIndexChanged(int index);
    void on_btnDetect_clicked();
};

#endif // MAINWINDOW_H
