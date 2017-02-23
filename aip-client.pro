#-------------------------------------------------
#
# Project created by QtCreator 2016-08-27T09:45:45
#
#-------------------------------------------------

QT       += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aip-client
TEMPLATE = app


SOURCES += main.cpp\
        CWinClient.cpp \
    MyTcpThread.cpp \
    CTcpNetwork.cpp \
    CTcpClient.cpp

HEADERS  += CWinClient.h \
    MyTcpThread.h \
    CTcpNetwork.h \
    CTcpClient.h

FORMS    += CWinClient.ui

RESOURCES += \
    image.qrc

RC_FILE += ico.rc
