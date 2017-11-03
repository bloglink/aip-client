/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       二代网络客户端
*******************************************************************************/
#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QFile>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QStackedWidget>
#include <QApplication>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonDocument>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

#include "lqmessagebox.h"

#include "signin.h"
#include "system.h"

#include "tcpsocket.h"

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();
signals:
    void sendAppShow(QString win);
    void sendSqlOpen(QString name);
    void sendNetMsg(QJsonObject dat);
    void sendAppCmd(QJsonObject dat);
    void sendDisplay(QString msg);
private slots:
    void initUI();
    void initSkin();
    void initPopup();
    void initLayout();
    void initSignIn();
    void initSystem();
    void initSocket();
    void initVersion();
    void initSqlite(QByteArray msg);

    void recvAppShow(QString sourceName);
    void recvAppMsg(QJsonObject msg);
    void recvNetMsg(quint16 addr, quint16 cmd, QByteArray msg);
private:
    LQMessageBox *popup;
    QStackedWidget *stack;
    SignIn *signin;
    System *system;

    TcpSocket *tcp;
    QSqlDatabase db;
};

#endif // TCPCLIENT_H
