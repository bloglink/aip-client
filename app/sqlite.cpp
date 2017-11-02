/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据管理
*******************************************************************************/
#include "sqlite.h"

Sqlite::Sqlite(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Sqlite::initUI()
{
    initView();
    initButton();
    initLayout();
}

void Sqlite::initView()
{
    view = new QTableView(this);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateViews(QModelIndex)));

    views = new QTableView(this);
    views->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Sqlite::initButton()
{
    dateEdit = new QDateEdit(this);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setDate(QDate::currentDate());

    QPushButton *btnUpdate = new QPushButton(this);
    btnUpdate->setText(tr("查询"));
    btnUpdate->setMinimumSize(97, 35);
    connect(btnUpdate, SIGNAL(clicked(bool)), this, SLOT(updateSqliteByDate()));

    QPushButton *btnExport = new QPushButton(this);
    btnExport->setText(tr("导出当前"));
    btnExport->setMinimumSize(97, 35);
    connect(btnExport, SIGNAL(clicked(bool)), this, SLOT(exportSqliteByDate()));

    QPushButton *btnExports = new QPushButton(this);
    btnExports->setText(tr("全部导出"));
    btnExports->setMinimumSize(97, 35);
    connect(btnExports, SIGNAL(clicked(bool)), this, SLOT(exportSqlite()));

    QPushButton *btnDelete = new QPushButton(this);
    btnDelete->setText(tr("清空数据"));
    btnDelete->setMinimumSize(97, 35);
    connect(btnDelete, SIGNAL(clicked(bool)), this, SLOT(deleteSqlite()));

    QPushButton *btnExit = new QPushButton(this);
    btnExit->setText(tr("退出"));
    btnExit->setMinimumSize(97, 35);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(exist()));

    btnLayout = new QHBoxLayout;
    btnLayout->addWidget(new QLabel("测试日期", this));
    btnLayout->addWidget(dateEdit);
    btnLayout->addWidget(btnUpdate);
    btnLayout->addWidget(btnExport);
    btnLayout->addWidget(btnExports);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();
    btnLayout->addWidget(btnExit);
}

void Sqlite::initLayout()
{
    QHBoxLayout *viewLayout = new QHBoxLayout;
    viewLayout->addWidget(view);
    viewLayout->addWidget(views);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(viewLayout);
    layout->addLayout(btnLayout);
    this->setLayout(layout);
}

void Sqlite::createSqlite()
{
    QSqlQuery query(QSqlDatabase::database(sqlName));

    query.exec("drop table TestData");
    query.exec("dorp table TestDatas");

    QString s = "create table if not exists TestData (";
    s += "id integer primary key, ";
    s += "date text, ";
    s += "time text, ";
    s += "type text, ";
    s += "code text, ";
    s += "user text, ";
    s += "judge text)";
    if (!query.exec(s))
        qDebug() << "create table TestData Error";

    s = "create table if not exists TestDatas (";
    s += "id integer primary key, ";
    s += "parent integer, ";
    s += "item text, ";
    s += "para text, ";
    s += "result text, ";
    s += "judge text)";
    if (!query.exec(s))
        qDebug() << "create table TestDatas Error";
}

void Sqlite::updateSqlite()
{
    mView = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mView->setTable("TestData");
    mView->select();
    view->setModel(mView);
    view->hideColumn(0);
    QStringList header;
    header << tr("ID") << tr("测试日期") << tr("测试时间")
           << tr("测试型号") << tr("产品编码") << tr("测试人") << tr("测试判定");
    for (int i=0; i < header.size(); i++)
        mView->setHeaderData(i, Qt::Horizontal, header.at(i));

    mViews = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mViews->setTable("TestDatas");
    mViews->select();
    views->setModel(mViews);
    views->hideColumn(0);
    views->hideColumn(1);
    QStringList headers;
    headers << tr("ID") << tr("父ID") << tr("项目") << tr("参数") << tr("结果") << tr("判定");
    for (int i=0; i < headers.size(); i++)
        mViews->setHeaderData(i, Qt::Horizontal, headers.at(i));
}

void Sqlite::updateSqliteByDate()
{
    QString s = dateEdit->date().toString("yyyy-MM-dd");
    mView->setFilter(QObject::tr("date='%1'").arg(s));
    mView->select();
}

void Sqlite::exportSqlite()
{
    QString name = getSaveFileName();
    QFile file(QString("%1.csv").arg(name));
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this,  "",   "创建失败",  QMessageBox::Ok);
        return;
    }
    QSqlQuery query(QSqlDatabase::database(sqlName));
    QString cmd;
    cmd = "select TestData.id, TestData.date, ";
    cmd += "TestData.time, TestData.type, TestData.code, ";
    cmd += "TestData.user, TestData.judge, ";
    cmd += "TestDatas.item, TestDatas.para, ";
    cmd += "TestDatas.result, TestDatas.judge ";
    cmd += "from TestData ";
    cmd += "INNER JOIN TestDatas ON TestData.id = TestDatas.parent";
    query.exec(cmd);
    QStringList header;
    header << tr("测试日期") << tr("测试时间") << tr("测试型号")
           << tr("产品编码") << tr("测试人") << tr("测试判定");
    for (int i=0; i < header.size(); i++) {
        file.write(ToGbk(header.at(i)));
        file.write(",");
    }
    QStringList headers;
    headers << tr("测试项目") << tr("测试参数") << tr("测试结果") << tr("测试判定");
    for (int i=0; i < 5; i++) {
        for (int j=0; j < headers.size(); j++) {
            file.write(ToGbk(headers.at(j%4)));
            file.write(",");
        }
    }
    while (query.next()) {
        double id = query.value(0).toDouble();
        if (current_id != id) {
            file.write("\n");
            file.write(ToGbk(query.value(1).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(2).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(3).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(4).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(5).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(6).toString().replace(",", " ")));
            file.write(",");
        } else {
            // nothing
        }
        file.write(ToGbk(query.value(7).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(8).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(9).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(10).toString().replace(",", " ")));
        file.write(",");
        current_id = id;
    }
    file.close();
    QMessageBox::information(this,  "",   "导出成功",  QMessageBox::Ok);
}

void Sqlite::exportSqliteByDate()
{
    QString name = getSaveFileName();
    QFile file(QString("%1.csv").arg(name));
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this,  "",   "创建失败",  QMessageBox::Ok);
        return;
    }
    QSqlQuery query(QSqlDatabase::database(sqlName));
    QString cmd;
    cmd = "select TestData.id, TestData.date, ";
    cmd += "TestData.time, TestData.type, TestData.code, ";
    cmd += "TestData.user, TestData.judge, ";
    cmd += "TestDatas.item, TestDatas.para, ";
    cmd += "TestDatas.result, TestDatas.judge ";
    cmd += "from TestData ";
    cmd += "INNER JOIN TestDatas ON TestData.id = TestDatas.parent";
    query.exec(cmd);
    QStringList header;
    header << tr("测试日期") << tr("测试时间") << tr("测试型号")
           << tr("产品编码") << tr("测试人") << tr("测试判定");
    for (int i=0; i < header.size(); i++) {
        file.write(ToGbk(header.at(i)));
        file.write(",");
    }
    QStringList headers;
    headers << tr("测试项目") << tr("测试参数") << tr("测试结果") << tr("测试判定");
    for (int i=0; i < 5; i++) {
        for (int j=0; j < headers.size(); j++) {
            file.write(ToGbk(headers.at(j%4)));
            file.write(",");
        }
    }
    while (query.next()) {
        QString s = query.value(1).toString();
        if (s != dateEdit->date().toString("yyyy-MM-dd"))
            continue;
        double id = query.value(0).toDouble();
        if (current_id != id) {
            file.write("\n");
            file.write(ToGbk(query.value(1).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(2).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(3).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(4).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(5).toString().replace(",", " ")));
            file.write(",");
            file.write(ToGbk(query.value(6).toString().replace(",", " ")));
            file.write(",");
        } else {
            // nothing
        }
        file.write(ToGbk(query.value(7).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(8).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(9).toString().replace(",", " ")));
        file.write(",");
        file.write(ToGbk(query.value(10).toString().replace(",", " ")));
        file.write(",");
        current_id = id;
    }
    file.close();
    QMessageBox::information(this,  "",   "导出成功",  QMessageBox::Ok);
}

void Sqlite::deleteSqlite()
{
    QSqlDatabase::database(sqlName).close();
    QSqlDatabase::database(sqlName).open();

    QSqlQuery query(QSqlDatabase::database(sqlName));
    if (!query.exec("drop table TestData"))
        qDebug() << query.lastError();
    if (!query.exec("drop table TestDatas"))
        qDebug() << query.lastError();
    createSqlite();
    updateSqlite();
}

void Sqlite::updateViews(QModelIndex index)
{
    int row = index.row();
    quint64 id = mView->index(row, 0).data().toDouble();
    mViews->setFilter(QObject::tr("parent=%1").arg(id));
    mViews->select();
}

void Sqlite::exist()
{
    emit sendAppShow("author");
}

QByteArray Sqlite::ToGbk(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    return gbk->fromUnicode(inStr);
}

QString Sqlite::getSaveFileName()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Open Config"),
                                                    dateEdit->date().toString("yy-MM-dd"),
                                                    tr("Config Files (*.csv)"));
    return fileName;
}

void Sqlite::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        mView->select();
        mViews->select();
    }
}

void Sqlite::recvSqlOpen(QString sql)
{
    sqlName = sql;
    createSqlite();
    updateSqlite();
}

