// $Revision: 504 $
// $Date: 2014-09-09 08:26:14 +1000 (Tue, 09 Sep 2014) $
// $Author: erwinmochtarwijaya@gmail.com $
/************************************************************************/
/* qt-opencv-multithreaded:                                             */
/* A multithreaded OpenCV application using the Qt framework.           */
/*                                                                      */
/* main.cpp                                                             */
/*                                                                      */
/* Nick D'Ademo <nickdademo@gmail.com>                                  */
/*                                                                      */
/* Copyright (c) 2012-2013 Nick D'Ademo                                 */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* The above copyright notice and this permission notice shall be       */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  */
/* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   */
/* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     */
/* SOFTWARE.                                                            */
/*                                                                      */
/************************************************************************/

#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "MainWindow.h"
#include "debugMessageHandler.cpp" //Implementation of debugMessageHandler

int main(int argc, char *argv[])
{
//                // Get timestamp
//                QLocale c(QLocale::C);
//                QDateTime now = QDateTime::currentDateTime();
//                QString dtString = c.toString(now, "yyyyMMdd-hhmmsszzz");
//                logFilePath = new QString(dtString + ".log");
//                // Setup logging
//                qRegisterMetaType<QTextCursor>("QTextCursor");
//                qInstallMessageHandler(logOutput);

    //Install custom debug message handler implemented in debugMessageHandler.cpp
    qInstallMessageHandler(debugMessageHandler);
    // Show main window
    QApplication a(argc, argv);
    MainWindow w;
    //QTextEdit *debugTextEdit = new QTextEdit();
    //w.setupLoggingUI(debugTextEdit);
    w.show();


    qDebug("Test message");

    // Start event loop
    return a.exec();
}
