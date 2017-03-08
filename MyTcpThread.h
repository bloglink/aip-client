#ifndef MYTCPTHREAD_H
#define MYTCPTHREAD_H

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QHostInfo>
#include <QTcpSocket>
#include <QDataStream>
#include <QApplication>
#include <QNetworkInterface>
#include <QCryptographicHash>

#define NET "./network/"
#define TMP "./temp/"
#define CON "./config/"

#define ADDR 6000
const quint16 ClientLogin      = 1000;
const quint16 ClientLoginError = 1001;
const quint16 ServerGetHead    = 1002;
const quint16 ClientGetHead    = 1003;
const quint16 SocketDisplay    = 1004;
const quint16 ClientDroped     = 1005;
const quint16 LocalLogin       = 1006;
const quint16 LocalLoginError  = 1007;
const quint16 LocalLoginSuccess= 1008;

const quint16 HeadMsg      = 2000;
const quint16 HeadError    = 2009;
const quint16 DataMsg      = 2001;
const quint16 DataError    = 2003;
const quint16 DataSuccess  = 2002;
const quint16 CmdMsg       = 2004;
const quint16 CmdSuccess   = 2005;
const quint16 ListMsg      = 2006;
const quint16 ListError    = 2007;
const quint16 GetHeart     = 6000;

#define LOCAL

class MyTcpThread : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpThread(QObject *parent = 0);

signals:
    void TransformCmd(quint16 addr,quint16 proc,QByteArray data);
private slots:
    void TcpInit(void);
    void TcpQuit(void);
    void Connected(void);
    void KeepAlive(void);
    void GetBlock(void);
    void GetFileHead(quint16 addr,QByteArray msg);
    void GetFileData(quint16 addr,QByteArray msg);
    void PutFileData(qint64 numBytes);
    void PutBlock(quint16 addr,quint16 proc,QByteArray data);
    void PutFileHead(QByteArray data);
    void ExcuteCmd(quint16 addr,quint16 proc,QByteArray data);
    QString getHardwareAddress(void);
    void Error(QAbstractSocket::SocketError);
    void Display(QByteArray msg);
private:
    QTimer *timer;
    QProcess *proc;
    QSettings *set;
    QString InitString;
    QString HostName;
    QString Version;
    QString User;

    QFile *file;
    QString fileName;
    QString fileDetail;

    quint16 TramsmitAddr;

    qint64 count;
    qint64 LoadSize;
    qint64 BlockSize;
    qint64 BytesRead;
    qint64 BytesToRead;
    qint64 BytesToWrite;
    qint64 BytesWritten;
    QByteArray SendVerify;
    QByteArray RecvVerify;
};

#endif // MYTCPTHREAD_H
