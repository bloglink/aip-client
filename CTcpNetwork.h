#ifndef CNETWORK_H
#define CNETWORK_H

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

class CTcpNetwork : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CTcpNetwork(QObject *parent = 0);

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

#endif // CNETWORK_H
