/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统界面
*******************************************************************************/
#include "system.h"

System::System(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

System::~System()
{
}

void System::setMasterName(QString name)
{
    masterName = name;
}

void System::initUI()
{
    initLayout();
    initOnline();
    initDevice();
    initMaster();
    initSource();
    initAction();
    initLogger();
}

void System::initLayout()
{
    stack = new QStackedWidget(this);
    btnLayout = new QVBoxLayout;
    btnLayout->setMargin(0);
    btnLayout->setSpacing(0);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(btnLayout);
    leftLayout->addStretch();
    leftLayout->setMargin(0);

    QWidget *leftWidget = new QWidget(this);
    leftWidget->setObjectName("leftWidget");
    leftWidget->setLayout(leftLayout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(leftWidget);
    layout->addWidget(stack);

    this->setLayout(layout);
    this->setObjectName("system");
}

void System::initOnline()
{
    online = new Online(this);
    online->setObjectName("online");
    connect(this, SIGNAL(sendAppShow(QString)), online, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), online, SLOT(recvSqlOpen(QString)));
    connect(this, SIGNAL(sendDisplay(QString)), online, SLOT(recvDisplay(QString)));
    connect(online, SIGNAL(sendNetMsg(QJsonObject)), this, SIGNAL(sendNetMsg(QJsonObject)));
    stack->addWidget(online);

    QPushButton *btnOnline = new QPushButton(tr("在线设备"), this);
    btnOnline->setFlat(true);
    btnOnline->setCheckable(true);
    btnOnline->setFixedSize(196, 60);
    btnOnline->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnOnline);
    btnLayout->addWidget(btnOnline);
    btnOnline->setObjectName("online");
    connect(btnOnline, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initDevice()
{
    device = new Device(this);
    device->setObjectName("device");
    connect(this, SIGNAL(sendAppShow(QString)), device, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), device, SLOT(recvSqlOpen(QString)));
    stack->addWidget(device);

    QPushButton *btnDevice = new QPushButton(tr("设备管理"), this);
    btnDevice->setFlat(true);
    btnDevice->setCheckable(true);
    btnDevice->setFixedSize(196, 60);
    btnDevice->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnDevice);
    btnLayout->addWidget(btnDevice);
    btnDevice->setObjectName("device");
    connect(btnDevice, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initMaster()
{
    master = new Master(this);
    master->setObjectName("master");
    connect(this, SIGNAL(sendAppShow(QString)), master, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), master, SLOT(recvSqlOpen(QString)));
    stack->addWidget(master);

    QPushButton *btnMaster = new QPushButton(tr("用户管理"), this);
    btnMaster->setFlat(true);
    btnMaster->setCheckable(true);
    btnMaster->setFixedSize(196, 60);
    btnMaster->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnMaster);
    btnLayout->addWidget(btnMaster);
    btnMaster->setObjectName("master");
    connect(btnMaster, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initSource()
{
    source = new Source(this);
    source->setObjectName("source");
    connect(this, SIGNAL(sendAppShow(QString)), source, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), source, SLOT(recvSqlOpen(QString)));
    stack->addWidget(source);

    QPushButton *btnSource = new QPushButton(tr("资源管理"), this);
    btnSource->setFlat(true);
    btnSource->setCheckable(true);
    btnSource->setFixedSize(196, 60);
    btnSource->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnSource);
    btnLayout->addWidget(btnSource);
    btnSource->setObjectName("source");
    connect(btnSource, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initAction()
{
    action = new Action(this);
    action->setObjectName("action");
    connect(this, SIGNAL(sendAppShow(QString)), action, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), action, SLOT(recvSqlOpen(QString)));
    stack->addWidget(action);

    QPushButton *btnAction = new QPushButton(tr("权限管理"), this);
    btnAction->setFlat(true);
    btnAction->setCheckable(true);
    btnAction->setFixedSize(196, 60);
    btnAction->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnAction);
    btnLayout->addWidget(btnAction);
    btnAction->setObjectName("action");
    connect(btnAction, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::initLogger()
{
    logger = Logger::instance();
    logger->setObjectName("logger");
    connect(this, SIGNAL(sendAppShow(QString)), logger, SLOT(recvAppShow(QString)));
    connect(this, SIGNAL(sendSqlOpen(QString)), logger, SLOT(recvSqlOpen(QString)));
    stack->addWidget(logger);

    QPushButton *btnLogger = new QPushButton(tr("调试信息"), this);
    btnLogger->setFlat(true);
    btnLogger->setCheckable(true);
    btnLogger->setFixedSize(196, 60);
    btnLogger->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnLogger);
    btnLayout->addWidget(btnLogger);
    btnLogger->setObjectName("logger");
    connect(btnLogger, SIGNAL(clicked(bool)), this, SLOT(clickButton()));
}

void System::clickButton()
{
    QString sourceName = QObject::sender()->objectName();
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    double actionValue = getAction(masterName, sourceName);
    if (actionValue == Qt::Checked) {
        for (int i=0; i < buttons.size(); i++) {
            if (buttons.at(i)->objectName() != sourceName) {
                buttons.at(i)->setChecked(false);
            }
        }
        for (int i=0; i < stack->count(); i++) {
            if (stack->widget(i)->objectName() == sourceName) {
                stack->setCurrentIndex(i);
                break;
            }
        }
        emit sendAppShow(sourceName);
    } else {
        QMessageBox::warning(this, "", "您无权限进行此操作", QMessageBox::Ok);
        btn->setChecked(false);
    }
}

double System::getAction(QString masterName, QString sourceName)
{
    mMaster->setFilter(QObject::tr("name='%1'").arg(masterName));
    mMaster->select();
    QString masterId = mMaster->index(0, 0).data().toString();
    mSource->setFilter(QObject::tr("name='%1'").arg(sourceName));
    mSource->select();
    QString sourceId = mSource->index(0, 0).data().toString();

    mAction->setFilter(QObject::tr("master_id='%1' and source_id='%2'").
                       arg(masterId).arg(sourceId));
    mAction->select();

    double id = mAction->index(0, 3).data().toDouble();

    return id;
}

void System::recvAppShow(QString win)
{
    if (win == this->objectName()) {
        buttons.at(0)->click();
    }
}

void System::recvSqlOpen(QString sql)
{
    sqlName = sql;

    mAction = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mAction->setTable("aip_action");

    mMaster = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mMaster->setTable("aip_master");

    mSource = new LQSqlTableModel(this, QSqlDatabase::database(sqlName));
    mSource->setTable("aip_source");

    emit sendSqlOpen(sql);
}

