/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QComboBox代理
*******************************************************************************/
#include "lqcombobox.h"

LQComboBox::LQComboBox(QObject *parent):QItemDelegate(parent)
{
}

QWidget *LQComboBox::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &,
                                  const QModelIndex &) const {
    QComboBox *editor = new QComboBox(parent);
    editor->setItemDelegate(new QStyledItemDelegate());
    for (int i=0; i < items.size(); i++)
        editor->addItem(items.at(i));
    return editor;
}

void LQComboBox::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int tindex = comboBox->findText(text);
    comboBox->setCurrentIndex(tindex);
}

void LQComboBox::setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString text = comboBox->currentText();
    model->setData(index, text, Qt::EditRole);
}

void LQComboBox::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &) const {
    editor->setGeometry(option.rect);
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->showPopup();
}

void LQComboBox::setItemNames(QStringList names) {
    items = names;
}

