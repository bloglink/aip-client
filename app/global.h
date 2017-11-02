/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       全局设置
*******************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>

#include <QSqlQuery>
#include <QSqlDatabase>

#include "lqsnowflake.h"
#include "lqreadonlyitem.h"
#include "lqsqltablemodel.h"

class Global : public QWidget
{
    Q_OBJECT
public:
    explicit Global(QWidget *parent = 0);

private slots:
    void initUI();
    void initView();
    void initButton();
    void initLayout();
    void createSqlite();
    void updateSqlite();
    void insertSqlite();
    void appendSqlite();
    void deleteSqlite();
    void changeSqlite();
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

#endif // GLOBAL_H
