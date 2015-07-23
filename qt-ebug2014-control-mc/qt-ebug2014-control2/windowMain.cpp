#include "windowMain.h"
#include "ui_windowMain.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QThread>

#include "threadWorld.h"

/**
 * @brief WindowMain::WindowMain
 * Start up the user interface defined in WindowMain.ui
 * Spawn the world thread which manages robots, comms in, comms out, environment, image rendering
 * Set up connections between events in the world thread and the UI
 * @param parent
 */
WindowMain::WindowMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowMain)
{
    ui->setupUi(this);

    ThreadWorld *thrdWorld = new ThreadWorld();

    //Set up connections from ui to world thread


}

WindowMain::~WindowMain()
{
    delete ui;
}



