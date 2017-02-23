#ifndef CWINCLIENT_H
#define CWINCLIENT_H

#include <QUrl>
#include <QHttpPart>
#include <QFile>
#include <QThread>
#include <QWidget>
#include <QListView>
#include <QDirModel>
#include <QTreeWidgetItem>

#include <QSettings>
#include <QHideEvent>
#include <QButtonGroup>
#include <QTableWidgetItem>

#include "CTcpNetwork.h"

#define W_ROW 250
#define W_COL 6

namespace Ui {
class CWinClient;
}

class CWinClient : public QWidget
{
    Q_OBJECT

public:
    explicit CWinClient(QWidget *parent = 0);
    ~CWinClient();

private:
    Ui::CWinClient *ui;
signals:
    void PutBlock(quint16 addr,quint16 cmd,QByteArray data);
    void PutFileHead(QByteArray data);
public slots:

private slots:
    void WinInit(void);
    void BtnInit(void);
    void DatInit(void);
    void DatSave(void);
    void ItemClick(int r,int c);
    void ViewClick(QModelIndex);
    void GetClientFileList(void);
    void ShowFileList(QByteArray msg);

    void TcpInit(void);
    void TcpLogin(void);
    void TcpGetListMsg(void);
    void TcpLocalPut(void);
    void TcpLocalGet(void);
    void TcpClientPut(void);
    void TcpClientGet(void);
    void TcpCommand(void);
    void TcpDisplay(QString msg);
    void ExcuteCmd(quint16 addr,quint16 cmd,QByteArray msg);
    void hideEvent(QHideEvent *e);
private:
    QThread *tcp;
    QTimer *timer;
    QSettings *set;
    bool isList;
    QString CurrentPath;
    QList<QTreeWidgetItem*> Items;
};

#endif // CWINCLIENT_H
