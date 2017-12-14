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
    TramsmitAddr = ADDR;

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

void MyTcpThread::TcpInit()
{
    count = 0;
    this->connectToHost("192.168.1.55", 6000);
    timer->start(10000);
}

void MyTcpThread::TcpQuit()
{
    this->close();
}

void MyTcpThread::Connected()
{
    QString s = QString("168912000X %1 V-170328").arg(getHardwareAddress());
    PutBlock(ADDR,GUEST_LOGIN,s.toUtf8());
    PutBlock(ADDR,ONLINE_DEVICES,NULL);
    PutBlock(ADDR,SERVER_FILES,"168912000X");
}

void MyTcpThread::KeepAlive()
{
    if (this->state() != QAbstractSocket::ConnectedState) {
        TcpInit();
    } else {
        PutBlock(ADDR,HEART_BEAT,"NULL");
        count++;
        if (count >6) {
            this->abort();
        }
    }
}

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

void MyTcpThread::GetFileHead(quint16 addr,QByteArray msg)
{
    SendVerify = msg.mid(0,16);
    QString temp = msg.remove(0,17);
    QStringList detail = temp.split(" ");
    BytesToRead = detail.at(0).toInt();
    BytesRead = 0;
    file = new QFile(QString(TMP).append(detail.at(1)));
    if(!file->open(QFile::ReadWrite)) {
        PutBlock(addr,FILE_HEAD_ERROR,file->errorString().toUtf8());
    }
}

void MyTcpThread::GetFileData(quint16 addr,QByteArray msg)
{
    int ret;
    if (file->isOpen())
        ret = file->write(msg);
    else
        return;
    BytesRead += ret;
    Display(QString("已接收%1K").arg(BytesRead/1024).toUtf8());
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
            Display("接收成功");
            PutBlock(addr,FILE_SUCCESS,"NULL");
        } else {
            QString cmd = QString("rm %1").arg(file->fileName());
            QProcess::execute(cmd);
            Display("接收失败");
            PutBlock(addr,FILE_ERROR,"NULL");
        }
        return;
    }
}

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

void MyTcpThread::PutFileHead(QByteArray data)
{
    QString name = data;
    file = new QFile(name);
    if (!file->open(QFile::ReadOnly)) {
        PutBlock(ADDR,FILE_HEAD_ERROR,file->errorString().toUtf8());
        return;
    }
    QByteArray msg;
    msg.append(QCryptographicHash::hash(file->readAll(),QCryptographicHash::Md5));
    file->seek(0);
    msg.append(QString(" %1 ").arg(file->size()));
    msg.append(name.right(name.size()- name.lastIndexOf('/')-1));
    PutBlock(ADDR,FILE_HEAD,msg);
    BytesToWrite = file->size();
}

void MyTcpThread::PutFileData(qint64 )
{
    if (BytesToWrite == 0)
        return;
    PutBlock(quint16(ADDR),quint16(FILE_DATA),file->read(LoadSize));
    BytesToWrite -= (int)qMin(BytesToWrite,LoadSize);
    Display(QString("剩余%1K").arg(BytesToWrite/1024).toUtf8());
    if (BytesToWrite == 0)
        file->close();
    count = 0;
}

void MyTcpThread::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray data)
{
    timer->stop();
    count = 0;
    switch (cmd) {
    case ONLINE_DEVICES:
        emit SendCommand(addr,cmd,data);
        break;
    case SHELL_CMD:
        proc->start(data);
        proc->waitForFinished();
        PutBlock(quint16(addr),SHELL_DAT,proc->readAll());
        break;
    case SHELL_DAT:
    case SERVER_FILES:
        emit SendCommand(addr,cmd,data);
        break;
    case GUEST_PUT_HEAD:
        TramsmitAddr = addr;
        PutFileHead(data);
        break;
    case GUEST_GET_HEAD:
        TramsmitAddr = addr;
        PutBlock(ADDR,GUEST_GET_HEAD,data);
        break;
    case GUEST_DISPLAY:
        emit SendCommand(ADDR,GUEST_DISPLAY,data);
        break;
    case FILE_SUCCESS:
        Display("发送成功");
        if (TramsmitAddr != ADDR)
            PutBlock(TramsmitAddr,FILE_SUCCESS,NULL);
        else
            PutBlock(ADDR,SERVER_FILES,"168912000X");
        break;
    case FILE_HEAD:
        GetFileHead(addr,data);
        break;
    case FILE_DATA:
        GetFileData(addr,data);
        break;
    case HEART_BEAT:
        break;
    default:
        qDebug()<<addr<<cmd<<data;
        break;
    }
    timer->start(10000);
}

QString MyTcpThread::getHardwareAddress()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (int i=0; i<list.size(); i++) {
        if (list[i].hardwareAddress() == "00:00:00:00:00:00")
            continue;
        if (list[i].hardwareAddress().size() == 17)
            return list[i].hardwareAddress();
    }
    return "00:00:00:00:00:00";
}

void MyTcpThread::Error(QAbstractSocket::SocketError socketError)
{
    Display(this->errorString().toUtf8());
    if (socketError == QAbstractSocket::RemoteHostClosedError)
        return;
    this->close();
}

void MyTcpThread::Display(QByteArray msg)
{
    if (TramsmitAddr != ADDR)
        PutBlock(TramsmitAddr,GUEST_DISPLAY,msg);
    else
        emit SendCommand(ADDR,GUEST_DISPLAY,msg);
}
