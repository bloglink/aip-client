#include "WinClient.h"
#include "ui_WinClient.h"

WinClient::WinClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinClient)
{
    ui->setupUi(this);
    WinInit();
    KeyInit();
    DatInit();
    TcpInit();
}

WinClient::~WinClient()
{
    delete ui;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      界面初始化
******************************************************************************/
void WinClient::WinInit()
{
#ifndef LOCAL
    ui->tabTcp->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    ui->tabTcp->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    ui->tabTcp->setRowCount(W_ROW);
    for (int i=0; i<W_ROW*W_COL; i++) {
        ui->tabTcp->setItem(i/W_COL,i%W_COL,new QTableWidgetItem);
        ui->tabTcp->item(i/W_COL,i%W_COL)->setTextAlignment(Qt::AlignCenter);
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      更新显示
******************************************************************************/
void WinClient::WinUpdate(QByteArray msg)
{
    QString Item = msg;
    QStringList ItemText = Item.split("@@");
    for (int i=0; i<W_ROW*W_COL; i++) {
        ui->tabTcp->item(i/W_COL,i%W_COL)->setText("");
    }
    for (int i=0; i<ItemText.size(); i++) {
        QStringList row = ItemText.at(i).split(" ");
        for (int j=0; j<row.size(); j++) {
            ui->tabTcp->item(i,j)->setText(row.at(j));
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      按键初始化
******************************************************************************/
void WinClient::KeyInit()
{
    connect(ui->KeyFileLocalGet,  SIGNAL(clicked(bool)),  this,SLOT(TcpFileLocalGet()));
    connect(ui->EditFileLocalGet, SIGNAL(returnPressed()),this,SLOT(TcpFileLocalGet()));
    connect(ui->KeyFileLocalPut,  SIGNAL(clicked(bool)),  this,SLOT(TcpFileLocalPut()));
    connect(ui->EditFileLocalPut, SIGNAL(returnPressed()),this,SLOT(TcpFileLocalPut()));
    connect(ui->KeyFileClientGet, SIGNAL(clicked(bool)),  this,SLOT(TcpFileClientGet()));
    connect(ui->EditFileClientGet,SIGNAL(returnPressed()),this,SLOT(TcpFileClientGet()));
    connect(ui->KeyFileClientPut, SIGNAL(clicked(bool)),  this,SLOT(TcpFileClientPut()));
    connect(ui->EditFileClientPut,SIGNAL(returnPressed()),this,SLOT(TcpFileClientPut()));

    connect(ui->KeyCmdSystem, SIGNAL(clicked(bool)),  this,SLOT(TcpCmdSystem()));
    connect(ui->EditCmdSystem,SIGNAL(returnPressed()),this,SLOT(TcpCmdSystem()));

    connect(ui->KeyLogin,SIGNAL(clicked(bool)),this,SLOT(TcpStart()));
    connect(ui->EditUser,SIGNAL(returnPressed()),this,SLOT(TcpStart()));
    connect(ui->EditPassword,SIGNAL(returnPressed()),this,SLOT(TcpStart()));

    connect(ui->KeyExit,SIGNAL(clicked(bool)),this,SLOT(close()));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      数据初始化
******************************************************************************/
void WinClient::DatInit()
{
    set = new QSettings("./default.ini",QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("Client");

    ui->EditFileLocalGet->setText(set->value("LOCALGET","network/aip").toString());
    ui->EditFileLocalPut->setText(set->value("LOCALPUT","network/aip").toString());
    ui->EditFileClientGet->setText(set->value("CLIENTGET","network/aip").toString());
    ui->EditFileClientPut->setText(set->value("CLIENTPUT","/mnt/aip").toString());
    ui->EditCmdSystem->setText(set->value("CMDSYSTEM","ls").toString());

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(TcpGetListMsg()));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      数据保存
******************************************************************************/
void WinClient::DatSave()
{
    set->setValue("LOCALGET",ui->EditFileLocalGet->text());
    set->setValue("LOCALPUT",ui->EditFileLocalPut->text());
    set->setValue("CLIENTGET",ui->EditFileClientGet->text());
    set->setValue("CLIENTPUT",ui->EditFileClientPut->text());
    set->setValue("CMDSYSTEM",ui->EditCmdSystem->text());
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      网络初始化
******************************************************************************/
void WinClient::TcpInit()
{
    tcp = new QThread(this);
    MyTcpThread *tcpThread = new MyTcpThread;
    tcpThread->moveToThread(tcp);
    connect(tcp, SIGNAL(started()), tcpThread,SLOT(TcpInit()));
    connect(tcp, SIGNAL(finished()),tcpThread,SLOT(TcpQuit()));
    connect(tcpThread,SIGNAL(TransformCmd(quint16,quint16,QByteArray)),this,SLOT(ExcuteCmd(quint16,quint16,QByteArray)));
    connect(this,SIGNAL(PutBlock(quint16,quint16,QByteArray)),tcpThread,SLOT(PutBlock(quint16,quint16,QByteArray)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      网络登录登出
******************************************************************************/
void WinClient::TcpStart()
{
    if (ui->KeyLogin->text() == tr("登录"))
        TcpLogin();
    else
        TcpLogout();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      网络登录
******************************************************************************/
void WinClient::TcpLogin()
{
    QStringList temp;
    ui->KeyLogin->setText(tr("断开"));
    temp.append(ui->EditUser->text());
    temp.append(ui->EditPassword->text());
    set->setValue("HOST","s.aipuo.com");
    set->setValue("USER",temp.join("@@"));
    set->setValue("VERSION","V0.1");
    set->sync();
    tcp->start();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      网络登出
******************************************************************************/
void WinClient::TcpLogout()
{
    ui->KeyLogin->setText(tr("登录"));
    WinUpdate(NULL);
    timer->stop();
    tcp->quit();
    tcp->wait();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      获取在线列表
******************************************************************************/
void WinClient::TcpGetListMsg()
{
    emit PutBlock(ADDR,ListMsg,"NULL");
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      本地下载
******************************************************************************/
void WinClient::TcpFileLocalGet()
{
    emit PutBlock(ADDR,ClientGetHead,ui->EditFileLocalGet->text().toUtf8());
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      本地上传
******************************************************************************/
void WinClient::TcpFileLocalPut()
{
    emit PutBlock(ADDR,ServerGetHead,ui->EditFileLocalPut->text().toUtf8());
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      客户下载
******************************************************************************/
void WinClient::TcpFileClientGet()
{
    int ret = ui->tabTcp->currentRow();
    if (ret < 0)
        return;
    if (ui->EditFileClientGet->text().isEmpty())
        return;
    quint16 port = quint16(ui->tabTcp->item(ret,0)->text().toInt());
    QByteArray name = ui->EditFileClientGet->text().toUtf8();
    emit PutBlock(quint16(port),quint16(ClientGetHead),name);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      客户上传
******************************************************************************/
void WinClient::TcpFileClientPut()
{
    int ret = ui->tabTcp->currentRow();
    if (ret < 0)
        return;
    if (ui->EditFileClientGet->text().isEmpty())
        return;
    quint16 port = quint16(ui->tabTcp->item(ret,0)->text().toInt());
    QByteArray name = ui->EditFileClientGet->text().toUtf8();
    emit PutBlock(quint16(port),quint16(ServerGetHead),name);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      发送系统命令
******************************************************************************/
void WinClient::TcpCmdSystem()
{
    int ret = ui->tabTcp->currentRow();
    if (ret < 0)
        return;
    if (ui->EditCmdSystem->text().isEmpty())
        return;
    quint16 port = quint16(ui->tabTcp->item(ret,0)->text().toInt());
    QByteArray cmd = ui->EditCmdSystem->text().toUtf8();
    emit PutBlock(quint16(port),quint16(CmdMsg),cmd);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      显示信息
******************************************************************************/
void WinClient::TcpDisplay(QString msg)
{
    ui->Text->insertPlainText(msg);
    ui->Text->moveCursor(QTextCursor::End);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      执行命令
******************************************************************************/
void WinClient::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    switch (cmd) {
    case LocalLoginError:
        this->TcpDisplay(msg);
        TcpLogout();
        break;
    case LocalLoginSuccess:
        this->TcpDisplay("用户名和密码验证成功\n");
        timer->start(10000);
        TcpGetListMsg();
        break;
    case ListMsg:
        this->WinUpdate(msg);
        break;
    case ListError:
        break;
    case SocketDisplay:
        this->TcpDisplay(msg);
        break;
    default:
        qDebug()<<addr<<cmd<<msg;
        break;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.03.24
  * brief:      退出保存
******************************************************************************/
void WinClient::closeEvent(QCloseEvent *e)
{
    DatSave();
    tcp->quit();
    tcp->wait();
    e->accept();
}
