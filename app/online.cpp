/*******************************************************************************
 * Copyright [2017]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       在线设备
*******************************************************************************/
#include "online.h"

Online::Online(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void Online::initUI()
{
    initView();
    initTree();
    initFile();
    initText();
    initCtrl();
    initLayout();
}

void Online::initView()
{
    onlineView = new QTableView(this);
    onlineView->setSortingEnabled(true);
    onlineView->setItemDelegate(new LQReadOnlyItem());
    onlineView->horizontalHeader()->setHighlightSections(false);
    onlineView->setSelectionBehavior(QAbstractItemView::SelectRows);
    onlineView->setSelectionMode(QAbstractItemView::SingleSelection);
    onlineView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(onlineView, SIGNAL(clicked(QModelIndex)), this, SLOT(clickOnline(QModelIndex)));
}

void Online::initTree()
{
    onlineTree = new QTreeWidget(this);
    onlineTree->setHeaderHidden(true);
    connect(onlineTree, SIGNAL(clicked(QModelIndex)), this, SLOT(clickTree(QModelIndex)));
}

void Online::initFile()
{
    mOnlineFile = new QFileSystemModel(this);
    mOnlineFile->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    onlineFile = new QTreeView(this);
    onlineFile->setModel(mOnlineFile);
#ifdef __linux__
    onlineFile->setRootIndex(mOnlineFile->setRootPath("/home/link/smbtmp/"));
#else
    onlineFile->setRootIndex(mOnlineFile->setRootPath("\\\\192.168.1.55\\share\\二代升级文件"));
#endif
    onlineFile->setHeaderHidden(true);
    onlineFile->hideColumn(3);
    onlineFile->hideColumn(2);
    onlineFile->hideColumn(1);
    connect(onlineFile, SIGNAL(clicked(QModelIndex)), this, SLOT(clickFile(QModelIndex)));
}

void Online::initText()
{
    QStringList items;
    items << "设备编号" << "设备地址" << "出厂编号" << "设备型号"
          << "评审编号" << "出厂日期" << "客户名称" << "网络端口";
    QStringList headers;
    headers << "项目" << "参数";
    mOnlineText = new LQStandardItem(this);
    mOnlineText->setColumnCount(headers.size());
    mOnlineText->setRowCount(items.size());
    for (int i=0; i < mOnlineText->columnCount(); i++) {
        mOnlineText->setHeaderData(i, Qt::Horizontal, headers.at(i));
    }
    for (int i=0; i < items.size(); i++) {
        mOnlineText->setItem(i, 0, new QStandardItem(items.at(i)));
        mOnlineText->setItem(i, 1, new QStandardItem());
    }
    onlineText = new QTableView(this);
    onlineText->setModel(mOnlineText);
    onlineText->setItemDelegateForColumn(0, new LQReadOnlyItem);
    onlineText->setItemDelegateForColumn(1, new LQReadOnlyItem);
    onlineText->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    onlineText->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Online::initCtrl()
{
    QPushButton *btnShellCmd = new QPushButton("发送命令", this);
    connect(btnShellCmd, SIGNAL(clicked(bool)), this, SLOT(sendShellCmd()));
    shellCmd = new QLineEdit(this);
    connect(shellCmd, SIGNAL(editingFinished()), this, SLOT(sendShellCmd()));

    QPushButton *btnSendFile = new QPushButton("发送文件", this);
    connect(btnSendFile, SIGNAL(clicked(bool)), this, SLOT(sendFileHead()));
    sendFile = new QLineEdit(this);
    connect(sendFile, SIGNAL(editingFinished()), this, SLOT(sendFileHead()));

    QPushButton *btnRecvFile = new QPushButton("接收文件", this);
    connect(btnRecvFile, SIGNAL(clicked(bool)), this, SLOT(recvFileHead()));
    recvFile = new QLineEdit(this);
    connect(recvFile, SIGNAL(editingFinished()), this, SLOT(recvFileHead()));

    text = new QTextBrowser(this);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(recvFile, 0, 0);
    layout->addWidget(btnRecvFile, 0, 1);

    layout->addWidget(sendFile, 1, 0);
    layout->addWidget(btnSendFile, 1, 1);

    layout->addWidget(shellCmd, 2, 0);
    layout->addWidget(btnShellCmd, 2, 1);

    layout->addWidget(text, 3, 0, 1, 2);
    layout->setColumnStretch(0, 1);

    onlineCtrl = new QGroupBox(this);
    onlineCtrl->setLayout(layout);
}

void Online::initLayout()
{
    QSplitter *hsplitter = new QSplitter(this);
    hsplitter->addWidget(onlineText);
    hsplitter->addWidget(onlineTree);
    hsplitter->addWidget(onlineFile);
    hsplitter->addWidget(onlineCtrl);
    hsplitter->setStretchFactor(0, 3);
    hsplitter->setStretchFactor(1, 2);
    hsplitter->setStretchFactor(2, 2);
    hsplitter->setStretchFactor(3, 4);

    QSplitter *vsplitter = new QSplitter(Qt::Vertical, this);
    vsplitter->addWidget(onlineView);
    vsplitter->addWidget(hsplitter);
    vsplitter->setStretchFactor(0, 16);
    vsplitter->setStretchFactor(1, 5);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vsplitter);
    this->setLayout(layout);
}

void Online::clickTree(QModelIndex index)
{
    QString temp = onlineTree->currentItem()->text(1);
    QString name = onlineTree->currentItem()->text(0);
    if (temp == "文件夹") {
        int t = currentPath.lastIndexOf("/");
        if (name == ".." && t > 0) {
            currentPath = currentPath.mid(0,t);
        } else if (name == ".." && t == 0) {
            currentPath = "/";
        } else {
            if (currentPath != "/")
                currentPath.append("/");
            currentPath.append(name);
        }
        QString text = shellCmd->text();
        shellCmd->setText(QString("ls -aF %1").arg(currentPath));
        onlineTree->clear();
        sendShellCmd();
        shellCmd->setText(text);
    }
    if (temp == "文件") {
        QString path = currentPath;
        path.append("/");
        path.append(name);
        recvFile->setText(path);
    }
}

void Online::clickFile(QModelIndex index)
{
    QString path = mOnlineFile->filePath(index);
#ifdef __linux__
    path.remove("/home/link/smbtmp/");
#else
    path.remove("//192.168.1.55/share/二代升级文件/");
#endif
    sendFile->setText(path);
}

void Online::clickOnline(QModelIndex index)
{
    int row = index.row();
    QString uuid = mOnlineView->index(row, ONLINE_DEV_UUID).data().toString();
    QString filter = QString("devUuid = %1").arg(uuid);
    mDeviceView->setFilter(filter);
    mDeviceView->select();
    for (int i=0; i < mDeviceView->columnCount(); i++) {
        QString item = mDeviceView->index(0, i).data().toString();
        mOnlineText->setData(mOnlineText->index(i, 1), item, Qt::DisplayRole);
    }
    currentPort = mOnlineView->index(row, ONLINE_NET_PORT).data().toString();
    int pRow = mOnlineText->rowCount()-1;
    mOnlineText->setData(mOnlineText->index(pRow, 1), currentPort, Qt::DisplayRole);

    onlineTree->clear();
    shellCmd->setText("ls -aF /");
    currentPath = "/";
    recvFile->setText(currentPath);
    sendShellCmd();
    shellCmd->setText("mv network/userinfo.txt nandflash");
}

void Online::recvFileHead()
{
    qDebug() << "recv file" << recvFile->text();
    if (currentPort.isEmpty()) {
        qDebug() << "empty";
    } else {
        QJsonObject obj;
        obj.insert("txCommand", ONLINE_READ_HAED);
        obj.insert("txAddress", currentPort.toInt());
        obj.insert("txMessage", recvFile->text());
        emit sendNetMsg(obj);
    }
}

void Online::sendShellCmd()
{
    qDebug() << "shell cmd" << shellCmd->text();
    if (currentPort.isEmpty()) {
        qDebug() << "empty";
    } else {
        QJsonObject obj;
        obj.insert("txCommand", ONLINE_SHELL_CMD);
        obj.insert("txAddress", currentPort.toInt());
        obj.insert("txMessage", shellCmd->text());
        emit sendNetMsg(obj);
    }
}

void Online::sendFileHead()
{
    qDebug() << "send file" << sendFile->text();
    if (currentPort.isEmpty()) {
        qDebug() << "empty";
    } else {
        QJsonObject obj;
        obj.insert("txCommand", ONLINE_SEND_HAED);
        obj.insert("txAddress", currentPort.toInt());
        obj.insert("txMessage", sendFile->text());
        emit sendNetMsg(obj);
    }
}

void Online::updateSqlite()
{
    mOnlineView = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mOnlineView->setTable("aip_online");
    mOnlineView->select();
    QStringList headers;
    headers << "记录编号" << "设备编号" << "设备地址" << "出厂编号"
            << "网络端口" << "网络地址" << "记录时间" << "设备状态" << "软件版本";
    for (int i=0; i < qMin(headers.size(), mOnlineView->columnCount()); i++) {
        mOnlineView->setHeaderData(i, Qt::Horizontal, headers.at(i));
    }
    onlineView->setModel(mOnlineView);
    onlineView->hideColumn(0);
    onlineView->hideColumn(1);

    mDeviceView = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mDeviceView->setTable("aip_device");
}

void Online::recvAppShow(QString win)
{
    if (win == this->objectName())
        mOnlineView->select();
}

void Online::recvSqlOpen(QString sql)
{
    sqlName = sql;
    updateSqlite();
}

void Online::recvDisplay(QString msg)
{
    qDebug() << msg;
    if (msg.startsWith("./")) {
        QString temp = msg;
        QStringList FileList = temp.split("\n",QString::SkipEmptyParts);
        for (int i=0; i<FileList.size(); i++) {
            QString t = FileList.at(i);
            if (t.endsWith("@") || t.endsWith("|") || t.endsWith("=") || t == "./") {
                continue;
            }
            QTreeWidgetItem *item = new QTreeWidgetItem;
            if (t.endsWith("/")) {
                item->setText(0, t.remove("/"));
                QPixmap pixmap("./image/dir.png");
                item->setIcon(0, pixmap);
                item->setText(1, "文件夹");
            } else if (t.endsWith("*")) {
                item->setText(0, t.remove("*"));
                QPixmap pixmap("./image/link.png");
                item->setIcon(0, pixmap);
                item->setText(1, "文件");
            } else {
                item->setText(0, t.remove("*"));
                QPixmap pixmap("./image/file.png");
                item->setIcon(0, pixmap);
                item->setText(1, "文件");
            }
            onlineTree->addTopLevelItem(item);
        }
    } else {
        text->setPlainText(msg);
    }
}

