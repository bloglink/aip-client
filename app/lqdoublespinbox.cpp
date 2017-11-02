/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QDoubleSpinBox代理
*******************************************************************************/
#include "lqdoublespinbox.h"

LQDoubleSpinBox::LQDoubleSpinBox(QObject *parent):QItemDelegate(parent)
{
    min = 0;
    max = 9999.99;
    prec = 2;
}

QWidget *LQDoubleSpinBox::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &,
                                       const QModelIndex &) const {
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMaximum(max);
    editor->setMinimum(min);
    editor->setDecimals(prec);
    editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
    return editor;
}

void LQDoubleSpinBox::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void LQDoubleSpinBox::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const {
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void LQDoubleSpinBox::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &) const {
    editor->setGeometry(option.rect);
}

void LQDoubleSpinBox::setMaxinum(double m)
{
    max = m;
}

void LQDoubleSpinBox::setMininum(double m)
{
    min = m;
}

void LQDoubleSpinBox::setDecimals(int p)
{
    prec = p;
}


