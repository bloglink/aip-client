/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据管理
*******************************************************************************/
#ifndef SQLITE_H
#define SQLITE_H

#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QDateEdit>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

#include <QTextCodec>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

#include "lqsnowflake.h"
#include "lqreadonlyitem.h"
#include "lqsqltablemodel.h"

class Sqlite : public QWidget
{
    Q_OBJECT
public:
    explicit Sqlite(QWidget *parent = 0);

signals:
    void sendAppShow(QString win);
private slots:
    void initUI();
    void initView();
    void initButton();
    void initLayout();
    void createSqlite();
    void updateSqlite();
    void updateSqliteByDate();
    void exportSqlite();
    void exportSqliteByDate();
    void deleteSqlite();
    void updateViews(QModelIndex index);
    void exist();
    QByteArray ToGbk(const QString &inStr);
    QString getSaveFileName();
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);
private:
    QHBoxLayout *btnLayout;
    QDateEdit *dateEdit;

    QString sqlName;
    QTableView *view;
    LQSqlTableModel *mView;
    QTableView *views;
    LQSqlTableModel *mViews;

    double current_id;
};

#endif // SQLITE_H
