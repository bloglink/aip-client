/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       筛选输入表头
*******************************************************************************/
#include "lqheadview.h"

LQHeadView::LQHeadView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent) {
    initUI();
}

QSize LQHeadView::sizeHint() const
{
    QSize s = QHeaderView::sizeHint();
    if (filterWidgets.size())
        s.setHeight(s.height() + filterWidgets.at(0)->sizeHint().height() + 5);
    return s;
}

void LQHeadView::updateGeometries()
{
    QSize s = QHeaderView::sizeHint();
    if (filterWidgets.size())
        setViewportMargins(0, 0, 0, s.height()+5);
    else
        setViewportMargins(0, 0, 0, 0);
    QHeaderView::updateGeometries();
    adjustPositions();
}

void LQHeadView::initUI()
{
    setSectionsClickable(true);
    setSortIndicatorShown(true);
    connect(this, SIGNAL(sectionResized(int, int, int)), this, SLOT(adjustPositions()));
}

void LQHeadView::adjustPositions()
{
    for (int i=0; i < filterWidgets.size(); ++i) {
        LQLineEdit* w = filterWidgets.at(i);
        if (QApplication::layoutDirection() == Qt::RightToLeft)
            w->move(width() - (sectionPosition(i)+sectionSize(i)-offset()),
                    w->sizeHint().height() + 2);
        else
            w->move(sectionPosition(i) - offset(), w->sizeHint().height() + 2);
        w->resize(sectionSize(i), w->sizeHint().height());
    }
}

void LQHeadView::inputChanged(QString msg)
{
    LQLineEdit *w = qobject_cast<LQLineEdit*>(sender());
    emit filterChanged(filterWidgets.indexOf(w), msg);
    adjustPositions();
}

void LQHeadView::createFilters(int number, bool showFirst)
{
    qDeleteAll(filterWidgets);
    filterWidgets.clear();

    for (int i=0; i < number; ++i) {
        LQLineEdit* l = new LQLineEdit(this);
        if (!showFirst && i == 0)
            l->setVisible(false);
        else
            l->setVisible(true);
        filterWidgets.push_back(l);
        connect(l, SIGNAL(delayedTextChanged(QString)), this, SLOT(inputChanged(QString)));
    }
    adjustPositions();
}

