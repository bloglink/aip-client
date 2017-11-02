/*******************************************************************************
 * Copyright [2017]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       在线设备
*******************************************************************************/
#ifndef ONLINE_H
#define ONLINE_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QLineEdit>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QFileSystemModel>
#include <QStandardItem>
#include <QTextBrowser>
#include <QTreeWidgetItem>

#include <QLabel>
#include <QSplitter>
#include <QPushButton>
#include <QGroupBox>

#include <QJsonObject>
#include <QJsonDocument>

#include "lqsqltablemodel.h"
#include "lqstandarditem.h"
#include "lqreadonlyitem.h"
#define ONLINE_REC_UUID 0x00
#define ONLINE_DEV_UUID 0x01
#define ONLINE_DEV_ADDR 0x02
#define ONLINE_DEV_NUMB 0x03
#define ONLINE_NET_PORT 0x04
#define ONLINE_NET_ADDR 0x05
#define ONLINE_REC_TIME 0x06
#define ONLINE_NET_STAT 0x07
#define ONLINE_VER_NUMB 0x08

#define ONLINE_SHELL_CMD 101
#define ONLINE_SEND_HAED 102
#define ONLINE_READ_HAED 103

class Online : public QWidget
{
    Q_OBJECT
public:
    explicit Online(QWidget *parent = 0);
signals:
    void sendNetMsg(QJsonObject dat);
private slots:
    void initUI();
    void initView();
    void initTree();
    void initFile();
    void initText();
    void initCtrl();
    void initLayout();
    void clickTree(QModelIndex index);
    void clickFile(QModelIndex index);
    void clickOnline(QModelIndex index);
    void recvFileHead();
    void sendShellCmd();
    void sendFileHead();
    void updateSqlite();
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
    void recvDisplay(QString msg);
private:
    QTableView *onlineView;
    LQSqlTableModel *mOnlineView;
    LQSqlTableModel *mDeviceView;

    QTreeWidget *onlineTree;

    QTreeView *onlineFile;
    QFileSystemModel *mOnlineFile;

    QTableView *onlineText;
    LQStandardItem *mOnlineText;

    QGroupBox *onlineCtrl;
    QLineEdit *sendFile;
    QLabel *currentRate;
    QLineEdit *shellCmd;
    QTextBrowser *text;
    QLineEdit *recvFile;

    QString sqlName;
    QString currentPort;
    QString currentPath;
};

#endif // ONLINE_H
