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

#include <QSerialPort>
#include <QSerialPortInfo>
#include "MainWindow.h"
#include <QApplication>

//static QTextEdit *debugTextEdit = 0; Moving this to first usage
static QString *logFilePath = 0;

void logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Write to local string
    QString line;
    QTextStream out(&line, QIODevice::WriteOnly);
    out << QTime::currentTime().toString("hh:mm:ss.zzz ");
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
        case QtDebugMsg:
            out << "Debug: " << localMsg.constData() << " (" << context.file << ":" << context.line << "," << context.function << ")";
            break;
        case QtWarningMsg:
            out << "Warning: " << localMsg.constData() << " (" << context.file << ":" << context.line << "," << context.function << ")";
            break;
        case QtCriticalMsg:
            out << "Critical: " << localMsg.constData() << " (" << context.file << ":" << context.line << "," << context.function << ")";
            break;
        //case QtFatalMsg:
            //out << "Fatal: " << localMsg.constData() << " (" << context.file << ":" << context.line << "," << context.function << ")";
            //abort();
    }
    out.flush();

    // Write to UI (always)
    //debugTextEdit->append(line.toStdString( ).data());

    // Add new line
    out << "\n";

    // Write to file
    #ifdef LOG_OUTPUT_DEST_FILE
    QFile file(*logFilePath);
    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out_file(&file);
        out_file << line.toStdString().data();
    }
    #endif

    // Write to stderr
    #ifdef LOG_OUTPUT_DEST_STDERR
    fprintf(stderr, "%s", line.toStdString().data());
    #endif
}

int main(int argc, char *argv[])
{
    // Get timestamp
    QLocale c(QLocale::C);
    QDateTime now = QDateTime::currentDateTime();
    QString dtString = c.toString(now, "yyyyMMdd-hhmmsszzz");
    logFilePath = new QString(dtString + ".log");
    // Setup logging
    qRegisterMetaType<QTextCursor>("QTextCursor");
    qInstallMessageHandler(logOutput);
    // Show main window
    QApplication a(argc, argv);
    MainWindow w;
    QTextEdit *debugTextEdit = new QTextEdit();
    w.setupLoggingUI(debugTextEdit);
    w.show();


    qDebug("Test message");

    // Start event loop
    return a.exec();
}
