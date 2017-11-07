/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       筛选输入表头
*******************************************************************************/
#ifndef LQHEADVIEW_H
#define LQHEADVIEW_H

#include <QDebug>
#include <QObject>
#include <QHeaderView>
#include <QApplication>

#include "lqlineedit.h"

class LQHeadView : public QHeaderView
{
    Q_OBJECT
public:
    explicit LQHeadView(Qt::Orientation orientation, QWidget *parent = 0);

signals:
    void filterChanged(int column, QString value);
public slots:
    void createFilters(int number, bool showFirst = false);
private slots:
    void initUI();
    void adjustPositions();
    void inputChanged(QString msg);
protected:
    virtual QSize sizeHint() const;
    virtual void updateGeometries();

private:
    QList<LQLineEdit*> filterWidgets;
};

#endif // LQHEADVIEW_H
