/*******************************************************************************
 * Copyright [2016]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       日志设置
*******************************************************************************/
#ifndef LOGGER_H
#define LOGGER_H

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QListView>
#include <QDateTime>
#include <QComboBox>
#include <QPushButton>
#include <QTextStream>
#include <QTextBrowser>

class Logger : public QWidget
{
    Q_OBJECT
public:
    static Logger *_instance;
    static Logger *instance();
    explicit Logger(QWidget *parent = 0);

signals:

public slots:
    void changeHandle(int t);
    void output(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private slots:
    void initUI();
    void initText();
    void initButton();
    void initLayout();
    void initSetting();
    void readSetting();
    void recvAppShow(QString win);
    void recvSqlOpen(QString sql);

private:
    QComboBox *combo;
    QTextBrowser *text;

    QString sqlName;

    QHBoxLayout *btnLayout;
};

#endif // LOGGER_H
