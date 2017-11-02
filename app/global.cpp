/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       全局设置
*******************************************************************************/
#include "global.h"

Global::Global(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Global::initUI()
{
    initView();
    initButton();
    initLayout();
}

void Global::initView()
{
    view = new QTableView(this);
    view->setItemDelegateForColumn(1, new LQReadOnlyItem);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Global::initButton()
{
    btnLayout = new QHBoxLayout;
    btnLayout->addStretch();

//    QPushButton *append = new QPushButton(tr("添加用户"), this);
//    btnLayout->addWidget(append);
//    append->setFixedSize(97, 44);
//    connect(append, SIGNAL(clicked(bool)), this, SLOT(appendSqlite()));

//    QPushButton *remove = new QPushButton(tr("删除用户"), this);
//    btnLayout->addWidget(remove);
//    remove->setFixedSize(97, 44);
//    connect(remove, SIGNAL(clicked(bool)), this, SLOT(deleteSqlite()));

    QPushButton *update = new QPushButton(tr("保存修改"), this);
    btnLayout->addWidget(update);
    update->setFixedSize(97, 44);
    connect(update, SIGNAL(clicked(bool)), this, SLOT(changeSqlite()));
}

void Global::initLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    this->setLayout(layout);
}

void Global::createSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.exec("drop table global");
    QString cmd;
    cmd = "create table if not exists global (";
    cmd += "id integer primary key, explanation text, parameter text, name text)";
    if (!query.exec(cmd)) {
        qDebug() << "create table global Error";
    }
}

void Global::updateSqlite()
{
    mView = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mView->setTable("global");
    mView->select();
    if (mView->rowCount() == 0) {
        insertSqlite();
        mView->select();
    }
    view->setModel(mView);
    view->hideColumn(0);
    view->hideColumn(3);
}

void Global::insertSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    QStringList names;
    QStringList explanations;
    QStringList parameters;

    names << "timeOK";
    parameters << "0.2";
    explanations << tr("合格报警时间");

    names << "timeNG";
    parameters << "0.5";
    explanations << tr("不合格报警时间");

    names << "barcodeOrigin";
    parameters << "0";
    explanations << tr("条码起始位");

    names << "barcodeLength";
    parameters << "9";
    explanations << tr("条码长度");

    names << "testMode";
    parameters << "3";
    explanations << tr("测试模式");

    for (int i=0; i < names.size(); i++) {
        query.prepare("insert into global values(?,?,?,?)");
        query.bindValue(0, snow.getId());
        query.bindValue(1, explanations.at(i));
        query.bindValue(2, parameters.at(i));
        query.bindValue(3, names.at(i));
        query.exec();
    }
}

void Global::appendSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("insert into global values(?,?,?)");
    query.bindValue(0, snow.getId());
    query.bindValue(1, getText(tr("用户"), tr("请输入新的用户名:"), "name"));
    query.bindValue(2, getText(tr("用户"), tr("请输入新的密码:"), "pass"));
    query.exec();

    mView->select();
}

void Global::deleteSqlite()
{
    int row = view->currentIndex().row();
    if (row >= 0) {
        double id = mView->index(row, 0).data().toDouble();
        QSqlQuery query(QSqlDatabase::database(sqlName));
        query.prepare("delete from global where id = :id");
        query.bindValue(":id", id);
        query.exec();

        query.prepare("delete from action where Global = :id");
        query.bindValue(":id", id);
        query.exec();
    } else {
        QMessageBox::warning(this, tr("警告"), tr("请选择一项数据"), QMessageBox::Ok);
    }
    mView->select();
}

void Global::changeSqlite()
{
    int row = view->currentIndex().row();
    int column = view->currentIndex().column();
    if (row >= 0) {
        double id = mView->index(row, 0).data().toDouble();
        QString name = mView->index(row, 1).data().toString();
        QString pass = mView->index(row, 2).data().toString();
        switch (column) {
        case 0:
            break;
        case 1:
            name = getText(tr("用户"), tr("请输入新的用户名:"), name);
            break;
        case 2:
            pass = getText(tr("密码"), tr("请输入新的密码:"), pass);
            break;
        default:
            break;
        }
        QSqlQuery query(QSqlDatabase::database(sqlName));
        query.prepare("replace into Global values(?,?,?)");
        query.bindValue(0, id);
        query.bindValue(1, name);
        query.bindValue(2, pass);
        query.exec();
    } else {
        QMessageBox::warning(this, tr("警告"), tr("请选择一项数据"), QMessageBox::Ok);
    }
    mView->select();
}

QString Global::getText(QString title, QString text, QString s)
{
    bool ok;
    QString input = QInputDialog::getText(this, title, text, QLineEdit::Normal, s, &ok);
    if (ok)
        return input;
    else
        return s;
}

void Global::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        mView->select();
    }
}

void Global::recvSqlOpen(QString sql)
{
    sqlName = sql;
    createSqlite();
    updateSqlite();
}
