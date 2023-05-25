#include "TreeView.h"
#include "Common/SmoothScroll.h"

#include <QWheelEvent>

TreeWidget::TreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    verticalSmoothScroll = new SmoothScroll(this, Qt::Vertical);
    horizonSmoothScroll  = new SmoothScroll(this, Qt::Horizontal);

    setItemDelegate(new TreeItemDelegate(this));
    setIconSize(QSize(16, 16));
    FluentStyleSheet::apply("TREE_VIEW", this);
}

void TreeWidget::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    QRect rc = rect;
    rc.moveLeft(15);

    return QTreeWidget::drawBranches(painter, rc, index);
}

void TreeWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::NoModifier) {
        if ((verticalSmoothScroll->smoothMode() == SmoothMode::NO_SMOOTH)
            || (abs(event->angleDelta().y()) % 120 != 0)) {
            QTreeWidget::wheelEvent(event);
        } else {
            verticalSmoothScroll->wheelEvent(event);
        }
    } else {
        if ((horizonSmoothScroll->smoothMode() == SmoothMode::NO_SMOOTH) || (abs(event->angleDelta().y()) % 120 != 0)) {
            QTreeWidget::wheelEvent(event);
        } else {
            horizonSmoothScroll->wheelEvent(event);
        }
    }
}

TreeView::TreeView(QWidget *parent) : QTreeView(parent)
{
    verticalSmoothScroll = new SmoothScroll(this, Qt::Vertical);
    horizonSmoothScroll  = new SmoothScroll(this, Qt::Horizontal);

    setItemDelegate(new TreeItemDelegate(this));
    setIconSize(QSize(16, 16));
    FluentStyleSheet::apply("TREE_VIEW", this);
    FluentStyleSheet::apply("TREE_VIEW", (QWidget *)this->verticalScrollBar());
}

void TreeView::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    QRect rc = rect;
    rc.moveLeft(15);

    return QTreeView::drawBranches(painter, rc, index);
}

void TreeView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::NoModifier) {
        if ((verticalSmoothScroll->smoothMode() == SmoothMode::NO_SMOOTH)
            || (abs(event->angleDelta().y()) % 120 != 0)) {
            QTreeView::wheelEvent(event);
        } else {
            verticalSmoothScroll->wheelEvent(event);
        }
    } else {
        if ((horizonSmoothScroll->smoothMode() == SmoothMode::NO_SMOOTH) || (abs(event->angleDelta().y()) % 120 != 0)) {
            QTreeView::wheelEvent(event);
        } else {
            horizonSmoothScroll->wheelEvent(event);
        }
    }
}
