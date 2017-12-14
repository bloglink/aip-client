#ifndef WINCLIENT_H
#define WINCLIENT_H

#include <QFile>
#include <QThread>
#include <QWidget>
#include <QSettings>
#include <QCloseEvent>
#include <QButtonGroup>
#include <QTableWidgetItem>
#include <QFileSystemModel>
#include <QUrlQuery>
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
    void InitWindows(void);
    void WinUpdate(QByteArray msg);
    void InitButtons(void);
    void InitClient(void);
    void Login(void);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void SendShellCommand(void);
    void ReadServerFiles(QByteArray msg);
    void ReadClientFiles(QByteArray msg);
    void SendLocalFile(void);
    void SendServerFile(void);
    void SendClientFile(void);
    void ReadServerFile(void);

    void on_tabTcp_clicked(const QModelIndex &index);
    void on_LocalFiles_clicked(const QModelIndex &index);
    void on_ServerFiles_clicked(const QModelIndex &index);
    void on_ClientFiles_clicked(const QModelIndex &index);

private:
    bool isFiles;
    QThread *tcp;
    QFileSystemModel *files;
};

#endif // WINCLIENT_H
