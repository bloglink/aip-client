/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       QDoubleSpinBox代理
*******************************************************************************/
#ifndef DDOUBLESPINBOX_H
#define DDOUBLESPINBOX_H

#include <QObject>
#include <QModelIndex>
#include <QDoubleSpinBox>
#include <QItemDelegate>
#include <QStyledItemDelegate>

class LQDoubleSpinBox : public QItemDelegate
{
    Q_OBJECT

public:
    explicit LQDoubleSpinBox(QObject *parent = 0);

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
    void setMaxinum(double m);
    void setMininum(double m);
    void setDecimals(int prec);
private:
    double max;
    double min;
    int prec;
};

#endif // DDOUBLESPINBOX_H
