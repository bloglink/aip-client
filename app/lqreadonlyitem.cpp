/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       只读委托
*******************************************************************************/
#include "lqreadonlyitem.h"

LQReadOnlyItem::LQReadOnlyItem(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *LQReadOnlyItem::createEditor(QWidget *,
                                      const QStyleOptionViewItem &,
                                      const QModelIndex &) const {
    return NULL;
}

