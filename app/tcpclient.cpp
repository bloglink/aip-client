#include "tcpclient.h"

TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
{
    initUI();
    initSocket();
}

TcpClient::~TcpClient()
{
}

void TcpClient::initUI()
{
    initSkin();
    initPopup();
    initLayout();
    initSignIn();
    initSystem();
    initVersion();
}

void TcpClient::initSkin()
{
    QFile file;
    QString qss;
    file.setFileName("./skins/soft_rain.qss");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);
}

void TcpClient::initPopup()
{
    popup = new LQMessageBox(this, "", "", QMessageBox::Ok);
    popup->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup);
    popup->setStyleSheet("QDialog{border:2px solid cyan;}");
}

void TcpClient::initLayout()
{
    stack = new QStackedWidget(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(stack);

    this->setLayout(layout);
}

void TcpClient::initSignIn()
{
    signin = new SignIn(this);
    signin->setObjectName("signin");
    connect(signin, SIGNAL(sendAppMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    stack->addWidget(signin);
}

void TcpClient::initSystem()
{
    system = new System(this);
    system->setObjectName("system");
    connect(this, SIGNAL(sendAppShow(QString)), system, SLOT(recvAppShow(QString)));
    connect(system, SIGNAL(sendAppShow(QString)), this, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), system, SLOT(recvSqlOpen(QString)));
    connect(system, SIGNAL(sendNetMsg(QJsonObject)), this, SLOT(recvAppMsg(QJsonObject)));
    connect(this, SIGNAL(sendDisplay(QString)), system, SIGNAL(sendDisplay(QString)));
    stack->addWidget(system);
}

void TcpClient::initSocket()
{
    tcp = new TcpSocket(this);
    connect(tcp, SIGNAL(command(quint16, quint16, QByteArray)),
            this, SLOT(recvNetMsg(quint16,quint16,QByteArray)));
}

void TcpClient::initVersion()
{
    this->setWindowTitle("二代网络客户端试用版V0.1");
}

void TcpClient::initSqlite(QByteArray msg)
{
    QJsonObject sql = QJsonDocument::fromJson(msg).object();
    db = QSqlDatabase::addDatabase("QMYSQL3", "mysql");
    db.setHostName(sql.value("hostname").toString());
    db.setUserName(sql.value("username").toString());
    db.setPassword(sql.value("password").toString());
    db.setDatabaseName(sql.value("database").toString());
    if (!db.open()) {
        qDebug() << "open fail" << db.lastError();
    }
    emit sendSqlOpen("mysql");
    qDebug() << "sql" << sql;
    recvAppShow("system");
}

void TcpClient::recvAppShow(QString sourceName)
{
    for (int i=0; i < stack->count(); i++) {
        if (stack->widget(i)->objectName() == sourceName) {
            stack->setCurrentIndex(i);
            emit sendAppShow(sourceName);
            break;
        }
    }
}

void TcpClient::recvAppMsg(QJsonObject msg)
{
    int cmd = msg.value("txCommand").toInt();
    int addr = msg.value("txAddress").toInt();
    QByteArray dat = msg.value("txMessage").toString().toUtf8();
    switch (cmd) {
    case CMD_SIGNIN:
        tcp->connectToServer(msg);
        system->setMasterName(msg.value("username").toString());
        break;
    case ONLINE_SHELL_CMD:
        tcp->sendSocket(addr, SHELL_CMD, dat);
        break;
    case ONLINE_SEND_HAED:
        tcp->sendSocket(addr, SEND_HEAD, dat);
        break;
    case ONLINE_READ_HAED:
        tcp->sendSocket(addr, READ_HEAD, dat);
        break;
    default:
        break;
    }
}

void TcpClient::recvNetMsg(quint16 addr, quint16 cmd, QByteArray msg)
{
    switch (cmd) {
    case SIGN_OK:
        initSqlite(QByteArray::fromBase64(msg));
        break;
    case SHOW:
    case SHELL_DAT:
    case HEAD_FAIL:
        emit sendDisplay(msg);
        break;
    default:
        qDebug() << addr << cmd << msg;
        break;
    }
}
