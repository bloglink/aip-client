#ifndef WINCLIENT_H
#define WINCLIENT_H

#include <QFile>
#include <QThread>
#include <QWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QButtonGroup>
#include <QTableWidgetItem>

#include "MyTcpThread.h"

#define W_ROW 250
#define W_COL 6

namespace Ui {
class WinClient;
}

class WinClient : public QWidget
{
    Q_OBJECT

public:
    explicit WinClient(QWidget *parent = 0);
    ~WinClient();

private:
    Ui::WinClient *ui;

signals:
    void PutBlock(quint16 addr,quint16 cmd,QByteArray data);
    void PutFileHead(QByteArray data);
public slots:

private slots:
    void WinInit(void);
    void WinUpdate(QByteArray msg);
    void KeyInit(void);
    void DatInit(void);
    void DatSave(void);

    void TcpInit(void);
    void TcpStart(void);
    void TcpLogin(void);
    void TcpLogout(void);
    void TcpGetListMsg(void);
    void TcpFileLocalPut(void);
    void TcpFileLocalGet(void);
    void TcpFileClientPut(void);
    void TcpFileClientGet(void);
    void TcpCmdSystem(void);
    void TcpDisplay(QString msg);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void closeEvent(QCloseEvent *e);
private:
    QThread *tcp;
    QTimer *timer;
    QSettings *set;
};

#endif // WINCLIENT_H
