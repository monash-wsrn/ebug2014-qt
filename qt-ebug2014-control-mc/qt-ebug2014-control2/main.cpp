/* ---------------------------------------------------------------------------
** See copyright.txt
**
** main.cpp
** Launches Qt application and main window.
**
** Author: 2015 Matthew Boys
** Author: 2013 Nick D'Ademo <nickdademo@gmail.com>
** -------------------------------------------------------------------------*/

//User interfaces
#include "windowMain.h"
//Qt libraries
#include <QApplication>
#include <QObject>
//Custom implementations
#include "debugMessageHandler.cpp"


/**
 * @brief main: Launches app and main window.
 * @param argc, argv : Command line arguments not implemented
 * @return : 0 if app calls exit via quit
 */
int main(int argc, char *argv[])
{
    //Install custom debug message handler implemented in debugMessageHandler.cpp
    qInstallMessageHandler(debugMessageHandler);

    QApplication app(argc, argv);
    // Show main window
    WindowMain winMain;
    winMain.show();

    // Launch app
    return app.exec();
}
