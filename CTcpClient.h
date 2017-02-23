#ifndef CTCPCLIENT_H
#define CTCPCLIENT_H

#include <QUrl>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QHostInfo>
#include <QTcpSocket>
#include <QDataStream>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QFileInfo>
#include <QDateTime>

#include "define.h"
#define ADDR 6000
#define GUEST_LOGIN             1000 //登录
#define GUEST_LOGIN_ERROR       1001 //登录出错
#define GUEST_PUT_HEAD          1002 //服务器获取文件信息
#define GUEST_GET_HEAD          1003 //客户端获取文件信息
#define GUEST_DISPLAY           1004 //显示信息
#define GUEST_DROPED            1005 //客户端下线
#define ADMIN_LOGIN             1006 //客户端登录
#define ADMIN_LOGIN_ERROR       1007 //登录出错
#define ADMIN_LOGIN_SUCCESS     1008 //登录成功
#define BUILD_TRANSMIT          1102 //建立转发
#define BREAK_TRANSMIT          1103 //取消转发
#define FILE_HEAD               2000 //文件头
#define FILE_DATA               2001 //文件内容
#define FILE_SUCCESS            2002 //文件发送成功
#define FILE_ERROR              2003 //文件内容错误
#define FILE_HEAD_ERROR         2009 //文件头错误
#define SHELL_CMD               2004 //发送系统命令
#define SHELL_DAT               2005 //系统命令返回
#define ONLINE_DEVICES          2006 //在线设备列表
#define HEART_BEAT              6000 //心跳

class CTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CTcpClient(QObject *parent = 0);

signals:
    void TransformCmd(quint16 addr,quint16 Process,QByteArray data);
public slots:
    void ConnectToHost(void);
private slots:
    void Disconnect(void);
    void Connected(void);
    void KeepAlive(void);
    void GetBlock(void);
    void PutBlock(quint16 addr,quint16 cmd,QByteArray data);
    void ExcuteCmd(QByteArray msg);
    void ProcessStart(QUrl url);
    void ProcessReady(void);
    QString HardwareAddress(void);

private:
    QTimer *timer;
    QProcess *Process;
    QSettings *set;
    QUrl UrlInit;

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

#endif // CTCPCLIENT_H
