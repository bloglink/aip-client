/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统界面
*******************************************************************************/
#ifndef PERMISSION_H
#define PERMISSION_H

#include <QFile>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QStackedWidget>
#include <QApplication>
#include <QMessageBox>

#include <QSqlQuery>
#include <QSqlDatabase>

#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>

#include "online.h"
#include "device.h"
#include "master.h"
#include "action.h"
#include "source.h"
#include "logger.h"

class System : public QWidget
{
    Q_OBJECT

public:
    explicit System(QWidget *parent = 0);
    ~System();
signals:
    void sendAppShow(QString win);
    void sendSqlOpen(QString name);
    void sendNetMsg(QJsonObject dat);
    void sendDisplay(QString msg);
public slots:
    void setMasterName(QString name);
private slots:
    void initUI();
    void initLayout();
    void initOnline();
    void initDevice();
    void initMaster();
    void initSource();
    void initAction();
    void initLogger();
    void clickButton();
    double getAction(QString masterName, QString sourceName);
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
private:
    QString masterName;
    QString sqlName;
    QSqlDatabase db;
    QStackedWidget *stack;
    QVBoxLayout *btnLayout;
    QList<QPushButton*> buttons;

    Online *online;
    Device *device;
    Master *master;
    Action *action;
    Source *source;
    Logger *logger;

    LQSqlTableModel *mMaster;
    LQSqlTableModel *mSource;
    LQSqlTableModel *mAction;
};

#endif // PERMISSION_H
