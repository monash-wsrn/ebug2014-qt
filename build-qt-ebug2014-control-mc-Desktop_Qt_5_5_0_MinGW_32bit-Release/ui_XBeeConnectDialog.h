/********************************************************************************
** Form generated from reading UI file 'XBeeConnectDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XBEECONNECTDIALOG_H
#define UI_XBEECONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XBeeConnectDialog
{
public:
    QGroupBox *selectBox;
    QGridLayout *gridLayout;
    QLabel *pidLabel;
    QLabel *manufacturerLabel;
    QPushButton *detectPushButton;
    QLabel *vidLabel;
    QLabel *descriptionLabel;
    QLabel *locationLabel;
    QComboBox *serialPortInfoListBox;
    QGroupBox *parametersBox;
    QGridLayout *gridLayout_2;
    QComboBox *parityBox;
    QLabel *baudRateLabel;
    QLabel *flowControlLabel;
    QLabel *stopBitsLabel;
    QComboBox *baudRateBox;
    QComboBox *stopBitsBox;
    QComboBox *flowControlBox;
    QLabel *dataBitsLabel;
    QComboBox *dataBitsBox;
    QLabel *parityLabel;
    QGroupBox *otherSettingsBox;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer;
    QCheckBox *dropPacketCheckBox;
    QLineEdit *txBufferSizeEdit;
    QLabel *dataBitsLabel_3;
    QLabel *dataBitsLabel_2;
    QLabel *baudRateLabel_2;
    QLineEdit *threadSleepEdit;
    QComboBox *threadPriorityBox;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *resetToDefaultsPushButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *XBeeConnectDialog)
    {
        if (XBeeConnectDialog->objectName().isEmpty())
            XBeeConnectDialog->setObjectName(QStringLiteral("XBeeConnectDialog"));
        XBeeConnectDialog->resize(530, 390);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(XBeeConnectDialog->sizePolicy().hasHeightForWidth());
        XBeeConnectDialog->setSizePolicy(sizePolicy);
        XBeeConnectDialog->setMinimumSize(QSize(530, 390));
        XBeeConnectDialog->setMaximumSize(QSize(530, 390));
        selectBox = new QGroupBox(XBeeConnectDialog);
        selectBox->setObjectName(QStringLiteral("selectBox"));
        selectBox->setGeometry(QRect(10, 10, 251, 181));
        gridLayout = new QGridLayout(selectBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pidLabel = new QLabel(selectBox);
        pidLabel->setObjectName(QStringLiteral("pidLabel"));

        gridLayout->addWidget(pidLabel, 5, 0, 1, 2);

        manufacturerLabel = new QLabel(selectBox);
        manufacturerLabel->setObjectName(QStringLiteral("manufacturerLabel"));

        gridLayout->addWidget(manufacturerLabel, 2, 0, 1, 2);

        detectPushButton = new QPushButton(selectBox);
        detectPushButton->setObjectName(QStringLiteral("detectPushButton"));

        gridLayout->addWidget(detectPushButton, 0, 1, 1, 1);

        vidLabel = new QLabel(selectBox);
        vidLabel->setObjectName(QStringLiteral("vidLabel"));

        gridLayout->addWidget(vidLabel, 4, 0, 1, 2);

        descriptionLabel = new QLabel(selectBox);
        descriptionLabel->setObjectName(QStringLiteral("descriptionLabel"));

        gridLayout->addWidget(descriptionLabel, 1, 0, 1, 2);

        locationLabel = new QLabel(selectBox);
        locationLabel->setObjectName(QStringLiteral("locationLabel"));

        gridLayout->addWidget(locationLabel, 3, 0, 1, 2);

        serialPortInfoListBox = new QComboBox(selectBox);
        serialPortInfoListBox->setObjectName(QStringLiteral("serialPortInfoListBox"));

        gridLayout->addWidget(serialPortInfoListBox, 0, 0, 1, 1);

        parametersBox = new QGroupBox(XBeeConnectDialog);
        parametersBox->setObjectName(QStringLiteral("parametersBox"));
        parametersBox->setGeometry(QRect(270, 10, 251, 181));
        gridLayout_2 = new QGridLayout(parametersBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        parityBox = new QComboBox(parametersBox);
        parityBox->setObjectName(QStringLiteral("parityBox"));

        gridLayout_2->addWidget(parityBox, 2, 1, 1, 1);

        baudRateLabel = new QLabel(parametersBox);
        baudRateLabel->setObjectName(QStringLiteral("baudRateLabel"));

        gridLayout_2->addWidget(baudRateLabel, 0, 0, 1, 1);

        flowControlLabel = new QLabel(parametersBox);
        flowControlLabel->setObjectName(QStringLiteral("flowControlLabel"));

        gridLayout_2->addWidget(flowControlLabel, 4, 0, 1, 1);

        stopBitsLabel = new QLabel(parametersBox);
        stopBitsLabel->setObjectName(QStringLiteral("stopBitsLabel"));

        gridLayout_2->addWidget(stopBitsLabel, 3, 0, 1, 1);

        baudRateBox = new QComboBox(parametersBox);
        baudRateBox->setObjectName(QStringLiteral("baudRateBox"));

        gridLayout_2->addWidget(baudRateBox, 0, 1, 1, 1);

        stopBitsBox = new QComboBox(parametersBox);
        stopBitsBox->setObjectName(QStringLiteral("stopBitsBox"));

        gridLayout_2->addWidget(stopBitsBox, 3, 1, 1, 1);

        flowControlBox = new QComboBox(parametersBox);
        flowControlBox->setObjectName(QStringLiteral("flowControlBox"));

        gridLayout_2->addWidget(flowControlBox, 4, 1, 1, 1);

        dataBitsLabel = new QLabel(parametersBox);
        dataBitsLabel->setObjectName(QStringLiteral("dataBitsLabel"));

        gridLayout_2->addWidget(dataBitsLabel, 1, 0, 1, 1);

        dataBitsBox = new QComboBox(parametersBox);
        dataBitsBox->setObjectName(QStringLiteral("dataBitsBox"));

        gridLayout_2->addWidget(dataBitsBox, 1, 1, 1, 1);

        parityLabel = new QLabel(parametersBox);
        parityLabel->setObjectName(QStringLiteral("parityLabel"));

        gridLayout_2->addWidget(parityLabel, 2, 0, 1, 1);

        otherSettingsBox = new QGroupBox(XBeeConnectDialog);
        otherSettingsBox->setObjectName(QStringLiteral("otherSettingsBox"));
        otherSettingsBox->setGeometry(QRect(10, 200, 511, 151));
        gridLayout_3 = new QGridLayout(otherSettingsBox);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 2, 1, 1);

        dropPacketCheckBox = new QCheckBox(otherSettingsBox);
        dropPacketCheckBox->setObjectName(QStringLiteral("dropPacketCheckBox"));

        gridLayout_3->addWidget(dropPacketCheckBox, 4, 0, 1, 2);

        txBufferSizeEdit = new QLineEdit(otherSettingsBox);
        txBufferSizeEdit->setObjectName(QStringLiteral("txBufferSizeEdit"));

        gridLayout_3->addWidget(txBufferSizeEdit, 3, 1, 1, 1);

        dataBitsLabel_3 = new QLabel(otherSettingsBox);
        dataBitsLabel_3->setObjectName(QStringLiteral("dataBitsLabel_3"));

        gridLayout_3->addWidget(dataBitsLabel_3, 0, 0, 1, 1);

        dataBitsLabel_2 = new QLabel(otherSettingsBox);
        dataBitsLabel_2->setObjectName(QStringLiteral("dataBitsLabel_2"));

        gridLayout_3->addWidget(dataBitsLabel_2, 3, 0, 1, 1);

        baudRateLabel_2 = new QLabel(otherSettingsBox);
        baudRateLabel_2->setObjectName(QStringLiteral("baudRateLabel_2"));

        gridLayout_3->addWidget(baudRateLabel_2, 2, 0, 1, 1);

        threadSleepEdit = new QLineEdit(otherSettingsBox);
        threadSleepEdit->setObjectName(QStringLiteral("threadSleepEdit"));

        gridLayout_3->addWidget(threadSleepEdit, 0, 1, 1, 1);

        threadPriorityBox = new QComboBox(otherSettingsBox);
        threadPriorityBox->setObjectName(QStringLiteral("threadPriorityBox"));

        gridLayout_3->addWidget(threadPriorityBox, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 2, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 3, 2, 1, 1);

        horizontalLayoutWidget = new QWidget(XBeeConnectDialog);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 350, 511, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        resetToDefaultsPushButton = new QPushButton(horizontalLayoutWidget);
        resetToDefaultsPushButton->setObjectName(QStringLiteral("resetToDefaultsPushButton"));

        horizontalLayout->addWidget(resetToDefaultsPushButton);

        buttonBox = new QDialogButtonBox(horizontalLayoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(serialPortInfoListBox, detectPushButton);
        QWidget::setTabOrder(detectPushButton, baudRateBox);
        QWidget::setTabOrder(baudRateBox, dataBitsBox);
        QWidget::setTabOrder(dataBitsBox, parityBox);
        QWidget::setTabOrder(parityBox, stopBitsBox);
        QWidget::setTabOrder(stopBitsBox, flowControlBox);
        QWidget::setTabOrder(flowControlBox, threadSleepEdit);
        QWidget::setTabOrder(threadSleepEdit, threadPriorityBox);
        QWidget::setTabOrder(threadPriorityBox, txBufferSizeEdit);
        QWidget::setTabOrder(txBufferSizeEdit, dropPacketCheckBox);
        QWidget::setTabOrder(dropPacketCheckBox, buttonBox);
        QWidget::setTabOrder(buttonBox, resetToDefaultsPushButton);

        retranslateUi(XBeeConnectDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), XBeeConnectDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), XBeeConnectDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(XBeeConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *XBeeConnectDialog)
    {
        XBeeConnectDialog->setWindowTitle(QApplication::translate("XBeeConnectDialog", "Connect to XBee", 0));
        selectBox->setTitle(QApplication::translate("XBeeConnectDialog", "Serial Port:", 0));
        pidLabel->setText(QApplication::translate("XBeeConnectDialog", "Product ID:", 0));
        manufacturerLabel->setText(QApplication::translate("XBeeConnectDialog", "Manufacturer:", 0));
        detectPushButton->setText(QApplication::translate("XBeeConnectDialog", "Detect", 0));
        vidLabel->setText(QApplication::translate("XBeeConnectDialog", "Vendor ID:", 0));
        descriptionLabel->setText(QApplication::translate("XBeeConnectDialog", "Description:", 0));
        locationLabel->setText(QApplication::translate("XBeeConnectDialog", "Location:", 0));
        parametersBox->setTitle(QApplication::translate("XBeeConnectDialog", "Parameters:", 0));
        baudRateLabel->setText(QApplication::translate("XBeeConnectDialog", "Baud Rate:", 0));
        flowControlLabel->setText(QApplication::translate("XBeeConnectDialog", "Flow Control:", 0));
        stopBitsLabel->setText(QApplication::translate("XBeeConnectDialog", "Stop Bits:", 0));
        dataBitsLabel->setText(QApplication::translate("XBeeConnectDialog", "Data Bits:", 0));
        parityLabel->setText(QApplication::translate("XBeeConnectDialog", "Parity:", 0));
        otherSettingsBox->setTitle(QApplication::translate("XBeeConnectDialog", "Other Settings:", 0));
        dropPacketCheckBox->setText(QApplication::translate("XBeeConnectDialog", "Drop packet if buffer is full", 0));
        dataBitsLabel_3->setText(QApplication::translate("XBeeConnectDialog", "Thread Sleep (ms):", 0));
        dataBitsLabel_2->setText(QApplication::translate("XBeeConnectDialog", "TX Buffer Size:", 0));
        baudRateLabel_2->setText(QApplication::translate("XBeeConnectDialog", "Thread Priority:", 0));
        resetToDefaultsPushButton->setText(QApplication::translate("XBeeConnectDialog", "Reset to Defaults", 0));
    } // retranslateUi

};

namespace Ui {
    class XBeeConnectDialog: public Ui_XBeeConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XBEECONNECTDIALOG_H
