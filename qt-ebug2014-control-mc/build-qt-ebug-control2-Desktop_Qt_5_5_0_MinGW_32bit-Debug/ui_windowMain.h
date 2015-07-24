/********************************************************************************
** Form generated from reading UI file 'windowMain.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWMAIN_H
#define UI_WINDOWMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WindowMain
{
public:
    QAction *actionFileQuit;
    QAction *actionCommsAdd;
    QAction *actionEBugs_Out;
    QAction *actionSave_All_Logs;
    QWidget *widgetlayoutMain;
    QHBoxLayout *horizontalLayout;
    QGroupBox *grpbxWorld;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicswinWorld;
    QGroupBox *grpbxWorldEvents;
    QGridLayout *gridLayout_3;
    QTextBrowser *textbrowWorldEvents;
    QTabWidget *tabsMain;
    QWidget *tabRobots;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableRobots;
    QWidget *tabCommsIn;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *layoutCommsInBtns;
    QPushButton *btnCommsInAdd;
    QSpacerItem *spacerCommsInBtns;
    QPushButton *btnCommsInDelete;
    QSplitter *splitterCommsIn;
    QTableWidget *tableCommsIn;
    QGroupBox *grpbxCommsIn;
    QGridLayout *gridLayout_2;
    QTextBrowser *textbrowCommsInMsgs;
    QWidget *tabCommsOut;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *layoutCommsOutBtns;
    QPushButton *btnCommsOutAdd;
    QSpacerItem *spacerCommsOutBtns;
    QPushButton *btnCommsOutDelete;
    QSplitter *splitterCommsOut;
    QTableWidget *tableCommsOut;
    QGroupBox *grpbxCommsOut;
    QGridLayout *gridLayout;
    QTextBrowser *textbrowCommsOutMsgs;
    QMenuBar *menubarMain;
    QMenu *menuFile;
    QMenu *menuConnect;

    void setupUi(QMainWindow *WindowMain)
    {
        if (WindowMain->objectName().isEmpty())
            WindowMain->setObjectName(QStringLiteral("WindowMain"));
        WindowMain->resize(927, 688);
        actionFileQuit = new QAction(WindowMain);
        actionFileQuit->setObjectName(QStringLiteral("actionFileQuit"));
        actionCommsAdd = new QAction(WindowMain);
        actionCommsAdd->setObjectName(QStringLiteral("actionCommsAdd"));
        actionEBugs_Out = new QAction(WindowMain);
        actionEBugs_Out->setObjectName(QStringLiteral("actionEBugs_Out"));
        actionEBugs_Out->setEnabled(false);
        actionSave_All_Logs = new QAction(WindowMain);
        actionSave_All_Logs->setObjectName(QStringLiteral("actionSave_All_Logs"));
        widgetlayoutMain = new QWidget(WindowMain);
        widgetlayoutMain->setObjectName(QStringLiteral("widgetlayoutMain"));
        horizontalLayout = new QHBoxLayout(widgetlayoutMain);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        grpbxWorld = new QGroupBox(widgetlayoutMain);
        grpbxWorld->setObjectName(QStringLiteral("grpbxWorld"));
        QFont font;
        font.setPointSize(12);
        grpbxWorld->setFont(font);
        verticalLayout = new QVBoxLayout(grpbxWorld);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        graphicswinWorld = new QGraphicsView(grpbxWorld);
        graphicswinWorld->setObjectName(QStringLiteral("graphicswinWorld"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicswinWorld->sizePolicy().hasHeightForWidth());
        graphicswinWorld->setSizePolicy(sizePolicy);
        graphicswinWorld->setMinimumSize(QSize(500, 500));
        QFont font1;
        font1.setPointSize(9);
        graphicswinWorld->setFont(font1);

        verticalLayout->addWidget(graphicswinWorld);

        grpbxWorldEvents = new QGroupBox(grpbxWorld);
        grpbxWorldEvents->setObjectName(QStringLiteral("grpbxWorldEvents"));
        grpbxWorldEvents->setFont(font1);
        gridLayout_3 = new QGridLayout(grpbxWorldEvents);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        textbrowWorldEvents = new QTextBrowser(grpbxWorldEvents);
        textbrowWorldEvents->setObjectName(QStringLiteral("textbrowWorldEvents"));

        gridLayout_3->addWidget(textbrowWorldEvents, 0, 0, 1, 1);


        verticalLayout->addWidget(grpbxWorldEvents);


        horizontalLayout->addWidget(grpbxWorld);

        tabsMain = new QTabWidget(widgetlayoutMain);
        tabsMain->setObjectName(QStringLiteral("tabsMain"));
        QFont font2;
        font2.setPointSize(10);
        tabsMain->setFont(font2);
        tabsMain->setUsesScrollButtons(false);
        tabRobots = new QWidget();
        tabRobots->setObjectName(QStringLiteral("tabRobots"));
        verticalLayout_2 = new QVBoxLayout(tabRobots);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tableRobots = new QTableWidget(tabRobots);
        if (tableRobots->columnCount() < 4)
            tableRobots->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableRobots->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableRobots->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableRobots->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableRobots->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableRobots->setObjectName(QStringLiteral("tableRobots"));
        tableRobots->setFont(font1);
        tableRobots->horizontalHeader()->setCascadingSectionResizes(true);

        verticalLayout_2->addWidget(tableRobots);

        tabsMain->addTab(tabRobots, QString());
        tabCommsIn = new QWidget();
        tabCommsIn->setObjectName(QStringLiteral("tabCommsIn"));
        verticalLayout_3 = new QVBoxLayout(tabCommsIn);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        layoutCommsInBtns = new QHBoxLayout();
        layoutCommsInBtns->setSpacing(6);
        layoutCommsInBtns->setObjectName(QStringLiteral("layoutCommsInBtns"));
        btnCommsInAdd = new QPushButton(tabCommsIn);
        btnCommsInAdd->setObjectName(QStringLiteral("btnCommsInAdd"));
        btnCommsInAdd->setFont(font1);

        layoutCommsInBtns->addWidget(btnCommsInAdd);

        spacerCommsInBtns = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layoutCommsInBtns->addItem(spacerCommsInBtns);

        btnCommsInDelete = new QPushButton(tabCommsIn);
        btnCommsInDelete->setObjectName(QStringLiteral("btnCommsInDelete"));
        btnCommsInDelete->setFont(font1);

        layoutCommsInBtns->addWidget(btnCommsInDelete);


        verticalLayout_3->addLayout(layoutCommsInBtns);

        splitterCommsIn = new QSplitter(tabCommsIn);
        splitterCommsIn->setObjectName(QStringLiteral("splitterCommsIn"));
        splitterCommsIn->setOrientation(Qt::Vertical);
        tableCommsIn = new QTableWidget(splitterCommsIn);
        if (tableCommsIn->columnCount() < 4)
            tableCommsIn->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableCommsIn->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableCommsIn->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableCommsIn->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableCommsIn->setHorizontalHeaderItem(3, __qtablewidgetitem7);
        tableCommsIn->setObjectName(QStringLiteral("tableCommsIn"));
        tableCommsIn->setFont(font1);
        splitterCommsIn->addWidget(tableCommsIn);
        grpbxCommsIn = new QGroupBox(splitterCommsIn);
        grpbxCommsIn->setObjectName(QStringLiteral("grpbxCommsIn"));
        grpbxCommsIn->setFont(font1);
        gridLayout_2 = new QGridLayout(grpbxCommsIn);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        textbrowCommsInMsgs = new QTextBrowser(grpbxCommsIn);
        textbrowCommsInMsgs->setObjectName(QStringLiteral("textbrowCommsInMsgs"));

        gridLayout_2->addWidget(textbrowCommsInMsgs, 0, 0, 1, 1);

        splitterCommsIn->addWidget(grpbxCommsIn);

        verticalLayout_3->addWidget(splitterCommsIn);

        tabsMain->addTab(tabCommsIn, QString());
        tabCommsOut = new QWidget();
        tabCommsOut->setObjectName(QStringLiteral("tabCommsOut"));
        verticalLayout_4 = new QVBoxLayout(tabCommsOut);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        layoutCommsOutBtns = new QHBoxLayout();
        layoutCommsOutBtns->setSpacing(6);
        layoutCommsOutBtns->setObjectName(QStringLiteral("layoutCommsOutBtns"));
        btnCommsOutAdd = new QPushButton(tabCommsOut);
        btnCommsOutAdd->setObjectName(QStringLiteral("btnCommsOutAdd"));
        btnCommsOutAdd->setFont(font1);

        layoutCommsOutBtns->addWidget(btnCommsOutAdd);

        spacerCommsOutBtns = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layoutCommsOutBtns->addItem(spacerCommsOutBtns);

        btnCommsOutDelete = new QPushButton(tabCommsOut);
        btnCommsOutDelete->setObjectName(QStringLiteral("btnCommsOutDelete"));
        btnCommsOutDelete->setFont(font1);

        layoutCommsOutBtns->addWidget(btnCommsOutDelete);


        verticalLayout_4->addLayout(layoutCommsOutBtns);

        splitterCommsOut = new QSplitter(tabCommsOut);
        splitterCommsOut->setObjectName(QStringLiteral("splitterCommsOut"));
        splitterCommsOut->setOrientation(Qt::Vertical);
        tableCommsOut = new QTableWidget(splitterCommsOut);
        if (tableCommsOut->columnCount() < 4)
            tableCommsOut->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableCommsOut->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableCommsOut->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableCommsOut->setHorizontalHeaderItem(2, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableCommsOut->setHorizontalHeaderItem(3, __qtablewidgetitem11);
        tableCommsOut->setObjectName(QStringLiteral("tableCommsOut"));
        tableCommsOut->setFont(font1);
        splitterCommsOut->addWidget(tableCommsOut);
        grpbxCommsOut = new QGroupBox(splitterCommsOut);
        grpbxCommsOut->setObjectName(QStringLiteral("grpbxCommsOut"));
        grpbxCommsOut->setFont(font1);
        gridLayout = new QGridLayout(grpbxCommsOut);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textbrowCommsOutMsgs = new QTextBrowser(grpbxCommsOut);
        textbrowCommsOutMsgs->setObjectName(QStringLiteral("textbrowCommsOutMsgs"));

        gridLayout->addWidget(textbrowCommsOutMsgs, 0, 0, 1, 1);

        splitterCommsOut->addWidget(grpbxCommsOut);

        verticalLayout_4->addWidget(splitterCommsOut);

        tabsMain->addTab(tabCommsOut, QString());

        horizontalLayout->addWidget(tabsMain);

        WindowMain->setCentralWidget(widgetlayoutMain);
        menubarMain = new QMenuBar(WindowMain);
        menubarMain->setObjectName(QStringLiteral("menubarMain"));
        menubarMain->setGeometry(QRect(0, 0, 927, 19));
        menuFile = new QMenu(menubarMain);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuConnect = new QMenu(menubarMain);
        menuConnect->setObjectName(QStringLiteral("menuConnect"));
        WindowMain->setMenuBar(menubarMain);

        menubarMain->addAction(menuFile->menuAction());
        menubarMain->addAction(menuConnect->menuAction());
        menuFile->addAction(actionSave_All_Logs);
        menuFile->addAction(actionFileQuit);
        menuConnect->addAction(actionCommsAdd);

        retranslateUi(WindowMain);
        QObject::connect(actionFileQuit, SIGNAL(triggered()), WindowMain, SLOT(close()));

        tabsMain->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(WindowMain);
    } // setupUi

    void retranslateUi(QMainWindow *WindowMain)
    {
        WindowMain->setWindowTitle(QApplication::translate("WindowMain", "WindowMain", 0));
        actionFileQuit->setText(QApplication::translate("WindowMain", "Quit", 0));
        actionCommsAdd->setText(QApplication::translate("WindowMain", "Add Connection...", 0));
        actionEBugs_Out->setText(QApplication::translate("WindowMain", "Robots (Out)", 0));
        actionSave_All_Logs->setText(QApplication::translate("WindowMain", "Save All Logs...", 0));
        grpbxWorld->setTitle(QApplication::translate("WindowMain", "World", 0));
        grpbxWorldEvents->setTitle(QApplication::translate("WindowMain", "World Events", 0));
        QTableWidgetItem *___qtablewidgetitem = tableRobots->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("WindowMain", "ID", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableRobots->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("WindowMain", "X", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableRobots->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("WindowMain", "Y", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableRobots->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("WindowMain", "State", 0));
        tabsMain->setTabText(tabsMain->indexOf(tabRobots), QApplication::translate("WindowMain", "Robots", 0));
        btnCommsInAdd->setText(QApplication::translate("WindowMain", "Add Connection...", 0));
        btnCommsInDelete->setText(QApplication::translate("WindowMain", "Disconnect Selected", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableCommsIn->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("WindowMain", "Select", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableCommsIn->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("WindowMain", "ID", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableCommsIn->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("WindowMain", "Type", 0));
        QTableWidgetItem *___qtablewidgetitem7 = tableCommsIn->horizontalHeaderItem(3);
        ___qtablewidgetitem7->setText(QApplication::translate("WindowMain", "Latest Activity", 0));
        grpbxCommsIn->setTitle(QApplication::translate("WindowMain", "Incomming Messages", 0));
        tabsMain->setTabText(tabsMain->indexOf(tabCommsIn), QApplication::translate("WindowMain", "Incomming Connections", 0));
        btnCommsOutAdd->setText(QApplication::translate("WindowMain", "Add Connection...", 0));
        btnCommsOutDelete->setText(QApplication::translate("WindowMain", "Disconnect Selected", 0));
        QTableWidgetItem *___qtablewidgetitem8 = tableCommsOut->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QApplication::translate("WindowMain", "Select", 0));
        QTableWidgetItem *___qtablewidgetitem9 = tableCommsOut->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QApplication::translate("WindowMain", "ID", 0));
        QTableWidgetItem *___qtablewidgetitem10 = tableCommsOut->horizontalHeaderItem(2);
        ___qtablewidgetitem10->setText(QApplication::translate("WindowMain", "Type", 0));
        QTableWidgetItem *___qtablewidgetitem11 = tableCommsOut->horizontalHeaderItem(3);
        ___qtablewidgetitem11->setText(QApplication::translate("WindowMain", "Latest Activity", 0));
        grpbxCommsOut->setTitle(QApplication::translate("WindowMain", "Outgoing Messages", 0));
        tabsMain->setTabText(tabsMain->indexOf(tabCommsOut), QApplication::translate("WindowMain", "Outgoing Connections", 0));
        menuFile->setTitle(QApplication::translate("WindowMain", "File", 0));
        menuConnect->setTitle(QApplication::translate("WindowMain", "Connect", 0));
    } // retranslateUi

};

namespace Ui {
    class WindowMain: public Ui_WindowMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWMAIN_H
