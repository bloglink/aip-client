#include "WinClient.h"
#include "ui_WinClient.h"

WinClient::WinClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinClient)
{
    ui->setupUi(this);
    InitWindows();
    InitButtons();
    InitClient();
}

WinClient::~WinClient()
{
    tcp->quit();
    tcp->wait();
    delete ui;
}

void WinClient::InitWindows()
{
    ui->tabTcp->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabTcp->setRowCount(W_ROW);
    for (int i=0; i<W_ROW*W_COL; i++) {
        ui->tabTcp->setItem(i/W_COL,i%W_COL,new QTableWidgetItem);
        ui->tabTcp->item(i/W_COL,i%W_COL)->setTextAlignment(Qt::AlignCenter);
    }
    this->setWindowTitle("二代服务端V-0.1.170328");

    files = new QFileSystemModel(this);
    files->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    ui->LocalFiles->setModel(files);
    ui->LocalFiles->setRootIndex(files->setRootPath("/home/link"));
    ui->LocalFiles->hideColumn(3);
    ui->LocalFiles->hideColumn(2);
    ui->LocalFiles->hideColumn(1);
    isFiles = false;
}

void WinClient::WinUpdate(QByteArray msg)
{
    QString Item = msg;
    QStringList ItemText = Item.split("\n", QString::SkipEmptyParts);
    for (int i=0; i<W_ROW*W_COL; i++) {
        ui->tabTcp->item(i/W_COL,i%W_COL)->setText("");
    }
    for (int i=0; i<ItemText.size(); i++) {
        QStringList row = ItemText.at(i).split("@@",QString::SkipEmptyParts);
        for (int j=0; j<row.size(); j++) {
            ui->tabTcp->item(i,j)->setText(row.at(j));
        }
    }
}

void WinClient::InitButtons()
{
    connect(ui->BtnLogin,SIGNAL(clicked(bool)),this,SLOT(Login()));
    connect(ui->BtnExit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->ShellCommand,SIGNAL(editingFinished()),this,SLOT(SendShellCommand()));
    connect(ui->LocalToServer,SIGNAL(clicked(bool)),this,SLOT(SendLocalFile()));
    connect(ui->ServerToLocal,SIGNAL(clicked(bool)),this,SLOT(ReadServerFile()));
    connect(ui->ServerToClient,SIGNAL(clicked(bool)),this,SLOT(SendServerFile()));
    connect(ui->ClientToServer,SIGNAL(clicked(bool)),this,SLOT(SendClientFile()));
}

void WinClient::InitClient()
{
    tcp = new QThread(this);
    MyTcpThread *s = new MyTcpThread;
    s->moveToThread(tcp);
    connect(tcp,SIGNAL(started()), s,SLOT(TcpInit()));
    connect(tcp,SIGNAL(finished()),s,SLOT(TcpQuit()));
    connect(s,SIGNAL(SendCommand(quint16,quint16,QByteArray)),this,
            SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(PutBlock(quint16,quint16,QByteArray)),s,SLOT(PutBlock(quint16,quint16,QByteArray)));
}

void WinClient::Login()
{
    if (ui->BtnLogin->text() == tr("登录")) {
        ui->BtnLogin->setText(tr("断开"));
        tcp->start();
    } else {
        ui->BtnLogin->setText(tr("登录"));
        WinUpdate(NULL);
        tcp->quit();
        tcp->wait();
    }
}

void WinClient::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    switch (cmd) {
    case ONLINE_DEVICES:
        this->WinUpdate(msg);
        break;
    case SHELL_DAT:
        if (isFiles) {
            isFiles = false;
            ReadClientFiles(msg);
        } else {
            ui->Text->setText(msg);
        }
        break;
    case SERVER_FILES:
        ReadServerFiles(msg);
        break;
    case GUEST_DISPLAY:
        ui->Text->setText(msg);
        break;
    default:
        qDebug()<<addr<<cmd<<msg;
        break;
    }
}

void WinClient::SendShellCommand()
{
    if (ui->TextPort->text().isEmpty())
        return;
    if (ui->ShellCommand->text().isEmpty())
        return;
    QByteArray msg = ui->ShellCommand->text().toUtf8();
    emit PutBlock(ui->TextPort->text().toInt(),SHELL_CMD,msg);
}

void WinClient::ReadServerFiles(QByteArray msg)
{
    ui->ServerFiles->clear();
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
            QPixmap pixmap(":/source/dir.png");
            item->setIcon(0, pixmap);
            item->setText(1, "文件夹");
        } else if (t.endsWith("*")) {
            item->setText(0, t.remove("*"));
            QPixmap pixmap(":/source/link.png");
            item->setIcon(0, pixmap);
            item->setText(1, "文件");
        } else {
            item->setText(0, t.remove("*"));
            QPixmap pixmap(":/source/file.png");
            item->setIcon(0, pixmap);
            item->setText(1, "文件");
        }
        ui->ServerFiles->addTopLevelItem(item);
    }
}

void WinClient::ReadClientFiles(QByteArray msg)
{
    ui->ClientFiles->clear();
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
            QPixmap pixmap(":/source/dir.png");
            item->setIcon(0, pixmap);
            item->setText(1, "文件夹");
        } else if (t.endsWith("*")) {
            item->setText(0, t.remove("*"));
            QPixmap pixmap(":/source/link.png");
            item->setIcon(0, pixmap);
            item->setText(1, "文件");
        } else {
            item->setText(0, t.remove("*"));
            QPixmap pixmap(":/source/file.png");
            item->setIcon(0, pixmap);
            item->setText(1, "文件");
        }
        ui->ClientFiles->addTopLevelItem(item);
    }
}

void WinClient::SendLocalFile()
{
    emit PutBlock(ADDR,GUEST_PUT_HEAD,ui->LocalFile->text().toUtf8());
}

void WinClient::SendServerFile()
{
    if (ui->TextPort->text().isEmpty())
        return;
    if (ui->ServerFile->text().isEmpty())
        return;
    QString name = ui->ServerFile->text().toUtf8();
    name.insert(0,"168912000X/");
    emit PutBlock(ui->TextPort->text().toInt(),GUEST_GET_HEAD,name.toUtf8());
}

void WinClient::SendClientFile()
{
    if (ui->TextPort->text().isEmpty())
        return;
    if (ui->ClientFile->text().isEmpty())
        return;
    QString name = ui->ClientFile->text().toUtf8();
    emit PutBlock(ui->TextPort->text().toInt(),GUEST_PUT_HEAD,name.toUtf8());
}

void WinClient::ReadServerFile()
{
    QString name = ui->ServerFile->text().toUtf8();
    name.insert(0,"168912000X/");
    emit PutBlock(ADDR,GUEST_GET_HEAD,name.toUtf8());
}

void WinClient::on_tabTcp_clicked(const QModelIndex &index)
{
    ui->TextPort->setText(ui->tabTcp->item(index.row(),0)->text());
    ui->ClientFile->setText("/");
    QString msg = QString("ls -aF %1").arg(ui->ClientFile->text());
    emit PutBlock(ui->TextPort->text().toInt(),SHELL_CMD,msg.toUtf8());
    isFiles = true;
}

void WinClient::on_LocalFiles_clicked(const QModelIndex &index)
{
    ui->LocalFile->setText(files->filePath(index));
}

void WinClient::on_ServerFiles_clicked(const QModelIndex &)
{
    QString name = ui->ServerFiles->currentItem()->text(0);
    ui->ServerFile->setText(name);
}

void WinClient::on_ClientFiles_clicked(const QModelIndex &)
{
    QString temp = ui->ClientFiles->currentItem()->text(1);
    QString name = ui->ClientFiles->currentItem()->text(0);
    int t = ui->ClientFile->text().lastIndexOf("/");
    QString path = ui->ClientFile->text().mid(0,t);
    if (temp == "文件夹") {
        int t = path.lastIndexOf("/");
        if (name == "..")
            ui->ClientFile->setText(path.mid(0,t) + "/");
        else
            ui->ClientFile->setText(path.append("/").append(name).append("/"));

        QString msg = QString("ls -aF %1").arg(path);
        emit PutBlock(ui->TextPort->text().toInt(),SHELL_CMD,msg.toUtf8());
        isFiles = true;
    }
    if (temp == "文件")
        ui->ClientFile->setText(path + "/" + name);
}
