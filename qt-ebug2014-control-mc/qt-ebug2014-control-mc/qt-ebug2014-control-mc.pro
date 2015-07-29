QT += core gui \
    serialport

#TODO Changed by MatthewBoys
#QT_CONFIG -= no-pkg-config
#CONFIG += link_pkgconfig
#PKGCONFIG += opencv

LIBS += -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui
#TODO Changed by MatthewBoys


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-ebug-control-multicamera
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x \
            -Wall -H

DEFINES += APP_VERSION=\\\"1.0.0\\\" \
    LOG_OUTPUT_DEST_STDERR \
    LOG_OUTPUT_DEST_FILE

SOURCES += main.cpp \
    MainWindow.cpp \
    MatToQImage.cpp \
    FrameLabel.cpp \
    CameraView.cpp \
    ProcessingThread.cpp \
    CaptureThread.cpp \
    CameraConnectDialog.cpp \
    ImageProcessingSettingsDialog.cpp \
    SharedImageBuffer.cpp \
    ebug-api-cpp/eBugAPI.cpp \
    XBeeThread.cpp \
    XBeeConnectDialog.cpp \
    ControlThread.cpp   \
    control-algorithms/FormationSelection.cpp \
    control-algorithms/Demo.cpp \
    RenderThread.cpp \
    ImageView.cpp \
    PIDController.cpp \
    debugMessageHandler.cpp

HEADERS  += \
    MainWindow.h \
    Config.h \
    MatToQImage.h \
    FrameLabel.h \
    Structures.h \
    CameraView.h \
    ProcessingThread.h \
    CaptureThread.h \
    CameraConnectDialog.h \
    ImageProcessingSettingsDialog.h \
    SharedImageBuffer.h \
    ebug-api-cpp/eBugAPI.h \
    XBeeThread.h \
    XBeeConnectDialog.h \
    Buffer.h \
    ControlThread.h     \
    control-algorithms/ControlAlgorithms.h \
    control-algorithms/FormationSelection.h \
    control-algorithms/Demo.h \
    EBugData.h \
    utils.h \
    LedDetection.h \
    RenderThread.h \
    ImageView.h \
    PIDController.h \
    Eigen/Dense

FORMS    += \
    MainWindow.ui \
    CameraView.ui \
    CameraConnectDialog.ui \
    ImageProcessingSettingsDialog.ui \
    XBeeConnectDialog.ui \
    ImageView.ui
