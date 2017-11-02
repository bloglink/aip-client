/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QSpinBox代理
*******************************************************************************/
#include "lqspinbox.h"

LQSpinBox::LQSpinBox(QObject *parent):QItemDelegate(parent)
{
    max = 99;
    min = 0;
}

QWidget *LQSpinBox::createEditor(QWidget *parent,
                                const QStyleOptionViewItem &,
                                const QModelIndex &) const {
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMaximum(max);
    editor->setMinimum(min);
    editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
    return editor;
}

void LQSpinBox::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void LQSpinBox::setModelData(QWidget *editor,
                             QAbstractItemModel *model,
                            const QModelIndex &index) const {
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void LQSpinBox::updateEditorGeometry(QWidget *editor,
                                    const QStyleOptionViewItem &option,
                                     const QModelIndex &) const {
    editor->setGeometry(option.rect);
}

void LQSpinBox::setMaxinum(int m)
{
    max = m;
}

void LQSpinBox::setMininum(int m)
{
    min = m;
}
