// Sets up the MainWindow user interface and starts the processing threads

#include "MainWindow.h"
#include "ui_MainWindow.h"
// Qt
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup UI based on MainWindow.ui form (made in Qt designer)
    ui->setupUi(this);

    // Set initial UI state (initial button states set with Qt Designer)
    ui->linedRobotsDiscoveryDuration->setText(QString::number(DEFAULT_XBEE_ND_DURATION));

    setupTables();

    //Add a blank "No camera connected" tab in camera tabs
    newBlankCameraTab();

    // Add "Connect to Camera" button to tab
    connectToCameraButton = new QPushButton();
    connectToCameraButton->setText("Connect to Camera...");
    ui->tabsCamera->setCornerWidget(connectToCameraButton, Qt::TopLeftCorner);
    // Set focus on button
    connectToCameraButton->setFocus();

    /* Added by Erwin 20/02/14: Creating the Receiver button on the tab*/
    connectToReceiverButton = new QPushButton();
    connectToReceiverButton->setText("Connect to Receiver...");
    ui->tabsCamera->setCornerWidget(connectToReceiverButton, Qt::TopRightCorner);
    connectToReceiverButton->setFocus();
    /************************************************************************/

    // Connect other signals/slots
    connect(connectToReceiverButton,SIGNAL(released()),this, SLOT(connectToReceiver())); //Added by Erwin 20/02/14
    connect(connectToCameraButton,SIGNAL(released()),this, SLOT(connectToCamera()));
    connect(ui->tabsCamera,SIGNAL(tabCloseRequested(int)),this, SLOT(disconnectCamera(int)));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->xBeeConnectButton, SIGNAL(released()), this, SLOT(xBeeConnect()));
    connect(ui->xBeeDisconnectButton, SIGNAL(released()), this, SLOT(xBeeDisconnect()));

//    connect(ui->startTrackingButton, SIGNAL(released()), this, SLOT(startTracking()));
//    connect(ui->stopTrackingButton, SIGNAL(released()), this, SLOT(stopTracking()));

    connect(ui->startControlButton, SIGNAL(released()), this, SLOT(startControl()));
    connect(ui->stopControlButton, SIGNAL(released()), this, SLOT(stopControl()));
    connect(ui->xBeeNodeDiscoveryButton, SIGNAL(released()), this, SLOT(doXBeeNodeDiscovery()));
    
    // Create SharedImageBuffer object
    sharedImageBuffer = new SharedImageBuffer();
    // Create dialog
    xBeeConnectDialog = new XBeeConnectDialog(this);
    // Create timer
    nodeDiscoveryTimer = new QTimer(this);
    // Set timer as single shot
    nodeDiscoveryTimer->setSingleShot(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupLoggingUI(QTextEdit *textEdit)
{
    // Save to member
    debugTextEdit = textEdit;
    // Set font size
    QFont font = debugTextEdit->font();
    font.setPointSize(8);
    font.setBold(false);
    debugTextEdit->setFont(font);
    // Create widget
    QWidget *logPage = new QWidget();
    // Create V layout
    QHBoxLayout *hbox = new QHBoxLayout();
    // Create buttons and add to layout
    QPushButton *clearLogButton = new QPushButton("Clear");
    QPushButton *saveLogButton = new QPushButton("Save to File...");
    hbox->addWidget(clearLogButton);
    hbox->addWidget(saveLogButton);
    // Create signal/slot connections
    connect(clearLogButton, SIGNAL(released()), this, SLOT(clearLog()));
    connect(saveLogButton, SIGNAL(released()), this, SLOT(saveLog()));
    // Create V layout
    QVBoxLayout *vbox = new QVBoxLayout();
    // Add text edit and buttons to layout
    vbox->addWidget(debugTextEdit);
    vbox->addLayout(hbox);
    // Set layout in widget
    logPage->setLayout(vbox);
    // Add widget to new tab
    ui->sideTab->addTab(logPage,"Log");
}

void MainWindow::clearLog()
{
    debugTextEdit->clear();
}

void MainWindow::saveLog()
{
    // Get timestamp
    QLocale c(QLocale::C);
    QDateTime now = QDateTime::currentDateTime();
    QString dtString = c.toString(now, "yyyyMMdd-hhmmsszzz");
    // Show dialog
    QString filename = QFileDialog::getSaveFileName(this, tr("Save log"), QDir::currentPath() + "/" + dtString, tr("Log file (*.log)") );
    if(!filename.isNull())
    {
        QFile ffile(filename + ".log");
        ffile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&ffile);
        out << debugTextEdit->toPlainText();
        ffile.close();
    }
}

void MainWindow::connectToCamera()
{
    // We cannot connect to a camera if devices are already connected and stream synchronization is in progress
    if(ui->actionSynchronizeStreams->isChecked() && deviceNumberMap.size()>0 && sharedImageBuffer->getSyncEnabled())
    {
        // Prompt user
        QMessageBox::warning(this, tr("qt-opencv-multithreaded"),
                                    tr("Stream synchronization is in progress.\n\n"
                                       "Please close all currently open streams before attempting to open a new stream."),
                                        QMessageBox::Ok);
    }
    // Attempt to connect to camera
    else
    {
        // Get next tab index
        int nextTabIndex = (deviceNumberMap.size()==0) ? 0 : ui->tabsCamera->count();
        // Show dialog
        CameraConnectDialog *cameraConnectDialog = new CameraConnectDialog(this, ui->actionSynchronizeStreams->isChecked());
        if(cameraConnectDialog->exec()==QDialog::Accepted)
        {
            // Save user-defined device number
            int deviceNumber = cameraConnectDialog->getDeviceNumber();
            // Check if this camera is already connected
            if(!deviceNumberMap.contains(deviceNumber))
            {
                // Create ImageBuffer with user-defined size
                Buffer<Mat> *imageBuffer = new Buffer<Mat>(cameraConnectDialog->getImageBufferSize());
                // Add created ImageBuffer to SharedImageBuffer object
                sharedImageBuffer->add(deviceNumber, imageBuffer, ui->actionSynchronizeStreams->isChecked());
                // Create CameraView
                cameraViewMap[deviceNumber] = new CameraView(ui->tabsCamera, deviceNumber, sharedImageBuffer);

                // Check if stream synchronization is enabled
                if(ui->actionSynchronizeStreams->isChecked())
                {
                    // Prompt user
                    int ret = QMessageBox::question(this, tr("qt-opencv-multithreaded"),
                                                    tr("Stream synchronization is enabled.\n\n"
                                                       "Do you want to start processing?\n\n"
                                                       "Choose 'No' if you would like to open additional streams."),
                                                    QMessageBox::Yes | QMessageBox::No,
                                                    QMessageBox::Yes);
                    // Start processing
                    if(ret==QMessageBox::Yes)
                        sharedImageBuffer->setSyncEnabled(true);
                    // Defer processing
                    else
                        sharedImageBuffer->setSyncEnabled(false);
                }

                // Attempt to connect to camera
                if(cameraViewMap[deviceNumber]->connectToCamera(cameraConnectDialog->getDropFrameCheckBoxState(),
                                               cameraConnectDialog->getCaptureThreadPrio(),
                                               cameraConnectDialog->getProcessingThreadPrio(),
                                               cameraConnectDialog->getEnableFrameProcessingCheckBoxState(),
                                               cameraConnectDialog->getResolutionWidth(),
                                               cameraConnectDialog->getResolutionHeight()))
                {
                    // Add to map
                    deviceNumberMap[deviceNumber] = nextTabIndex;
                    // Save tab label
                    QString tabLabel = cameraConnectDialog->getTabLabel();
                    // Allow tabs to be closed
                    ui->tabsCamera->setTabsClosable(true);
                    // If start tab, remove
                    if(nextTabIndex==0)
                        ui->tabsCamera->removeTab(0);
                    // Add tab
                    ui->tabsCamera->addTab(cameraViewMap[deviceNumber], tabLabel + " [" + QString::number(deviceNumber) + "]");
                    ui->tabsCamera->setCurrentWidget(cameraViewMap[deviceNumber]);
                    // Set tooltips
                    setTabCloseToolTips(ui->tabsCamera, "Disconnect Camera");
                    // Prevent user from enabling/disabling stream synchronization after a camera has been connected
                    ui->actionSynchronizeStreams->setEnabled(false);
                }
                // Could not connect to camera
                else
                {
                    // Display error message
                    QMessageBox::warning(this,"ERROR:","Could not connect to camera. Please check device number.");
                    // Explicitly delete widget
                    delete cameraViewMap[deviceNumber];
                    cameraViewMap.remove(deviceNumber);
                    // Remove from shared buffer
                    sharedImageBuffer->removeByDeviceNumber(deviceNumber);
                    // Explicitly delete ImageBuffer object
                    delete imageBuffer;
                }
            }
            // Display error message
            else
                QMessageBox::warning(this,"ERROR:","Could not connect to camera. Already connected.");
        }
        // Delete dialog
        delete cameraConnectDialog;
    }
}

void MainWindow::connectToReceiver() { 

    // Check if XBeeThread has already been created
    if(!(xBeeThread->isRunning()))
        {
        // Display error message
        QMessageBox::warning(this, "ERROR:","No XBee nodes found.\n\n1. Ensure XBeeThread is already running.\n");
    }
    else {

    // Create Control thread
    controlThread = new ControlThread(xBeeTXPacketBuffer,xBeeThread->xBeeNodeList);
    connect(controlThread,SIGNAL(controlThreadStopped()),this,SLOT(deleteControlThread()));
    connect(this,SIGNAL(newGoalPos(int,int)),controlThread,SLOT(updateTarget(int,int)));
    connect(controlThread,SIGNAL(targetReached()),this, SLOT(updateGoalPos()));
    }

    //Create Render Thread
    renderThread = new RenderThread(xBeeThread->xBeeNodeList.size());
    imageView = new ImageView(this);
    connect(renderThread,SIGNAL(updateTrackingData()),this,SLOT(trackEBug()));
    connect(renderThread,SIGNAL(newFrame(QImage)),imageView,SLOT(updateFrame(QImage)));
    connect(renderThread,SIGNAL(newTargetLocation(int,int)),this,SLOT(updateGoal(int,int)));
    connect(this,SIGNAL(updateTarget()),renderThread,SLOT(updateTargetPosition()));

    renderThread->start(QThread::TimeCriticalPriority);
    imageView->show();
}

void MainWindow::disconnectCamera(int index)
{
    bool doDisconnect;//initialise local variable

    // Check if stream synchronization is enabled, more than 1 camera connected, and frame processing is not in progress
    if(ui->actionSynchronizeStreams->isChecked() && cameraViewMap.size()>1 && !sharedImageBuffer->getSyncEnabled())
    {
        // Prompt user
        int ret = QMessageBox::question(this, tr("qt-opencv-multithreaded"),
                                        tr("Stream synchronization is enabled.\n\n"
                                           "Disconnecting this camera will cause frame processing to begin on other streams.\n\n"
                                           "Do you wish to proceed?"),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::Yes);
        // Disconnect
        if(ret==QMessageBox::Yes)
            doDisconnect=true;
        // Do not disconnect
        else
            doDisconnect=false;
    }

    // Disconnect camera
    if(doDisconnect)
    {
        // Save number of tabs
        int nTabs = ui->tabsCamera->count();
        // Close tab
        ui->tabsCamera->removeTab(index);

        // Delete widget (CameraView) contained in tab
        delete cameraViewMap[deviceNumberMap.key(index)];
        cameraViewMap.remove(deviceNumberMap.key(index));

        // Remove from map
        removeFromMapByTabIndex(deviceNumberMap, index);
        // Update map (if tab closed is not last)
        if(index!=(nTabs-1))
            updateMapValues(deviceNumberMap, index);

        // If start tab, set tab as blank
        if(nTabs==1)
        {
            newBlankCameraTab();
            ui->actionSynchronizeStreams->setEnabled(true);
        }
    }
}

/**
 * @brief MainWindow::newBlankCameraTab
 * Places a blank tab with no text in the tabsCamera tabs with
 * the label "No camera connected.".
 * Sets the tabsCamera to not closeable.
 * Used at setup and when no. camera tabs is zero.
 */
void MainWindow::newBlankCameraTab(){
    QLabel *newTab = new QLabel(ui->tabsCamera);
    newTab->setText("No camera connected.");
    newTab->setAlignment(Qt::AlignCenter);
    ui->tabsCamera->addTab(newTab, "");
    ui->tabsCamera->setTabsClosable(false);
}

void MainWindow::showAboutDialog()
{
    QMessageBox::information(this, "About", QString("Authors:\nNick D'Ademo (nickdademo@gmail.com)\n\nVersion: %1").arg(APP_VERSION));
}

bool MainWindow::removeFromMapByTabIndex(QMap<int, int>& map, int tabIndex)
{
    QMutableMapIterator<int, int> i(map);
    while (i.hasNext())
    {
         i.next();
         if(i.value()==tabIndex)
         {
             i.remove();
             return true;
         }
    }
    return false;
}

void MainWindow::updateMapValues(QMap<int, int>& map, int tabIndex)
{
    QMutableMapIterator<int, int> i(map);
    while (i.hasNext())
    {
        i.next();
        if(i.value()>tabIndex)
            i.setValue(i.value()-1);
    }
}

void MainWindow::setTabCloseToolTips(QTabWidget *tabs, QString tooltip)
{
    QList<QAbstractButton*> allPButtons = tabs->findChildren<QAbstractButton*>();
    for (int ind = 0; ind < allPButtons.size(); ind++)
    {
        QAbstractButton* item = allPButtons.at(ind);
        if (item->inherits("CloseButton"))
            item->setToolTip(tooltip);
    }
}

void MainWindow::xBeeConnect()
{
    // Show dialog
    if(xBeeConnectDialog->exec()==QDialog::Accepted)
    {
        // Store settings from UI
        xBeeConnectDialog->updateSettings();
        // Get settings
        XBeeConnectDialog::Settings p = xBeeConnectDialog->settings();
        // Creates a buffer which stores XBee TX packet
        xBeeTXPacketBuffer = new Buffer<QByteArray>(p.txPacketBufferSize);
        // Create XBee thread
        xBeeThread = new XBeeThread(xBeeTXPacketBuffer, p);
        // Successful serial connect
        if(xBeeThread->port->isOpen())
        {
            // Start thread
            xBeeThread->start((QThread::Priority) p.threadPriority);
            // Set UI
            ui->xBeeConnectButton->setEnabled(false);
            ui->xBeeDisconnectButton->setEnabled(true);
            ui->xBeeNodeDiscoveryButton->setEnabled(true);
            // Signal/slot connections
            connect(nodeDiscoveryTimer, SIGNAL(timeout()), xBeeThread, SLOT(nodeDiscoveryTimeout()));
            connect(xBeeThread,SIGNAL(newXBeeNodeData(QList<XBeeNode>)),this,SLOT(updateXBeeNodeTable(QList<XBeeNode>)));

        }
        // Unsuccessful serial connect
        else
        {
            // Delete XBee thread object
            delete xBeeThread;
            // Delete XBee TX packet buffer object
            delete xBeeTXPacketBuffer;
            // Show message box
            QMessageBox::critical(this, tr("Error"), xBeeThread->port->errorString());
        }
    }
}

void MainWindow::xBeeDisconnect()
{
    qDebug() << "About to stop XBee thread...";
    // Stop XBee thread
    xBeeThread->stop();
    // If TX buffer is empty, add empty packet to allow thread to finish
    if(xBeeTXPacketBuffer->isEmpty())
        xBeeTXPacketBuffer->add(QByteArray());
    xBeeThread->wait();
    qDebug() << "XBee thread successfully stopped.";
    // Delete XBee thread (closes serial port if open)
    delete xBeeThread;
    // Set UI
    ui->xBeeConnectButton->setEnabled(true);
    ui->xBeeDisconnectButton->setEnabled(false);
    ui->xBeeNodeDiscoveryButton->setEnabled(false);
}

void MainWindow::doXBeeNodeDiscovery()
{
    // Validate duration
    if(ui->linedRobotsDiscoveryDuration->text().isEmpty())
    {
        // Set to default
        ui->linedRobotsDiscoveryDuration->setText(QString::number(DEFAULT_XBEE_ND_DURATION));
        // Display error message
        QMessageBox::warning(this, "ERROR:","Discovery duration field blank.\nAutomatically set to default value.");
    }
    // Get duration from UI
    int duration=ui->linedRobotsDiscoveryDuration->text().toInt();
    // Perform check
    if(duration<DEFAULT_MIN_XBEE_ND_DURATION)
    {
        // Display error message
        QMessageBox::warning(this,"ERROR:","ERROR: XBee node discovery duration must be >="+QString::number(DEFAULT_MIN_XBEE_ND_DURATION)+QString(" seconds."));
    }
    else
    {
        // Clear node list
        xBeeThread->xBeeNodeList.clear();
        // Clear XBee node table
        ui->xBeeNodeTable->clearContents();
        for(int i=0; i<ui->xBeeNodeTable->rowCount(); i++)
            ui->xBeeNodeTable->removeRow(i);
        // Send Node Discovery packet
        xBeeThread->doNodeDiscovery();
        // Start timer
        nodeDiscoveryTimer->start(1000*duration);
        // Set UI
        ui->linedRobotsDiscoveryDuration->setEnabled(false);
        ui->xBeeNodeDiscoveryButton->setEnabled(false);
        ui->xBeeNodeDiscoveryButton->setText("Wait...");
        ui->xBeeDisconnectButton->setEnabled(false);
    }
}

void MainWindow::updateXBeeNodeTable(QList<XBeeNode> xBeeNodeList)
{
    // Check if any nodes were found
    if(xBeeNodeList.size()==0)
    {
        // Display error message
        QMessageBox::warning(this, "ERROR:","No XBee nodes found.\n\n1. Ensure all nodes are powered on and associated (ASSOC LED flashing).\n2. Increase the discovery duration and try again.");
    }
    else
    {
        // Rows
        for(int i=0;i<xBeeNodeList.size();i++)
        {
            // Save current node
            XBeeNode node = xBeeNodeList.at(i);
            // Insert row
            ui->xBeeNodeTable->insertRow(i);
            // Copy NI string to QString
            QString NI;
            int k=0;
            while((node.NI[k+1]!=0x00)&&(k<20))
            {
                NI[k]=node.NI[k+1];
                k++;
            }
            // Columns
            for(int j=0;j<ui->xBeeNodeTable->columnCount();j++)
            {
                switch(j)
                {
                    // Serial Number
                    case 0:
                        ui->xBeeNodeTable->setItem(i, j, new QTableWidgetItem(QString("%1%2%3%4").arg(node.SH_high,0,16).arg(node.SH_low,0,16).arg(node.SL_high,0,16).arg(node.SL_low,0,16)));
                        break;
                    // Network Address
                    case 1:
                        ui->xBeeNodeTable->setItem(i, j, new QTableWidgetItem(QString("%1").arg(node.MY,0,16)));
                        break;
                    // Node ID
                    case 2:
                        ui->xBeeNodeTable->setItem(i, j, new QTableWidgetItem(NI,0));
                        break;
                    // Parent Network Address
                    case 3:
                        ui->xBeeNodeTable->setItem(i, j, new QTableWidgetItem(QString("%1").arg(node.parentNetworkAddress,0,16)));
                        break;
                    // Device Type
                    case 4:
                        ui->xBeeNodeTable->setItem(i, j, new QTableWidgetItem(QString("%1").arg(node.deviceType,0,16)));
                        break;
                    // Status
                    case 5:
                        ui->xBeeNodeTable->setItem(i, j, new QTableWidgetItem(QString("%1").arg(node.status,0,16)));
                        break;
                    // Profile ID
                    case 6:
                        ui->xBeeNodeTable->setItem(i, j, new QTableWidgetItem(QString("%1").arg(node.profileID,0,16)));
                        break;
                    // Manufacturer ID
                    case 7:
                        ui->xBeeNodeTable->setItem(i, j, new QTableWidgetItem(QString("%1").arg(node.manufacturerID,0,16)));
                        break;
                 }
            }
        }
        // Resize table
        ui->xBeeNodeTable->resizeColumnsToContents();
    }
    // Set UI
    ui->linedRobotsDiscoveryDuration->setEnabled(true);
    ui->xBeeNodeDiscoveryButton->setEnabled(true);
    ui->xBeeNodeDiscoveryButton->setText("Discover Nodes...");
    ui->xBeeDisconnectButton->setEnabled(true);
    ui->startControlButton->setEnabled(true);//allow user to start controlling the eBugs
}
#if 1
void MainWindow::trackEBug()
{
#if 0
    QList<EBugData*> eBugDataPtr;
    for (u_int i = 0;i < this->renderThread->getEBugData().size();i++){
    ebugdata = new EBugData(this->renderThread->getEBugData()[i]->getID(),this->renderThread->getEBugData()[i]->getPosition(),this->renderThread->getEBugData()[i]->getAngle());
    eBugDataPtr.append(ebugdata);
    qDebug() << "EBug's location from MainWindow::trackEBug = ";
    qDebug() << ebugdata->getPosition.x()<<"\t"<< ebugdata->getPosition.y() << "\n";
    delete ebugdata;
    }
    this->controlThread->setEbugDataVector(eBugDataPtr);
#else
    this->controlThread->setEbugDataVector(this->renderThread->getEBugData());
#endif

}

void MainWindow::updateGoal(int x, int y)
{
    emit newGoalPos(x,y);
}

void MainWindow::updateGoalPos()
{
    emit updateTarget();
}

/*****************************************Modified by Erwin 20/02/14********************************************************/
#else

void MainWindow::startTracking()
{   
    ui->startTrackingButton->setEnabled(false);
    ui->stopTrackingButton->setEnabled(true);
}

// ########## Sherry added 12/06/2013
void MainWindow::updateTracking(vector<EBugData*> eBugdata)
{
    qDebug() << "update tracking";
    qDebug() << "the number of eBugs tracked" << eBugdata.size();

    //validate the location before updating it
    if (fabs(eBugdata[0]->position.x()) > 0.01 && fabs(eBugdata[0]->position.y()) > 0.01){
      this->controlThread->setEbugDataVector(eBugdata);
    }
}

void MainWindow::stopTracking()
{
    // Insert code here

    // Set UI
    ui->startTrackingButton->setEnabled(true);
    ui->stopTrackingButton->setEnabled(false);
}
#endif

void MainWindow::startControl()
{
    this->controlThread->initializeEbugDataVector();
    this->controlThread->start(QThread::HighestPriority);//only start controlling the eBug when receiver is connected

    // Set UI
    ui->startControlButton->setEnabled(false);
    ui->stopControlButton->setEnabled(true);

    qDebug() << "control thread started";

    // enable tracking
    ui->startTrackingButton->setEnabled(true);
    ui->stopTrackingButton->setEnabled(false);
}

void MainWindow::stopControl()
{
    // ######### Sherry added 22/06/2013
    // Stop thread
    controlThread->stop();
    qDebug() << "control thread stopped";
}


void MainWindow::deleteControlThread()
{
    // delete control thread
    delete controlThread;
    qDebug() << "control thread deleted";

    // Set UI
    ui->startControlButton->setEnabled(true);
    ui->stopControlButton->setEnabled(false);
}

void MainWindow::setupTables()
{
    // Local variables
    QStringList trackingTableHeaderList;
    QStringList xBeeNodeTableHeaderList;
    /* Tracking table */
    // Set column count
    ui->trackingTable->setColumnCount(4);
    // Set horizontal header label
    trackingTableHeaderList<<"ID"<<"X"<<"Y"<<"Angle (degrees)";
    ui->trackingTable->setHorizontalHeaderLabels(trackingTableHeaderList);
    // Set longest possible length values to set appropriate column width
    ui->trackingTable->setItem(0, 0, new QTableWidgetItem(("9999")));
    ui->trackingTable->setItem(0, 1, new QTableWidgetItem(("9999")));
    ui->trackingTable->setItem(0, 2, new QTableWidgetItem(("9999")));
    ui->trackingTable->setItem(0, 3, new QTableWidgetItem(("-360.00")));
    ui->trackingTable->setItem(0, 4, new QTableWidgetItem(("100.00")));
    ui->trackingTable->resizeColumnsToContents();
    ui->trackingTable->clearContents();
    /* XBee Node table */
    // Set dimensions
    ui->xBeeNodeTable->setColumnCount(8);
    // Set horizontal header label
    xBeeNodeTableHeaderList<<"Serial Number"<<"Network Address"<<"Node ID"<<"Parent Network Address"<<"Device Type"<<"Status"<<"Profile ID"<<"Manufacturer ID";
    ui->xBeeNodeTable->setHorizontalHeaderLabels(xBeeNodeTableHeaderList);
    // Resize table
    ui->xBeeNodeTable->resizeColumnsToContents();
}
