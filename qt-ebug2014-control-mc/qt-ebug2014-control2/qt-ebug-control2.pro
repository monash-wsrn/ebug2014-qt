#-------------------------------------------------
#
# Project created by QtCreator 2015-07-17T11:56:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-ebug-control2
TEMPLATE = app


SOURCES += main.cpp\
        debugMessageHandler.cpp \
    windowMain.cpp \
    threadWorld.cpp \
    dialogCommsInAdd.cpp \
    CommsInDevices/commsInDeviceText.cpp

HEADERS  += \
    windowMain.h \
    threadWorld.h \
    ThreadableQObject.h \
    CommsInDevices/CommsIn.h \
    dataRobotLocation.h \
    dialogCommsInAdd.h \
    CommsInDevices/commsInDeviceText.h

FORMS    += \
    windowMain.ui \
    dialogCommsInAdd.ui \
    CommsInDevices/commsInDeviceText.ui
