#-------------------------------------------------
#
# Project created by QtCreator 2015-07-24T16:57:07
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestNordicReceiver
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x \
            -Wall -H

SOURCES += main.cpp\
        mainwindow.cpp \
    commsin.cpp \
    messagetranslator.cpp

HEADERS  += mainwindow.h \
    ThreadableQObject.h \
    commsin.h \
    messagetranslator.h

FORMS    += mainwindow.ui
