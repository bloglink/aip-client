/*******************************************************************************
 * Copyright [2017]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       设备管理
*******************************************************************************/
#include "device.h"

Device::Device(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Device::initUI()
{
    initView();
    initCtrl();

    this->setObjectName("device");

    QSplitter *hsplitter = new QSplitter(Qt::Vertical, this);
    hsplitter->addWidget(repairView);
    hsplitter->addWidget(recordView);
    hsplitter->addWidget(deviceCtrl);
    hsplitter->setStretchFactor(0, 5);
    hsplitter->setStretchFactor(1, 5);

    QSplitter *vsplitter = new QSplitter(this);
    vsplitter->addWidget(deviceView);
    vsplitter->addWidget(hsplitter);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vsplitter);
    this->setLayout(layout);
}

void Device::initView()
{
    deviceView = new QTableView(this);
    deviceView->setSortingEnabled(true);
    deviceView->setSelectionMode(QAbstractItemView::SingleSelection);
    deviceView->setSelectionBehavior(QAbstractItemView::SelectRows);
    deviceView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(deviceView, SIGNAL(clicked(QModelIndex)), this, SLOT(clickDevice(QModelIndex)));

    repairView = new QTableView(this);
    repairView->setSortingEnabled(true);
    repairView->setSelectionMode(QAbstractItemView::SingleSelection);
    repairView->setSelectionBehavior(QAbstractItemView::SelectRows);
    repairView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    recordView = new QTableView(this);
    recordView->setSortingEnabled(true);
    recordView->setSelectionMode(QAbstractItemView::SingleSelection);
    recordView->setSelectionBehavior(QAbstractItemView::SelectRows);
    recordView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Device::initDevice(QSqlDatabase db)
{
    mDeviceView = new QSqlTableModel(this, db);
    mDeviceView->setTable("aip_device");

    QStringList headers;
    headers << "设备编号" << "设备地址" << "出厂编号" << "设备型号"
            << "评审编号" << "出厂日期" << "客户名称";
    if (mDeviceView->columnCount() >= headers.size()) {
        for (int i=0; i < qMin(headers.size(), mDeviceView->columnCount()); i++) {
            mDeviceView->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
        deviceView->setModel(mDeviceView);
        deviceView->hideColumn(0);
    }
}

void Device::initRepair(QSqlDatabase db)
{
    mRepairView = new QSqlTableModel(this, db);
    mRepairView->setTable("aip_repair");
    if (mRepairView->columnCount() >= 8) {
        QStringList headers;
        headers << "记录编号" << "设备编号" << "记录人" << "情况描述"
                << "维修描述" << "维修日期" << "联系人" << "联系电话";
        for (int i=0; i < qMin(headers.size(), mRepairView->columnCount()); i++) {
            mRepairView->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
        repairView->setModel(mRepairView);
        repairView->hideColumn(0);
        repairView->hideColumn(1);
    }
}

void Device::initRecord(QSqlDatabase db)
{
    mRecordView = new QSqlTableModel(this, db);
    mRecordView->setTable("aip_record");
    if (mRecordView->columnCount() >= 9) {
        QStringList headers;
        headers << "记录编号" << "设备编号" << "设备地址" << "出厂编号"
                << "网络端口" << "网络地址" << "记录时间" << "设备状态" << "软件版本";
        for (int i=0; i < qMin(headers.size(), mRecordView->columnCount()); i++) {
            mRecordView->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
        recordView->setModel(mRecordView);
        recordView->hideColumn(0);
        recordView->hideColumn(1);
        recordView->hideColumn(4);
    }
}

void Device::clickDevice(QModelIndex index)
{
    int row = index.row();
    QString uuid = mDeviceView->index(row, 0).data().toString();
    QString filter = QString("devUuid = %1").arg(uuid);
    mRepairView->setFilter(filter);
    mRepairView->select();
    mRecordView->setFilter(filter);
    mRecordView->select();
}

void Device::initCtrl()
{
    QPushButton *btnSave = new QPushButton("保存修改", this);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveCtrl()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(btnSave);
    layout->setMargin(0);

    deviceCtrl = new QWidget(this);
    deviceCtrl->setLayout(layout);
}

void Device::saveCtrl()
{
    mDeviceView->submitAll();
    mRepairView->submitAll();
    mRecordView->submitAll();
}

void Device::updateSqlite()
{
    initDevice(QSqlDatabase::database(sqlName));
    initRepair(QSqlDatabase::database(sqlName));
    initRecord(QSqlDatabase::database(sqlName));
}

void Device::recvAppShow(QString win)
{
    if (win == this->objectName())
        mDeviceView->select();
}

void Device::recvSqlOpen(QString sql)
{
    sqlName = sql;
    updateSqlite();
}

