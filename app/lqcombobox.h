/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QComboBox代理
*******************************************************************************/
#ifndef DCOMBOBOX_H
#define DCOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include <QModelIndex>
#include <QItemDelegate>
#include <QStyledItemDelegate>

class LQComboBox : public QItemDelegate
{
    Q_OBJECT

public:
    explicit LQComboBox(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QStringList items;
public slots:
    void setItemNames(QStringList names);
};

#endif // DCOMBOBOX_H
