/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       视图委托
*******************************************************************************/
#include "lqstandarditem.h"

LQStandardItem::LQStandardItem(QObject *parent) : QStandardItemModel(parent)
{
}

bool LQStandardItem::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (role == Qt::CheckStateRole && columns.contains(index.column())) {
        checks[index.row()] = (value == Qt::Checked ? Qt::Checked : Qt::Unchecked);
        return true;
    } else {
        return QStandardItemModel::setData(index, value, role);
    }
}

QVariant LQStandardItem::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
    case Qt::CheckStateRole:
        if (columns.contains(index.column())) {
            if (checks.contains(index.row())) {
                return checks[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked;
            }
            return Qt::Unchecked;
        }
        break;
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
        break;
    default:
        break;
    }
    return QStandardItemModel::data(index, role);
}

Qt::ItemFlags LQStandardItem::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    if (columns.contains(index.column()))
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return QStandardItemModel::flags(index);
    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void LQStandardItem::setCheckable(int column)
{
    columns.append(column);
}

