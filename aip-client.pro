#-------------------------------------------------
#
# Project created by QtCreator 2017-10-27T08:58:38
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aip-client
TEMPLATE = app

HEADERS += \
    app/action.h \
    app/global.h \
    app/logger.h \
    app/lqcombobox.h \
    app/lqdoublespinbox.h \
    app/lqmessagebox.h \
    app/lqreadonlyitem.h \
    app/lqsnowflake.h \
    app/lqspinbox.h \
    app/lqsqltablemodel.h \
    app/lqstandarditem.h \
    app/master.h \
    app/signin.h \
    app/source.h \
    app/sqlite.h \
    app/system.h \
    app/tcpclient.h \
    app/tcpsocket.h \
    app/online.h \
    app/device.h

SOURCES += \
    app/action.cpp \
    app/global.cpp \
    app/logger.cpp \
    app/lqcombobox.cpp \
    app/lqdoublespinbox.cpp \
    app/lqmessagebox.cpp \
    app/lqreadonlyitem.cpp \
    app/lqsnowflake.cpp \
    app/lqspinbox.cpp \
    app/lqsqltablemodel.cpp \
    app/lqstandarditem.cpp \
    app/main.cpp \
    app/master.cpp \
    app/signin.cpp \
    app/source.cpp \
    app/sqlite.cpp \
    app/system.cpp \
    app/tcpclient.cpp \
    app/tcpsocket.cpp \
    app/online.cpp \
    app/device.cpp

RC_FILE += qrc/ico.rc

