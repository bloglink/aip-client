/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       登录界面
*******************************************************************************/
#ifndef SIGNIN_H
#define SIGNIN_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QListView>
#include <QComboBox>
#include <QLineEdit>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>

#include <QJsonObject>

#include <QApplication>

#define MAX_RECORD 5
#define CMD_SIGNIN 100

class SignIn : public QWidget
{
    Q_OBJECT
public:
    explicit SignIn(QWidget *parent = 0);
signals:
    void sendAppMsg(QJsonObject msg);
private slots:
    void initUI();
    void initLayout();
    void initTitleBar();
    void initContent();
    void initInputBar();
    void initSettings();
    void saveSettings();
    void saveHostAddr();
    void saveHostPort();
    void saveUserName();
    void sendLogin();
    void sendClose();
private:
    QVBoxLayout *layout;
    QComboBox *hostaddr;
    QComboBox *hostport;
    QComboBox *username;
    QLineEdit *password;
    QSettings *userConf;
};

#endif // SIGNIN_H
