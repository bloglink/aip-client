/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       筛选输入框
*******************************************************************************/
#ifndef LQLINEEDIT_H
#define LQLINEEDIT_H

#include <QTimer>
#include <QWidget>
#include <QLineEdit>

class LQLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LQLineEdit(QWidget *parent = 0);

    void clear();
    void setText(const QString& text);

private slots:
    void initUI();
    void delayedSignalTimerTriggered();

signals:
    void delayedTextChanged(QString text);

private:
    QTimer* delaySignalTimer;
    QString lastValue;
};

#endif // LQLINEEDIT_H
