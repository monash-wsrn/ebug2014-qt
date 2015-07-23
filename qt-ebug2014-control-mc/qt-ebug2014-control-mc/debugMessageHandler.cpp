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



/*********************************
** Old version of debug handling
**********************************/
//static QTextEdit *debugTextEdit = 0; Moving this to first usage
//static QString *logFilePath = 0;

//void logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
//{
//    // Write to local string
//    QString line;
//    QTextStream out(&line, QIODevice::WriteOnly);
//    out << QTime::currentTime().toString("hh:mm:ss.zzz ");
//    QByteArray localMsg = msg.toLocal8Bit();
//    switch (type)
//    {
//        case QtDebugMsg:
//            out << "Debug: " << localMsg.constData() << " (" << context.file << ":" << context.line << "," << context.function << ")";
//            break;
//        case QtWarningMsg:
//            out << "Warning: " << localMsg.constData() << " (" << context.file << ":" << context.line << "," << context.function << ")";
//            break;
//        case QtCriticalMsg:
//            out << "Critical: " << localMsg.constData() << " (" << context.file << ":" << context.line << "," << context.function << ")";
//            break;
//        //case QtFatalMsg:
//            //out << "Fatal: " << localMsg.constData() << " (" << context.file << ":" << context.line << "," << context.function << ")";
//            //abort();
//    }
//    out.flush();

//    // Write to UI (always)
//    //debugTextEdit->append(line.toStdString( ).data());

//    // Add new line
//    out << "\n";

//    // Write to file
//    #ifdef LOG_OUTPUT_DEST_FILE
//    QFile file(*logFilePath);
//    if(file.open(QIODevice::Append | QIODevice::Text))
//    {
//        QTextStream out_file(&file);
//        out_file << line.toStdString().data();
//    }
//    #endif

//    // Write to stderr
//    #ifdef LOG_OUTPUT_DEST_STDERR
//    fprintf(stderr, "%s", line.toStdString().data());
//    #endif
//}
