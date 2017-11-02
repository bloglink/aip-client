/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QSpinBox代理
*******************************************************************************/
#ifndef DSPINBOX_H
#define DSPINBOX_H

#include <QObject>
#include <QSpinBox>
#include <QModelIndex>
#include <QItemDelegate>
#include <QStyledItemDelegate>

class LQSpinBox : public QItemDelegate
{
    Q_OBJECT

public:
    explicit LQSpinBox(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
public slots:
    void setMaxinum(int m);
    void setMininum(int m);
private:
    int max;
    int min;
};

#endif // DSPINBOX_H
