/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       用户管理界面
*******************************************************************************/
#include "master.h"

Master::Master(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Master::initUI()
{
    initView();
    initButton();
    initLayout();
}

void Master::initView()
{
    view = new QTableView(this);
    view->setItemDelegate(new LQReadOnlyItem(this));
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Master::initButton()
{
    btnLayout = new QHBoxLayout;
    btnLayout->addStretch();

    QPushButton *append = new QPushButton(tr("添加用户"), this);
    btnLayout->addWidget(append);
    append->setFixedSize(97, 44);
    connect(append, SIGNAL(clicked(bool)), this, SLOT(appendSqlte()));

    QPushButton *remove = new QPushButton(tr("删除用户"), this);
    btnLayout->addWidget(remove);
    remove->setFixedSize(97, 44);
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(deleteSqlite()));

    QPushButton *update = new QPushButton(tr("修改用户"), this);
    btnLayout->addWidget(update);
    update->setFixedSize(97, 44);
    connect(update, SIGNAL(clicked(bool)), this, SLOT(changeSqlite()));
}

void Master::initLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addLayout(btnLayout);
    this->setLayout(layout);
}

void Master::createSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    //    query.exec("drop table aip_master");
    QString cmd;
    cmd = "create table if not exists aip_master (";
    cmd += "id bigint primary key, name text, password text)";
    if (!query.exec(cmd)) {
        qDebug() << "create table aip_master Error";
    }
}

void Master::updateSqlite()
{
    mView = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mView->setTable("aip_master");
    mView->select();
    if (mView->rowCount() == 0) {
        insertSqlite();
        mView->select();
    }
    view->setModel(mView);
    if (mView->columnCount() >= 3) {
        view->hideColumn(0);
        QStringList headers;
        headers << "ID" << "用户名" << "密码";
        for (int i=0; i < headers.size(); i++) {
            mView->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
    }
    //    if (mView->rowCount() > 0) {
    //        view->hideRow(0);  // 隐藏超级用户
    //    }
}

void Master::insertSqlite()
{
    QStringList name;
    QStringList pass;
    name << "supervisor";
    pass << tr("aip9918");
    name << "admin";
    pass << tr("");
    name << "guest";
    pass << tr("");
    QSqlQuery query(QSqlDatabase::database(sqlName));
    for (int i=0; i < name.size(); i++) {
        query.prepare("insert into aip_master values(?,?,?)");
        query.bindValue(0, snow.getId());
        query.bindValue(1, name.at(i));
        query.bindValue(2, pass.at(i));
        query.exec();
    }
}

void Master::appendSqlte()
{
    QString name = getText(tr("用户"), tr("请输入新的用户名:"), "name");
    if (isNameExists(name)) {
        QMessageBox::warning(this, "用户", tr("用户已存在"), QMessageBox::Ok);
    } else {
        QSqlQuery query(QSqlDatabase::database(sqlName));
        query.prepare("insert into aip_master values(?,?,?)");
        query.bindValue(0, snow.getId());
        query.bindValue(1, name);
        query.bindValue(2, getText(tr("用户"), tr("请输入新的密码:"), "pass"));
        query.exec();
        mView->select();
    }
}

void Master::deleteSqlite()
{
    int row = view->currentIndex().row();
    if (row >= 0) {
        double id = mView->index(row, 0).data().toDouble();
        QSqlQuery query(QSqlDatabase::database(sqlName));
        query.prepare("delete from aip_master where id=:id");
        query.bindValue(":id", id);
        query.exec();

        query.prepare("delete from aip_action where master_id=:id");
        query.bindValue(":id", id);
        query.exec();
    } else {
        QMessageBox::warning(this, tr("警告"), tr("请选择一项数据"), QMessageBox::Ok);
    }
    mView->select();
}

void Master::changeSqlite()
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
        if ((column == 1) && isNameExists(name)) {
            QMessageBox::warning(this, "", tr("用户已存在"), QMessageBox::Ok);
        } else {
            QSqlQuery query(QSqlDatabase::database(sqlName));
            query.prepare("replace into aip_master values(?,?,?)");
            query.bindValue(0, id);
            query.bindValue(1, name);
            query.bindValue(2, pass);
            query.exec();
        }
    } else {
        QMessageBox::warning(this, tr("警告"), tr("请选择一项数据"), QMessageBox::Ok);
    }
    mView->select();
}

bool Master::isNameExists(QString name)
{
    QSqlQuery query(QSqlDatabase::database(sqlName));
    query.prepare("select id from aip_master where name = :name");
    query.bindValue(":name", name);
    query.exec();
    if (query.next()) {
        return true;
    } else {
        return false;
    }
}

QString Master::getText(QString title, QString text, QString s)
{
    bool ok;
    QString input = QInputDialog::getText(this, title, text, QLineEdit::Normal, s, &ok);
    if (ok)
        return input;
    else
        return s;
}

void Master::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        mView->select();
    }
}

void Master::recvSqlOpen(QString sql)
{
    sqlName = sql;
    createSqlite();
    updateSqlite();
}


