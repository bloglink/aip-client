/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       警告弹窗
*******************************************************************************/
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QEvent>
#include <QApplication>
#include <QDebug>
#include "lqmessagebox.h"

LQMessageBox::LQMessageBox(QWidget  *parent, const QString &title, const QString &text,
                               QMessageBox::StandardButtons buttons,
                               QMessageBox::StandardButton defaultButton)
    : QDialog(parent, Qt::WindowStaysOnTopHint)
{
    setWindowIcon(QIcon("./image/link.png"));
    setWindowTitle(title);
    setMinimumSize(400, 130);

    ButtonBox = new QDialogButtonBox(this);
    ButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(buttons)) |
                                  QDialogButtonBox::StandardButtons(int(defaultButton)));

    QLabel *IconLabel = new QLabel(this);
    IconLabel->setPixmap(QPixmap("./image/logo.png"));
    IconLabel->setFixedSize(113, 57);
    IconLabel->setScaledContents(true);

    TextLabel = new QLabel(this);
    TextLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    TextLabel->setStyleSheet("color:yellow;font:15pt");
    TextLabel->setWordWrap(true);
    TextLabel->setText(text);

    QGridLayout *GridLayout = new QGridLayout(this);
    GridLayout->addWidget(IconLabel, 0, 0, 2, 1, Qt::AlignTop);
    GridLayout->addWidget(TextLabel, 0, 1, 2, 1);
    GridLayout->addWidget(ButtonBox, GridLayout->rowCount(), 0, 1, GridLayout->columnCount());

    GridLayout->setSizeConstraint(QLayout::SetNoConstraint);
    GridLayout->setHorizontalSpacing(10);
    GridLayout->setVerticalSpacing(10);
    GridLayout->setContentsMargins(10, 10, 10, 10);
    this->setLayout(GridLayout);

    translateButtonText();

    connect(ButtonBox, SIGNAL(clicked(QAbstractButton*)), this,
            SLOT(onButtonClicked(QAbstractButton*)));
}

LQMessageBox::~LQMessageBox()
{
}

void LQMessageBox::setText(QString s)
{
    TextLabel->setText(s);
}

void LQMessageBox::changeEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::LanguageChange:
        translateButtonText();
        break;
    default:
        QDialog::changeEvent(event);
        break;
    }
}

void LQMessageBox::translateButtonText()
{
    QPushButton *RetryButton = ButtonBox->button(QDialogButtonBox::Retry);
    if (RetryButton != NULL)
        RetryButton->setText(tr("停止"));

    QPushButton *CancelButton = ButtonBox->button(QDialogButtonBox::Yes);
    if (CancelButton != NULL)
        CancelButton->setText(tr("确定"));
    QPushButton *OkButton = ButtonBox->button(QDialogButtonBox::Ok);
    if (OkButton != NULL)
        OkButton->setText(tr("继续"));
}

void LQMessageBox::onButtonClicked(QAbstractButton *button)
{
    if (!ButtonBox->buttons().contains(button))
        return;
    done(ButtonBox->standardButton(button));
}
