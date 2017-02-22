#include "MyTcpThread.h"

MyTcpThread::MyTcpThread(QObject *parent) :
    QTcpSocket(parent)
{
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

    proc = new QProcess(this);
    set = new QSettings("./default.ini",QSettings::IniFormat);
    set->setIniCodec("GB18030");
    set->beginGroup("Client");

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(KeepAlive()));
    connect(this,SIGNAL(connected()),this,SLOT(Connected()));
    connect(this,SIGNAL(readyRead()),this,SLOT(GetBlock()));
    connect(this,SIGNAL(bytesWritten(qint64)),this,SLOT(PutFileData(qint64)));
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(Error(QAbstractSocket::SocketError)));
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      网络初始化
******************************************************************************/
void MyTcpThread::TcpInit()
{
    count = 0;
    HostName = set->value("HOST","s.aipuo.com").toString();
    Version = set->value("VERSION","V0.1").toString();
    User = set->value("USER","sa@@1234").toString();

    this->connectToHost(HostName, 6000);
    timer->start(10000);
    Display("连接服务器...\n");
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      网络退出
******************************************************************************/
void MyTcpThread::TcpQuit()
{
    this->close();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      连接成功
******************************************************************************/
void MyTcpThread::Connected()
{
#ifndef LOCAL
    ExcuteCmd(quint16(ADDR),quint16(ClientPutInit),NULL);
#else
    ExcuteCmd(quint16(ADDR),quint16(LocalLogin),NULL);
#endif
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      心跳
******************************************************************************/
void MyTcpThread::KeepAlive()
{
    if (this->state() != QAbstractSocket::ConnectedState) {
        TcpInit();
    } else {
        PutBlock(ADDR,GetHeart,"NULL");
        count++;
        if (count >6) {
            Display("服务器无反应，断开连接\n");
            this->abort();
        }
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      读取数据
******************************************************************************/
void MyTcpThread::GetBlock()
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
        ExcuteCmd(addr,cmd,msg);
        BlockSize = 0;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      读取文件头
******************************************************************************/
void MyTcpThread::GetFileHead(quint16 addr,QByteArray msg)
{
    SendVerify = msg.mid(0,16);
    QString temp = msg.remove(0,17);
    QStringList detail = temp.split(" ");
    BytesToRead = detail.at(0).toInt();
    BytesRead = 0;
    file = new QFile(QString(TMP).append(detail.at(1)));
    if(!file->open(QFile::ReadWrite)) {
        PutBlock(addr,HeadError,file->errorString().toUtf8());
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      读取文件
******************************************************************************/
void MyTcpThread::GetFileData(quint16 addr,QByteArray msg)
{
    int ret;
    if (file->isOpen())
        ret = file->write(msg);
    else
        return;
    BytesRead += ret;
    if (BytesRead == BytesToRead) {
        file->seek(0);
        RecvVerify = QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5);
        file->close();
        BytesRead = 0;
        BytesToRead = 0;
        if (SendVerify == RecvVerify) {
            QProcess::execute("sync");
            QString cmd = QString("mv %1 %2").arg(file->fileName()).arg(NET);
            QProcess::execute(cmd);
            PutBlock(addr,DataSuccess,"NULL");
            qDebug() << "Put Data Success";
        } else {
            QString cmd = QString("rm %1").arg(file->fileName());
            QProcess::execute(cmd);
            PutBlock(addr,DataError,"NULL");
            qDebug() << "Put Data Error";
        }
        return;
    }
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      写数据
******************************************************************************/
void MyTcpThread::PutBlock(quint16 addr, quint16 cmd, QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out<<(qint64)0 << quint16(addr)<<quint16(cmd)<<data;
    out.device()->seek(0);
    out<<(qint64)(msg.size()-sizeof(qint64));
    this->write(msg);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      发送文件头
******************************************************************************/
void MyTcpThread::PutFileHead(QByteArray data)
{
    QString name = data;
    file = new QFile(name);
    if (!file->open(QFile::ReadOnly)) {
        PutBlock(ADDR,HeadError,file->errorString().toUtf8());
        qDebug() << "open file error!" << file->errorString();
        return;
    }
    QByteArray msg;
    msg.append(QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5));
    file->seek(0);
    msg.append(QString(" %1 ").arg(file->size()));
    msg.append(name.right(name.size()- name.lastIndexOf('/')-1));
    PutBlock(ADDR,HeadMsg,msg);
    BytesToWrite = file->size();
    BytesWritten = 0;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      发送文件
******************************************************************************/
void MyTcpThread::PutFileData(qint64 numBytes)
{
    if (BytesToWrite == 0)
        return;
    BytesWritten += (int)numBytes;
    PutBlock(quint16(ADDR),quint16(DataMsg),file->read(LoadSize));
    BytesToWrite -= (int)qMin(BytesToWrite,LoadSize);
    if (BytesToWrite == 0)
        file->close();
    count = 0;
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      命令执行
******************************************************************************/
void MyTcpThread::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray data)
{
//    qDebug()<<addr<<cmd<<data;
    timer->stop();
    count = 0;
    switch (cmd) {
    case ClientLogin:
        InitString.clear();
        InitString.append("AIP00001 ");
        InitString.append(getHardwareAddress());
        InitString.append(" V-1.1");
        PutBlock(ADDR,ClientLogin,InitString.toUtf8());
        Display("连接服务器成功\n");
        break;
    case ClientLoginError:
        Display(data);
        break;
    case LocalLogin:
        InitString.clear();
        InitString.append(User);
        InitString.append(" ");
        InitString.append(getHardwareAddress());
        InitString.append(" ");
        InitString.append(Version);
        PutBlock(ADDR,LocalLogin,InitString.toUtf8());
        Display("连接服务器成功\n");
        Display("验证用户名和密码...\n");
        break;
    case LocalLoginError:
        emit TransformCmd(addr,cmd,data);
        break;
    case LocalLoginSuccess:
        emit TransformCmd(addr,cmd,data);
        break;
    case ServerGetHead:
        PutFileHead(data);
        break;
    case ClientGetHead:
        qDebug()<<"ClientGetHead"<<data;
        this->PutBlock(ADDR,ClientGetHead,data);
        break;
    case HeadMsg:
        GetFileHead(addr,data);
        break;
    case HeadError:
        Display(data);
        break;
    case DataMsg:
        GetFileData(addr,data);
        break;
    case DataError:
        Display(data);
        break;
    case CmdMsg:
        proc->start(data);
        proc->waitForFinished();
        PutBlock(quint16(addr),CmdSuccess,proc->readAll());
        break;
    case CmdSuccess:
        Display(data);
        break;
    case ListMsg:
        emit TransformCmd(addr,cmd,data);
        break;
    case ListError:
        break;
    case SocketDisplay:
        emit TransformCmd(addr,cmd,data);
        break;
    default:
        qDebug()<<addr<<cmd<<data;
        break;
    }
    timer->start(10000);
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      获取硬件地址
******************************************************************************/
QString MyTcpThread::getHardwareAddress()
{
    int i;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (i=0; i<list.size(); i++) {
        if (list[i].hardwareAddress().size() == 17 && list[i].hardwareAddress() != "00:00:00:00:00:00")
            return list[i].hardwareAddress();
    }
    return list[0].hardwareAddress();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      输出错误信息
******************************************************************************/
void MyTcpThread::Error(QAbstractSocket::SocketError socketError)
{
    Display(this->errorString().toUtf8());
    if (socketError == QAbstractSocket::RemoteHostClosedError)
        return;
    qDebug()<<"error:"<<this->errorString(); //输出错误信息
    this->close();
}
/******************************************************************************
  * version:    1.0
  * author:     link
  * date:       2016.07.16
  * brief:      显示
******************************************************************************/
void MyTcpThread::Display(QByteArray msg)
{
    if (TramsmitAddr != this->peerPort())
        emit TransformCmd(TramsmitAddr,SocketDisplay,msg);
    else
        emit TransformCmd(ADDR,SocketDisplay,msg);
}
