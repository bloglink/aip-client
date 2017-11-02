/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       权限管理界面
*******************************************************************************/
#ifndef ACTION_H
#define ACTION_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QInputDialog>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

#include "lqsnowflake.h"
#include "lqreadonlyitem.h"
#include "lqsqltablemodel.h"

class Action : public QWidget
{
    Q_OBJECT
public:
    explicit Action(QWidget *parent = 0);

private slots:
    void initUI();
    void initView();
    void initLayout();
    void createSqlite();
    void updateSqlite();
    void updateSource(QModelIndex index);
    void changeAction(QModelIndex index);
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
private:
    QHBoxLayout *btnLayout;

    QString sqlName;
    QTableView *master;
    LQSqlTableModel *mMaster;
    QTableView *source;
    LQSqlTableModel *mSource;
    LQSqlTableModel *mAction;
    LQSnowflake snow;
};

#endif // ACTION_H
