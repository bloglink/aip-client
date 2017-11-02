/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       视图委托
*******************************************************************************/
#ifndef LQSTANDARDITEM_H
#define LQSTANDARDITEM_H

#include <QDebug>
#include <QObject>
#include <QStandardItemModel>

class LQStandardItem : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit LQStandardItem(QObject *parent = 0);
    explicit LQStandardItem(int row, int column, QObject *parent = NULL);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setCheckable(int column);
private:
    QList<int> columns;
    QMap<int, Qt::CheckState> checks;
};

#endif // LQSTANDARDITEM_H
