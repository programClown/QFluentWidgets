#include "ExpandLayout.h"

#include <QResizeEvent>
#include <QWidget>

ExpandLayout::ExpandLayout(QWidget *parent) : QLayout(parent) { }

ExpandLayout::~ExpandLayout()
{
    removeAllItems();
}

void ExpandLayout::addWidget(QWidget *w)
{
    if (m_widgets.contains(w)) {
        return;
    }

    m_widgets.append(w);
    w->installEventFilter(this);
}

QSize ExpandLayout::sizeHint() const
{
    return minimumSize();
}

void ExpandLayout::addItem(QLayoutItem *item)
{
    m_items.append(item);
}

QLayoutItem *ExpandLayout::itemAt(int index) const
{
    if (index >= 0 && index < m_items.count()) {
        return m_items[index];
    }

    return nullptr;
}

QLayoutItem *ExpandLayout::takeAt(int index)
{
    if (index >= 0 && index < m_items.count()) {
        return m_items.takeAt(index);
    }

    return nullptr;
}

int ExpandLayout::count() const
{
    return m_items.count();
}

QSize ExpandLayout::minimumSize() const
{
    QSize size;

    for (auto w : m_widgets) {
        size = size.expandedTo(w->minimumSize());
    }

    QMargins m = contentsMargins();
    size += QSize(m.left() + m.right(), m.top() + m.bottom());
    return size;
}

Qt::Orientations ExpandLayout::expandingDirections() const
{
    return Qt::Vertical;
}

void ExpandLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, true);
}

bool ExpandLayout::hasHeightForWidth() const
{
    return true;
}

int ExpandLayout::heightForWidth(int width) const
{
    return doLayout(QRect(0, 0, width, 0), false);
}

bool ExpandLayout::eventFilter(QObject *watched, QEvent *event)
{
    if (m_widgets.contains((QWidget *)watched)) {
        if (event->type() == QEvent::Resize) {
            QResizeEvent *re = static_cast<QResizeEvent *>(event);
            QSize ds         = re->size() - re->oldSize();
            if (ds.height() != 0 && ds.width() == 0) {
                QWidget *w = this->parentWidget();
                w->resize(w->width(), w->height() + ds.height());
            }
        }
    }

    return QLayout::eventFilter(watched, event);
}

void ExpandLayout::removeAllItems()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

int ExpandLayout::doLayout(const QRect &rect, bool move) const
{
    QMargins margin = contentsMargins();
    int x           = rect.x() + margin.left();
    int y           = rect.y() + margin.top() + margin.bottom();
    int width       = rect.width() - margin.left() - margin.right();

    for (int i = 0; i < m_widgets.count(); ++i) {
        if (i > 0) {
            y += this->spacing();
        }
        if (move) {
            m_widgets.at(i)->setGeometry(QRect(QPoint(x, y), QSize(width, m_widgets.at(i)->height())));
        }

        y += m_widgets.at(i)->height();
    }

    return y - rect.y();
}
