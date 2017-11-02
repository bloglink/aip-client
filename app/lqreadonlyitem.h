/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       只读委托
*******************************************************************************/
#ifndef LQREADONLYITEM_H
#define LQREADONLYITEM_H

#include <QObject>
#include <QItemDelegate>

class LQReadOnlyItem : public QItemDelegate
{
    Q_OBJECT
public:
    explicit LQReadOnlyItem(QObject *parent = 0);

    QWidget *createEditor(QWidget*, const QStyleOptionViewItem &,
                          const QModelIndex &) const;
};

#endif // LQREADONLYITEM_H
