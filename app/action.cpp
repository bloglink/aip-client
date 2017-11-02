/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       权限管理界面
*******************************************************************************/
#include "action.h"

Action::Action(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Action::initUI()
{
    initView();
    initLayout();
}

void Action::initView()
{
    master = new QTableView(this);
    master->setItemDelegate(new LQReadOnlyItem(this));
    master->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(master, SIGNAL(clicked(QModelIndex)), this, SLOT(updateSource(QModelIndex)));

    source = new QTableView(this);
    source->setItemDelegate(new LQReadOnlyItem(this));
    source->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(source, SIGNAL(clicked(QModelIndex)), this, SLOT(changeAction(QModelIndex)));
}

void Action::initLayout()
{
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(master, 0, 0);
    layout->addWidget(source, 0, 1);

    this->setLayout(layout);
}

void Action::createSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    //    query.exec("drop table aip_action");
    QString cmd;
    cmd = "create table if not exists aip_action (";
    cmd += "id bigint primary key, master_id bigint, source_id bigint, action bigint)";
    if (!query.exec(cmd)) {
        qDebug() << "create table aip_action Error";
    }
}

void Action::updateSqlite()
{
    mAction = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mAction->setTable("aip_action");
    mAction->select();

    mMaster = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mMaster->setTable("aip_master");
    mMaster->select();
    master->setModel(mMaster);
    if (mMaster->columnCount() >= 3) {
        master->hideColumn(0);
        master->hideColumn(2);
        QStringList headers;
        headers << "ID" << "用户名" << "密码";
        for (int i=0; i < headers.size(); i++) {
            mMaster->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
    }
    //    if (mMaster->rowCount() > 0) {
    //        master->hideRow(0);  // 隐藏超级用户
    //    }

    mSource = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mSource->setTable("aip_source");
    mSource->select();
    mSource->setCheckable(1);
    source->setModel(mSource);
    if (mSource->columnCount() >= 3) {
        source->hideColumn(0);
        QStringList headers;
        headers << "ID" << "资源名" << "资源解析";
        for (int i=0; i < headers.size(); i++) {
            mSource->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
    }
}

void Action::updateSource(QModelIndex index)
{
    int row = index.row();
    QString masterId = mMaster->index(row, 0).data().toString();
    mAction->setFilter(QObject::tr("master_id='%1'").arg(masterId));
    mAction->select();

    QStringList sourceIds;
    QStringList actions;
    for (int i=0; i < mAction->rowCount(); i++) {
        sourceIds.append(mAction->index(i, 2).data().toString());
        actions.append(mAction->index(i, 3).data().toString());
    }

    for (int i=0; i < mSource->rowCount(); i++) {
        QString sourceId = mSource->index(i, 0).data().toString();
        QModelIndex indexSource = mSource->index(i, 1);
        if (sourceIds.contains(sourceId)) {
            int row = sourceIds.indexOf(sourceId);
            mSource->setData(indexSource, actions.at(row), Qt::CheckStateRole);
        } else {
            mSource->setData(indexSource, Qt::Unchecked, Qt::CheckStateRole);
        }
    }
    source->update();
}

void Action::changeAction(QModelIndex index)
{
    int row =  master->currentIndex().row();
    double masterId = mMaster->index(row, 0).data().toDouble();
    double sourceId = mSource->index(index.row(), 0).data().toDouble();
    double actionVal = mSource->data(index, Qt::CheckStateRole).toDouble();
    double actionId = snow.getId();

    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select id from aip_action where master_id=? and source_id=?");
    query.bindValue(0, masterId);
    query.bindValue(1, sourceId);
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
    if (query.next()) {
        actionId = query.value(0).toDouble();
    }

    query.prepare("replace into aip_action values(?,?,?,?)");
    query.addBindValue(actionId);
    query.addBindValue(masterId);
    query.addBindValue(sourceId);
    query.addBindValue(actionVal);
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
}

void Action::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        mAction->select();
        mMaster->select();
        mSource->select();
    }
}

void Action::recvSqlOpen(QString sql)
{
    sqlName = sql;
    createSqlite();
    updateSqlite();
}
