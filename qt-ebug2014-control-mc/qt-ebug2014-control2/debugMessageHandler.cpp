/* ---------------------------------------------------------------------------
** See copyright.txt
**
** debugMessageHandler.cpp
** Implementation of custom message handler to be installed in main.cpp
** When qDebug, qWarning, qCritical or qFatal are called then a message is
** created in the format:
** Date_stamp Message_type Message_text (FILE:Calling_file  LINE:Called_line    FUNCTION:Calling_func)
** This message is printed to stderr and appended to the file defined in logFilePath="logfile.txt"
**
**
** Author: 2015 Matthew Boys
** Reference: http://doc.qt.io/qt-5/qtglobal.html
** -------------------------------------------------------------------------*/

#include <QFile>
#include <QTextStream>
#include <QTime>

#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>

static QString logFilePath = "/logfile.txt";

void debugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    //Get a date stamp
    QString strDatestamp = QTime::currentTime().toString("hh:mm:ss.zzz");

    // Put content of msg into a string
    QByteArray localMsg = msg.toLocal8Bit();
    QString strMsgContent = QString("%1 (FILE:%2\tLINE:%3\tFUNCTION:%4)").arg( localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);

    // Get a string for the type of error
    QString strMessageType;
    switch (type) {
    case QtDebugMsg:
        strMessageType = "Debug";
        break;
// QtInfoMsg only introduced in Qt 5.5
//    case QtInfoMsg:
//        strMessageType = "Info";
//        break;
    case QtWarningMsg:
        strMessageType = "Warning";
        break;
    case QtCriticalMsg:
        strMessageType = "Critical";
        break;
    case QtFatalMsg:
        strMessageType = "Fatal";
        break;
    }

    //Construct error string
    QString strErrorMessage =  strDatestamp + "\t" + strMessageType + ": " + strMsgContent + "\n";

    //Print error to stderr
    QTextStream txtstrmStderr(stderr);
    txtstrmStderr << strErrorMessage;

    //Print error to log file
    QFile fileLog(logFilePath);
    if(fileLog.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream txtstrmLogFile(&fileLog);
        txtstrmLogFile << strErrorMessage;
        fileLog.close();
        //Broadcast log file change
        //TODO
    }


    //Abort if error was fatal
    if(type == QtFatalMsg){
        abort();
    }
}
