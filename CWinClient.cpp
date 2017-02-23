#include "CWinClient.h"
#include "ui_CWinClient.h"

CWinClient::CWinClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CWinClient)
{
    ui->setupUi(this);
    WinInit();
    BtnInit();
    DatInit();
    TcpInit();
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       析构
******************************************************************************/
CWinClient::~CWinClient()
{
    delete ui;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       界面初始化
******************************************************************************/
void CWinClient::WinInit()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    ui->TabDevices->header()->setResizeMode(QHeaderView::Stretch);
#else
    ui->TabDevices->header()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       按键初始化
******************************************************************************/
void CWinClient::BtnInit()
{

    connect(ui->BtnLogin,SIGNAL(clicked(bool)),this,SLOT(TcpLogin()));
    connect(ui->EditUserName,SIGNAL(editingFinished()),this,SLOT(TcpLogin()));
    connect(ui->EditPassword,SIGNAL(editingFinished()),this,SLOT(TcpLogin()));


    //    connect(ui->KeyLogin,SIGNAL(clicked(bool)),this,SLOT(TcpStart()));
    //    connect(ui->EditUserName,SIGNAL(returnPressed()),this,SLOT(TcpStart()));
    //    connect(ui->EditPassword,SIGNAL(returnPressed()),this,SLOT(TcpStart()));
    //    connect(ui->tabTcp,SIGNAL(cellClicked(int,int)),this,SLOT(ItemClick(int,int)));

    //    connect(ui->BtnGetClientFileList,SIGNAL(clicked(bool)),this,SLOT(GetClientFileList()));
    //    connect(ui->TabFileList,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(ViewClick(QModelIndex)));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       数据初始化
******************************************************************************/
void CWinClient::DatInit()
{
    set = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("Client");
    ui->EditUserName->setText(set->value("UserName","sa").toString());
    ui->EditPassword->setText(set->value("Password","1234").toString());

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(TcpGetListMsg()));
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       数据保存
******************************************************************************/
void CWinClient::DatSave()
{
    set->setValue("UserName", ui->EditUserName->text());
    set->setValue("Password", ui->EditPassword->text());
}

void CWinClient::ItemClick(int r, int)
{
    //    QString p = ui->TabDevices->item(r,0)->text();
    //    ui->EditCurrentPort->setText(p);
}

void CWinClient::ViewClick(QModelIndex)
{
    QString temp = ui->TabFileList->currentItem()->text(2);
    QString name = ui->TabFileList->currentItem()->text(0);
    if (temp == "文件夹") {
        if (name == "..") {
            int t = CurrentPath.lastIndexOf("/");
            if (t != 0)
                CurrentPath = CurrentPath.mid(0,t);
        } else {
            CurrentPath.append(name);
            CurrentPath.append("/");
        }
        GetClientFileList();
    }
    if (temp == "文件") {
        QString path = CurrentPath;
        path.append(name);
        ui->EditCurrentFile->setText(path);
    }
}

void CWinClient::GetClientFileList()
{
    //    if (QString(ui->EditCurrentPort->text()).isEmpty())
    //        return;
    //    if (CurrentPath.isEmpty())
    //        CurrentPath = "/";
    //    quint16 port = quint16(ui->EditCurrentPort->text().toInt());

    //    QByteArray cmd = "ls -alg ";
    //    cmd.append(CurrentPath);
    //    emit PutBlock(quint16(port),quint16(TCP_CMD_SHEEL_CMD),cmd);
    //    isList = true;
}

void CWinClient::ShowFileList(QByteArray msg)
{
    ui->TabFileList->clear();
    QStringList FileList = QString(msg).split("\n",QString::SkipEmptyParts);
    for (int i=1; i<FileList.size(); i++) {
        QStringList FileAll = FileList.at(i).split(" ",QString::SkipEmptyParts);
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, FileAll.at(7));
        item->setText(1, FileAll.at(3));
        item->setText(2, FileAll.at(3));
        if (QString(FileAll.first()).startsWith("-")) {
            item->setText(2, "文件");
            QPixmap pixmap(":/image/file.png");
            item->setIcon(0, pixmap);
        }
        if (QString(FileAll.first()).startsWith("d")) {
            item->setText(2, "文件夹");
            QPixmap pixmap(":/image/dir.png");
            item->setIcon(0, pixmap);
        }
        if (QString(FileAll.first()).startsWith("l")) {
            item->setText(2, "链接");
            QPixmap pixmap(":/image/link.png");
            item->setIcon(0, pixmap);
        }
        ui->TabFileList->addTopLevelItem(item);
    }
    isList = false;
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       网络初始化
******************************************************************************/
void CWinClient::TcpInit()
{
    tcp = new QThread(this);
    CTcpNetwork *tcpThread = new CTcpNetwork;
    tcpThread->moveToThread(tcp);
    connect(tcp, SIGNAL(started()), tcpThread,SLOT(ConnectToHost()));
    connect(tcp, SIGNAL(finished()),tcpThread,SLOT(Disconnect()));
    connect(tcpThread,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
#ifdef __arm__
    TcpLogin();
#endif
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       网络登录登出
******************************************************************************/
void CWinClient::TcpLogin()
{
    if (ui->BtnLogin->text() == tr("登录")) {
        ui->BtnLogin->setText(tr("断开"));
        set->setValue("UserName",ui->EditUserName->text());
        set->setValue("Password",ui->EditPassword->text());
        tcp->start();
    } else {
        ui->BtnLogin->setText(tr("登录"));
        ui->TabDevices->clear();
        timer->stop();
        tcp->quit();
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       获取在线列表
******************************************************************************/
void CWinClient::TcpGetListMsg()
{
    QByteArray cmd = "NULL";
    //    emit PutBlock(ADDR,TCP_CMD_LIST_GET,cmd);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       本地下载
******************************************************************************/
void CWinClient::TcpLocalGet()
{
    //    QByteArray cmd = ui->EditLocalGet->text().toUtf8();
    //    emit PutBlock(ADDR,TCP_CMD_CLIENT_GET_HEAD,cmd);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       本地上传
******************************************************************************/
void CWinClient::TcpLocalPut()
{
    //    QByteArray cmd = ui->EditLocalPut->text().toUtf8();
    //    emit PutBlock(ADDR,TCP_CMD_SERVER_GET_HEAD,cmd);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       客户下载
******************************************************************************/
void CWinClient::TcpClientGet()
{
    //    int ret = ui->tabTcp->currentRow();
    //    if (ret < 0)
    //        return;
    //    if (ui->EditClientGet->text().isEmpty())
    //        return;
    //    quint16 port = quint16(ui->tabTcp->item(ret,0)->text().toInt());
    //    QByteArray name = ui->EditClientGet->text().toUtf8();
    //    emit PutBlock(quint16(port),quint16(TCP_CMD_CLIENT_GET_HEAD),name);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       客户上传
******************************************************************************/
void CWinClient::TcpClientPut()
{
    //    int ret = ui->tabTcp->currentRow();
    //    if (ret < 0)
    //        return;
    //    if (ui->EditClientGet->text().isEmpty())
    //        return;
    //    quint16 port = quint16(ui->tabTcp->item(ret,0)->text().toInt());
    //    QByteArray name = ui->EditClientPut->text().toUtf8();
    //    emit PutBlock(quint16(port),quint16(TCP_CMD_SERVER_GET_HEAD),name);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       发送系统命令
******************************************************************************/
void CWinClient::TcpCommand()
{
    //    int ret = ui->tabTcp->currentRow();
    //    if (ret < 0)
    //        return;
    //    if (ui->EditCommand->text().isEmpty())
    //        return;
    //    quint16 port = quint16(ui->tabTcp->item(ret,0)->text().toInt());
    //    QByteArray cmd = ui->EditCommand->text().toUtf8();
    //    emit PutBlock(quint16(port),quint16(TCP_CMD_SHEEL_CMD),cmd);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       显示信息
******************************************************************************/
void CWinClient::TcpDisplay(QString msg)
{
    if (isList)
        this->ShowFileList(msg.toUtf8());
    else
        ui->Text->setText(msg);
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       执行命令
******************************************************************************/
void CWinClient::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    switch (cmd) {
    case TCP_CMD_LOGIN_ERROR:
        this->TcpDisplay(msg);
        break;
    case TCP_CMD_LOGIN_SUCCESS:
        this->TcpDisplay("用户名和密码验证成功\n");
        timer->start(10000);
        TcpGetListMsg();
        break;
    case TCP_CMD_LIST_GET:
        break;
    case TCP_CMD_LIST_ERROR:
        break;
    case TCP_CMD_SOCKET_DISPLAY:
        this->TcpDisplay(msg);
        break;
    case TCP_CMD_SHEEL_RETURN:
        this->ShowFileList(msg);
        break;
    default:
        qDebug()<<addr<<cmd<<msg;
        break;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.03.24
 * brief:       退出保存
******************************************************************************/
void CWinClient::hideEvent(QHideEvent *e)
{
    DatSave();
    tcp->quit();
    tcp->wait();
    e->accept();
}
