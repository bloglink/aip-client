#-------------------------------------------------
#
# Project created by QtCreator 2017-03-07T16:45:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpClient
TEMPLATE = app


SOURCES += main.cpp\
        WinClient.cpp \
    MyTcpThread.cpp

HEADERS  += WinClient.h \
    MyTcpThread.h

FORMS    += WinClient.ui

RESOURCES += \
    image.qrc
