/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionQuit;
    QAction *actionSynchronizeStreams;
    QAction *actionScaleToFitFrame;
    QAction *actionXBeeConnect;
    QAction *actionXBeeDisconnect;
    QAction *actionClick;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QTabWidget *sideTab;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *xBeeConnectButton;
    QPushButton *xBeeDisconnectButton;
    QFrame *line_5;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *xBeeNodeDiscoveryButton;
    QLabel *label_12;
    QLineEdit *xBeeDiscoveryDurationLineEdit;
    QLabel *label_9;
    QTableWidget *xBeeNodeTable;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *startTrackingButton;
    QPushButton *stopTrackingButton;
    QFrame *line;
    QTableWidget *trackingTable;
    QWidget *Control;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *startControlButton;
    QPushButton *stopControlButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuOptions;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(507, 501);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionSynchronizeStreams = new QAction(MainWindow);
        actionSynchronizeStreams->setObjectName(QStringLiteral("actionSynchronizeStreams"));
        actionSynchronizeStreams->setCheckable(true);
        actionScaleToFitFrame = new QAction(MainWindow);
        actionScaleToFitFrame->setObjectName(QStringLiteral("actionScaleToFitFrame"));
        actionScaleToFitFrame->setCheckable(true);
        actionXBeeConnect = new QAction(MainWindow);
        actionXBeeConnect->setObjectName(QStringLiteral("actionXBeeConnect"));
        actionXBeeDisconnect = new QAction(MainWindow);
        actionXBeeDisconnect->setObjectName(QStringLiteral("actionXBeeDisconnect"));
        actionClick = new QAction(MainWindow);
        actionClick->setObjectName(QStringLiteral("actionClick"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabsClosable(false);

        horizontalLayout_2->addWidget(tabWidget);

        sideTab = new QTabWidget(centralWidget);
        sideTab->setObjectName(QStringLiteral("sideTab"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        xBeeConnectButton = new QPushButton(tab);
        xBeeConnectButton->setObjectName(QStringLiteral("xBeeConnectButton"));
        QFont font;
        font.setPointSize(8);
        font.setBold(true);
        font.setWeight(75);
        xBeeConnectButton->setFont(font);

        horizontalLayout_10->addWidget(xBeeConnectButton);

        xBeeDisconnectButton = new QPushButton(tab);
        xBeeDisconnectButton->setObjectName(QStringLiteral("xBeeDisconnectButton"));
        xBeeDisconnectButton->setFont(font);

        horizontalLayout_10->addWidget(xBeeDisconnectButton);


        verticalLayout->addLayout(horizontalLayout_10);

        line_5 = new QFrame(tab);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_5);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        xBeeNodeDiscoveryButton = new QPushButton(tab);
        xBeeNodeDiscoveryButton->setObjectName(QStringLiteral("xBeeNodeDiscoveryButton"));
        QFont font1;
        font1.setPointSize(8);
        xBeeNodeDiscoveryButton->setFont(font1);

        horizontalLayout_13->addWidget(xBeeNodeDiscoveryButton);

        label_12 = new QLabel(tab);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setFont(font);

        horizontalLayout_13->addWidget(label_12);

        xBeeDiscoveryDurationLineEdit = new QLineEdit(tab);
        xBeeDiscoveryDurationLineEdit->setObjectName(QStringLiteral("xBeeDiscoveryDurationLineEdit"));
        xBeeDiscoveryDurationLineEdit->setFont(font1);

        horizontalLayout_13->addWidget(xBeeDiscoveryDurationLineEdit);

        label_9 = new QLabel(tab);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font);

        horizontalLayout_13->addWidget(label_9);


        verticalLayout->addLayout(horizontalLayout_13);

        xBeeNodeTable = new QTableWidget(tab);
        xBeeNodeTable->setObjectName(QStringLiteral("xBeeNodeTable"));
        xBeeNodeTable->setFont(font1);
        xBeeNodeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(xBeeNodeTable);

        sideTab->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        startTrackingButton = new QPushButton(tab_2);
        startTrackingButton->setObjectName(QStringLiteral("startTrackingButton"));
        startTrackingButton->setFont(font);

        horizontalLayout_11->addWidget(startTrackingButton);

        stopTrackingButton = new QPushButton(tab_2);
        stopTrackingButton->setObjectName(QStringLiteral("stopTrackingButton"));
        stopTrackingButton->setFont(font);

        horizontalLayout_11->addWidget(stopTrackingButton);


        verticalLayout_2->addLayout(horizontalLayout_11);

        line = new QFrame(tab_2);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        trackingTable = new QTableWidget(tab_2);
        trackingTable->setObjectName(QStringLiteral("trackingTable"));
        trackingTable->setFont(font1);
        trackingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout_2->addWidget(trackingTable);

        sideTab->addTab(tab_2, QString());
        Control = new QWidget();
        Control->setObjectName(QStringLiteral("Control"));
        verticalLayout_3 = new QVBoxLayout(Control);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        startControlButton = new QPushButton(Control);
        startControlButton->setObjectName(QStringLiteral("startControlButton"));
        startControlButton->setFont(font);

        horizontalLayout->addWidget(startControlButton);

        stopControlButton = new QPushButton(Control);
        stopControlButton->setObjectName(QStringLiteral("stopControlButton"));
        stopControlButton->setFont(font);

        horizontalLayout->addWidget(stopControlButton);


        verticalLayout_3->addLayout(horizontalLayout);

        sideTab->addTab(Control, QString());

        horizontalLayout_2->addWidget(sideTab);

        horizontalLayout_2->setStretch(0, 1);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 507, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(tabWidget, sideTab);
        QWidget::setTabOrder(sideTab, xBeeConnectButton);
        QWidget::setTabOrder(xBeeConnectButton, xBeeDisconnectButton);
        QWidget::setTabOrder(xBeeDisconnectButton, xBeeNodeDiscoveryButton);
        QWidget::setTabOrder(xBeeNodeDiscoveryButton, xBeeDiscoveryDurationLineEdit);
        QWidget::setTabOrder(xBeeDiscoveryDurationLineEdit, xBeeNodeTable);
        QWidget::setTabOrder(xBeeNodeTable, startTrackingButton);
        QWidget::setTabOrder(startTrackingButton, stopTrackingButton);
        QWidget::setTabOrder(stopTrackingButton, trackingTable);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);
        menuOptions->addAction(actionSynchronizeStreams);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);
        sideTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "qt-ebug-control-multicamera", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionSynchronizeStreams->setText(QApplication::translate("MainWindow", "Synchronize streams", 0));
        actionScaleToFitFrame->setText(QApplication::translate("MainWindow", "Scale to fit frame", 0));
        actionXBeeConnect->setText(QApplication::translate("MainWindow", "Connect...", 0));
        actionXBeeDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", 0));
        actionClick->setText(QApplication::translate("MainWindow", "Click", 0));
        xBeeConnectButton->setText(QApplication::translate("MainWindow", "Connect...", 0));
        xBeeDisconnectButton->setText(QApplication::translate("MainWindow", "Disconnect", 0));
        xBeeNodeDiscoveryButton->setText(QApplication::translate("MainWindow", "Discover Nodes...", 0));
        label_12->setText(QApplication::translate("MainWindow", "Duration:", 0));
        label_9->setText(QApplication::translate("MainWindow", "sec(s)", 0));
        sideTab->setTabText(sideTab->indexOf(tab), QApplication::translate("MainWindow", "XBee", 0));
        startTrackingButton->setText(QApplication::translate("MainWindow", "Start...", 0));
        stopTrackingButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        sideTab->setTabText(sideTab->indexOf(tab_2), QApplication::translate("MainWindow", "Tracking", 0));
        startControlButton->setText(QApplication::translate("MainWindow", "Start", 0));
        stopControlButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        sideTab->setTabText(sideTab->indexOf(Control), QApplication::translate("MainWindow", "Control", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
