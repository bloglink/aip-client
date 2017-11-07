/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       筛选输入框
*******************************************************************************/
#include "lqlineedit.h"

LQLineEdit::LQLineEdit(QWidget *parent) : QLineEdit(parent)
{
    initUI();
}

void LQLineEdit::clear()
{
    QLineEdit::clear();
    delayedSignalTimerTriggered();
}

void LQLineEdit::setText(const QString &text)
{
    QLineEdit::setText(text);
    delayedSignalTimerTriggered();
}

void LQLineEdit::initUI()
{
    setPlaceholderText(tr("过滤"));
//    setAlignment(Qt::AlignHCenter);
    setClearButtonEnabled(true);
    delaySignalTimer = new QTimer(this);
    delaySignalTimer->setInterval(200);
    connect(this, SIGNAL(textChanged(QString)), delaySignalTimer, SLOT(start()));
    connect(delaySignalTimer, SIGNAL(timeout()), this, SLOT(delayedSignalTimerTriggered()));
    connect(this, SIGNAL(editingFinished()), this, SLOT(delayedSignalTimerTriggered()));
}

void LQLineEdit::delayedSignalTimerTriggered()
{
    delaySignalTimer->stop();
    if (text() != lastValue) {
        emit delayedTextChanged(text());
        lastValue = text();
    }
}
