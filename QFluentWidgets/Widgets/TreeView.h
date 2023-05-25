#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QPainter>
#include <QStyledItemDelegate>
#include <QTreeWidget>

#include "QFluentWidgets.h"

class SmoothScroll;

class TreeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TreeItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent){};

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        QStyledItemDelegate::paint(painter, option, index);

        if (!(option.state & (QStyle::State_Selected | QStyle::State_MouseOver))) {
            return;
        }

        painter->save();
        painter->setPen(Qt::NoPen);

        // draw background
        int h = option.rect.height() - 4;
        int c = QFWIns.isDarkTheme() ? 255 : 0;
        painter->setBrush(QColor(c, c, c, 9));
        QWidget *p = static_cast<QWidget *>(this->parent());
        painter->drawRoundedRect(4, option.rect.y() + 2, p->width() - 8, h, 4, 4);

        // draw indicator
        if (option.state & QStyle::State_Selected) {
            painter->setBrush(themeColor());
            painter->drawRoundedRect(4, 9 + option.rect.y(), 3, h - 13, 1.5, 1.5);
        }

        painter->restore();
    }
};

class TreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(QWidget *parent = nullptr);

    SmoothScroll *verticalSmoothScroll;
    SmoothScroll *horizonSmoothScroll;

protected:
    virtual void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const override;

    virtual void wheelEvent(QWheelEvent *event) override;
};

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeView(QWidget *parent = nullptr);

    SmoothScroll *verticalSmoothScroll;
    SmoothScroll *horizonSmoothScroll;

protected:
    virtual void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const override;

    virtual void wheelEvent(QWheelEvent *event) override;
};

#endif  // TREEVIEW_H
