/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:
*******************************************************************************/
#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
    heart = 0;
    LoadSize = 4*1024;;
    BlockSize = 0;
    recvByte = 0;
    recvFile = 0;
    sendFile = 0;
    sendByte = 0;
    file = NULL;

    connect(this, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(sendFileData(qint64)));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(Error(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(connected()), this, SLOT(newSignIn()));

    QDir dir;
    dir.mkdir(QString("temp"));
    dir.mkdir(QString("network"));
}

void TcpSocket::connectToServer(QJsonObject msg)
{
    sign = msg;
    QString hostaddr = msg.value("hostaddr").toString();
    quint16 hostport = msg.value("hostport").toString().toInt();
    this->connectToHost(hostaddr, hostport);
}

void TcpSocket::readSocket()
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
        ExcuteCmd(addr, cmd, msg);
        BlockSize = 0;
    }
}

void TcpSocket::recvFileHead(QByteArray msg)
{
    GetFileMD5 = msg.mid(0, 16);
    QString temp = msg.remove(0, 17);
    QStringList detail = temp.split(" ");
    recvFile = detail.at(0).toInt();
    fileName = detail.at(1).toUtf8();
    recvByte = 0;
    file = new QFile(QString("./temp/").append(fileName));
    if (!file->open(QFile::ReadWrite)) {
        sendSocket(ADDR, FILE_FAIL, file->errorString().toUtf8());
    }
}

void TcpSocket::recvFileData(QByteArray msg)
{
    if (file == NULL)
        return;
    if (!file->isOpen())
        return;

    int ret = file->write(msg);

    recvByte += ret;
    emit display(QString("服务器已接收%1%").arg(recvByte*100/recvFile).toUtf8());
    if (recvByte >= recvFile) {
        file->seek(0);
        QByteArray md5 = QCryptographicHash::hash(file->readAll(), QCryptographicHash::Md5);
        recvByte = 0;
        recvFile = 0;
        if (GetFileMD5 == md5) {
            QProcess::execute("sync");
            QString cmd = QString("mv %1 %2").arg(file->fileName()).arg("./network/");
            QProcess::execute(cmd);
            emit display("服务器接收成功");
        } else {
            file->close();
            file->remove();
            emit display("服务器接收失败");
        }
    }
}

void TcpSocket::sendSocket(quint16 addr, quint16 cmd, QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << (qint64)0 << quint16(addr) << quint16(cmd) << data;
    out.device()->seek(0);
    out << (qint64)(msg.size()-sizeof(qint64));
    this->write(msg);
}

void TcpSocket::sendFileHead(QByteArray data)
{
    QString name = data;
    file = new QFile(name);
    if (!file->open(QFile::ReadOnly)) {
        emit display(file->errorString().toUtf8());
        return;
    }
    QByteArray md5;
    md5 = QCryptographicHash::hash(file->readAll(), QCryptographicHash::Md5);
    file->seek(0);
    sendFile = file->size();
    sendByte = 0;
    PutFileName = name.right(name.size()- name.lastIndexOf('/')-1).toUtf8();
    QByteArray msg;
    msg.append(md5);
    msg.append(QString(" %1 ").arg(sendFile));
    msg.append(PutFileName);
    sendSocket(ADDR, FILE_HEAD, msg);
}

void TcpSocket::sendFileData(qint64)
{
    if (sendFile == 0)
        return;
    sendSocket(ADDR, FILE_DATA, file->read(LoadSize));
    sendByte += qMin(sendFile, LoadSize);
    sendFile -= qMin(sendFile, LoadSize);
    emit display(QString("服务器已发送%1%").arg(sendByte*100/(sendFile+sendByte)).toUtf8());
    heart = 0;
    if (sendFile == 0) {
        file->close();
    }
}

void TcpSocket::ExcuteCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    heart = 0;
    if (addr != ADDR) {
        emit command(addr, cmd, msg);
        return;
    }
    switch (cmd) {
    case SIGN_IN:           // 登录
        emit signin(msg);
        break;
    case FILE_OVER:           // 发送成功
        emit display(tr("服务器发送成功"));
        break;
    case FILE_FAIL:           // 发送失败
        emit display(msg);
        break;
    case READ_HEAD:    // 客户获取文件
        sendFileHead(msg);
        break;
    case FILE_HEAD:         // 收取文件头
        recvFileHead(msg);
        break;
    case FILE_DATA:         // 获取文件内容
        recvFileData(msg);
        break;
    case BEAT:        // 心跳
        sendSocket(ADDR, BEAT, NULL);
        break;
    default:
        emit command(addr, cmd, msg);
        break;
    }
}

void TcpSocket::Error(QAbstractSocket::SocketError socketError)
{
    if (socketError == QAbstractSocket::RemoteHostClosedError)
        return;
    emit display(errorString().toUtf8());
    this->close();
}

void TcpSocket::newSignIn()
{
    QByteArray msg = QJsonDocument(sign).toJson();
    qDebug() << "connected";
    this->sendSocket(ADDR, SIGN_IN, msg.toBase64());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(heartBeat()));
    timer->start(5000);
}

void TcpSocket::heartBeat()
{
    sendSocket(ADDR, BEAT, "");
}
