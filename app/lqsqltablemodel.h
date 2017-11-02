/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据库委托
*******************************************************************************/
#ifndef SQLCHECKBOX_H
#define SQLCHECKBOX_H

#include <QDebug>
#include <QObject>
#include <QSqlTableModel>

class LQSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
signals:
    void dataChaged(int index, int row);
public:
    explicit LQSqlTableModel(QObject *parent = 0);
    explicit LQSqlTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setCheckable(int column);
private:
    QList<int> columns;
    QMap<int, Qt::CheckState> checks;
};

#endif // SQLCHECKBOX_H
