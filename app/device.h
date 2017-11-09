/*******************************************************************************
 * Copyright [2017]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       设备管理
*******************************************************************************/
#ifndef DEVICE_H
#define DEVICE_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QFileSystemModel>
#include <QStandardItem>

#include <QLabel>
#include <QSplitter>
#include <QPushButton>
#include <QGroupBox>

#include "lqsqltablemodel.h"
#include "lqstandarditem.h"
#include "lqreadonlyitem.h"
#include "lqheadview.h"

class Device : public QWidget
{
    Q_OBJECT
public:
    explicit Device(QWidget *parent = 0);

private slots:
    void initUI();
    void initView();
    void initDevice(QSqlDatabase db);
    void initRepair(QSqlDatabase db);
    void initRecord(QSqlDatabase db);
    void clickDevice(QModelIndex index);
    void initCtrl();
    void saveCtrl();
    void updateSqlite();
    void updateFilter(int col, QString msg);
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
private:
    QTableView *deviceView;
    QSqlTableModel *mDeviceView;
    QMap<int, QString> filters;

    QTableView *repairView;
    QSqlTableModel *mRepairView;

    QTableView *recordView;
    QSqlTableModel *mRecordView;

    QWidget *deviceCtrl;

    QString sqlName;
};

#endif // DEVICE_H
