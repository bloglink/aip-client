/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       用户管理界面
*******************************************************************************/
#ifndef MASTER_H
#define MASTER_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

#include "lqsnowflake.h"
#include "lqreadonlyitem.h"
#include "lqsqltablemodel.h"

class Master : public QWidget
{
    Q_OBJECT
public:
    explicit Master(QWidget *parent = 0);

private slots:
    void initUI();
    void initView();
    void initButton();
    void initLayout();
    void createSqlite();
    void updateSqlite();
    void insertSqlite();
    void appendSqlte();
    void deleteSqlite();
    void changeSqlite();
    bool isNameExists(QString name);
    QString getText(QString title, QString text, QString s);
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
private:
    QHBoxLayout *btnLayout;

    QString sqlName;
    LQSnowflake snow;
    QTableView *view;
    LQSqlTableModel *mView;
};

#endif // MASTER_H
