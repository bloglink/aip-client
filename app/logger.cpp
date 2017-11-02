/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       日志设置
*******************************************************************************/
#include "logger.h"

Logger *logger;
Logger *Logger::_instance = 0;  // 初始化静态变量

Logger *Logger::instance()  // 构建单实例日志对象
{
    if (!Logger::_instance)
        Logger::_instance = new Logger;
    return Logger::_instance;
}

void outputHandle(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (!logger) {
        logger = Logger::instance();
    }
    if (logger) {
        logger->output(type, context, msg);
    }
}

Logger::Logger(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSetting();
}

void Logger::output(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message;
    QString newLine = "\r\n";
#ifdef __linux__
    newLine = "\n";
#endif
    switch (type) {
    case QtDebugMsg:
        message = QString("调试信息: ");
        break;
    case QtWarningMsg:
        message = QString("警告信息: ");
        break;
    case QtCriticalMsg:
        message = QString("严重错误: ");
        break;
    case QtFatalMsg:
        message = QString("致命错误: ");
        break;
    case QtInfoMsg:
        message = QString("常规信息: ");
        break;
    default:
        break;
    }

    QString tstring = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString lstring = QString::number(context.line, 10);
    message.append(QString("[%1] ").arg(tstring));
    message.append(QString("%1 (line: %2) ").arg(context.file).arg(lstring));
    message.append(QString("%1 ").arg(context.function));
    message.append(QString("%1 %2").arg(msg).arg(newLine));

    if (combo->currentIndex() == 1) {
        text->insertPlainText(message);
    } else if (combo->currentIndex() == 2) {
        QDir dir;
        dir.mkdir("log");
        QString name = QString("log/%1.txt").arg(QDate::currentDate().toString("yyyy-MM-dd"));
        QFile log(name);
        log.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream cout(&log);
        cout << message;
        log.flush();
        log.close();
    }
}

void Logger::initUI()
{
    initText();
    initButton();
    initLayout();
}

void Logger::initText()
{
    text = new QTextBrowser(this);
}

void Logger::initButton()
{
    btnLayout = new QHBoxLayout;
    btnLayout->addStretch();

    combo = new QComboBox(this);
    combo->setFixedSize(120, 44);
    combo->setView(new QListView);
    combo->addItem(tr("默认输出"));
    combo->addItem(tr("输出到窗口"));
    combo->addItem(tr("输出到文件"));
    btnLayout->addWidget(combo);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeHandle(int)));

    QPushButton *btnClear = new QPushButton(tr("清空显示"), this);
    btnClear->setFixedSize(97, 44);
    btnLayout->addWidget(btnClear);
    connect(btnClear, SIGNAL(clicked(bool)), text, SLOT(clear()));

    QPushButton *btnSave = new QPushButton(tr("保存设置"), this);
    btnSave->setEnabled(false);
    btnSave->setFixedSize(97, 44);
    btnLayout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(readSetting()));
}

void Logger::initLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(text);
    layout->addLayout(btnLayout);
    this->setLayout(layout);
}

void Logger::initSetting()
{
#ifdef __linux__
    combo->setCurrentIndex(0);
#else
    combo->setCurrentIndex(2);
#endif
}

void Logger::readSetting()
{
}

void Logger::changeHandle(int t)
{
    switch (t) {
    case 0:
        qInstallMessageHandler(0);
        break;
    case 1:
        qInstallMessageHandler(outputHandle);
        break;
    case 2:
        qInstallMessageHandler(outputHandle);
        break;
    default:
        break;
    }
}

void Logger::recvAppShow(QString win)
{
    if (win == this->objectName())
        return;
}

void Logger::recvSqlOpen(QString sql)
{
    sqlName = sql;
}
