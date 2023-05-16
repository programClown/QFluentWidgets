#include "FlowLayout.h"

#include <QWidget>
#include <QVariant>

FlowLayout::FlowLayout(QWidget *parent, bool needAni, bool isTight)
    : QLayout(parent),
      m_aniGroup(new QParallelAnimationGroup(this)),
      m_verticalSpacing(10),
      m_horizontalSpacing(10),
      m_needAni(needAni),
      m_isTight(isTight)
{
}

FlowLayout::~FlowLayout()
{
    removeAllItems();
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

void FlowLayout::addItem(QLayoutItem *item)
{
    m_items.append(item);
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    if (index >= 0 && index < m_items.count()) {
        return m_items[index];
    }

    return nullptr;
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < m_items.count()) {
        QLayoutItem *item       = m_items[index];
        QPropertyAnimation *ani = item->widget()->property("flowAni").value<QPropertyAnimation *>();
        if (ani) {
            m_anis.removeAll(ani);
            m_aniGroup->removeAnimation(ani);
            ani->deleteLater();
        }

        return m_items.takeAt(index);
    }

    return nullptr;
}

int FlowLayout::count() const
{
    return m_items.count();
}

QSize FlowLayout::minimumSize() const
{
    QSize size;

    for (auto item : m_items) {
        size = size.expandedTo(item->minimumSize());
    }

    QMargins m = contentsMargins();
    size += QSize(m.left() + m.right(), m.top() + m.bottom());
    return size;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return Qt::Orientation(0);
}

void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, true);
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    return doLayout(QRect(0, 0, width, 0), false);
}

void FlowLayout::addWidget(QWidget *w)
{
    QLayout::addWidget(w);

    if (!m_needAni) {
        return;
    }

    QPropertyAnimation *ani = new QPropertyAnimation(w, "geometry");
    ani->setEndValue(QRect(QPoint(0, 0), w->size()));
    ani->setDuration(300);
    w->setProperty("flowAni", QVariant::fromValue<QPropertyAnimation *>(ani));
    m_anis.append(ani);
    m_aniGroup->addAnimation(ani);
}

void FlowLayout::setAnimation(int duration /*msec*/, QEasingCurve ease)
{
    if (!m_needAni) {
        return;
    }

    for (auto a : m_anis) {
        a->setDuration(duration);
        a->setEasingCurve(ease);
    }
}

void FlowLayout::removeAllItems()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::takeAllWidgets()
{
    QLayoutItem *item;
    while ((item = takeAt(0))) {
        QWidget *w = item->widget();
        if (w) {
            w->deleteLater();
        }
        delete item;
    }
}

int FlowLayout::doLayout(const QRect &rect, bool move) const
{
    QMargins margin = contentsMargins();
    int x           = rect.x() + margin.left();
    int y           = rect.y() + margin.top();
    int rowHeight   = 0;
    int spaceX      = m_horizontalSpacing;
    int spaceY      = m_verticalSpacing;

    for (int i = 0; i < m_items.count(); ++i) {
        QLayoutItem *item = m_items.at(i);

        if (item->widget() && !item->widget()->isVisible() && m_isTight) {
            continue;
        }

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > rect.right() && rowHeight > 0) {
            x         = rect.x() + margin.left();
            y         = y + rowHeight + spaceY;
            nextX     = x + item->sizeHint().width() + spaceX;
            rowHeight = 0;
        }

        if (move) {
            if (!m_needAni) {
                item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
            } else {
                m_anis[i]->stop();
                m_anis[i]->setEndValue(QRect(QPoint(x, y), item->sizeHint()));
            }
        }

        x         = nextX;
        rowHeight = qMax(rowHeight, item->sizeHint().height());
    }

    if (m_needAni) {
        m_aniGroup->stop();
        m_aniGroup->start();
    }

    return y + rowHeight - rect.y();
}

int FlowLayout::horizontalSpacing() const
{
    return m_horizontalSpacing;
}

void FlowLayout::setHorizontalSpacing(int horizontalSpacing)
{
    m_horizontalSpacing = horizontalSpacing;
}

int FlowLayout::verticalSpacing() const
{
    return m_verticalSpacing;
}

void FlowLayout::setVerticalSpacing(int verticalSpacing)
{
    m_verticalSpacing = verticalSpacing;
}
