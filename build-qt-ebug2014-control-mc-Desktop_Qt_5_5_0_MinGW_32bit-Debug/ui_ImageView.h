/********************************************************************************
** Form generated from reading UI file 'ImageView.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEW_H
#define UI_IMAGEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageView
{
public:
    QLabel *label;

    void setupUi(QWidget *ImageView)
    {
        if (ImageView->objectName().isEmpty())
            ImageView->setObjectName(QStringLiteral("ImageView"));
        ImageView->resize(640, 480);
        label = new QLabel(ImageView);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, -6, 641, 491));

        retranslateUi(ImageView);

        QMetaObject::connectSlotsByName(ImageView);
    } // setupUi

    void retranslateUi(QWidget *ImageView)
    {
        ImageView->setWindowTitle(QApplication::translate("ImageView", "Form", 0));
        label->setText(QApplication::translate("ImageView", "QImage", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageView: public Ui_ImageView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEW_H
