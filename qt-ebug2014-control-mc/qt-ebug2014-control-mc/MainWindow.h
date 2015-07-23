// $Revision: 502 $
// $Date: 2014-08-26 17:25:55 +1000 (Tue, 26 Aug 2014) $
// $Author: erwinmochtarwijaya@gmail.com $
/************************************************************************/
/* qt-opencv-multithreaded:                                             */
/* A multithreaded OpenCV application using the Qt framework.           */
/*                                                                      */
/* MainWindow.h                                                         */
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMetaType>
#include <QMainWindow>
#include <QPushButton>
// QtSerialPort
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
// Local
#include "EBugData.h"
#include "CameraConnectDialog.h"
#include "CameraView.h"
#include "Buffer.h"
#include "SharedImageBuffer.h"
#include "XBeeConnectDialog.h"
#include "XBeeThread.h"
#include "ControlThread.h"
#include "ImageView.h" // added by Erwin 20/02/14
#include "RenderThread.h"// added by Erwin 20/02/14

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void setupLoggingUI(QTextEdit *textEdit);

    private:
        Ui::MainWindow *ui;
        QPushButton *connectToCameraButton;
        QPushButton *connectToReceiverButton;// added by Erwin 20/02/14
        QMap<int, int> deviceNumberMap;
        QMap<int, CameraView*> cameraViewMap;
        SharedImageBuffer *sharedImageBuffer;
        Buffer<QByteArray> *xBeeTXPacketBuffer;
        XBeeConnectDialog *xBeeConnectDialog;
        ImageView *imageView; // added by Erwin 20/02/14
        RenderThread *renderThread;// added by Erwin 20/02/14
        XBeeThread *xBeeThread;
        QTimer *nodeDiscoveryTimer;
        ControlThread *controlThread;
        QTextEdit *debugTextEdit;
        bool removeFromMapByTabIndex(QMap<int, int>& map, int tabIndex);
        void updateMapValues(QMap<int, int>& map, int tabIndex);
        void setTabCloseToolTips(QTabWidget *tabs, QString tooltip);
        void setupTables();
        void newBlankCameraTab();

    public slots:
        void connectToCamera();
        void connectToReceiver();// added by Erwin 20/02/14
        void disconnectCamera(int index);
        void showAboutDialog();
        void xBeeConnect();
        void xBeeDisconnect();
        void updateXBeeNodeTable(QList<XBeeNode> xBeeNodeList);
        void trackEBug();
        void updateGoal(int x, int y); //from RenderThread to THIS
        void updateGoalPos();   //from controlThread to THIS
#if 0
        void startTracking();
        void updateTracking(vector<EBugData*>);
        void stopTracking();
#endif
        void startControl();// modified by Erwin 20/02/14
        void stopControl();// modified by Erwin 20/02/14
        void deleteControlThread();// added by Erwin 20/02/14
        void doXBeeNodeDiscovery();
        void clearLog();
        void saveLog();

    signals:
        void newGoalPos(int x, int y);  //from THIS to ControlThread
        void updateTarget();    //from THIS to RenderThread
//private slots:
//        void on_startTrackingButton_clicked();
};

#endif // MAINWINDOW_H
