#include "VBoxLayout.h"

#include <QWidget>

VBoxLayout::VBoxLayout(QWidget *parent) : QVBoxLayout(parent) { }

VBoxLayout::~VBoxLayout()
{
    removeAllItems();
}

void VBoxLayout::addWidgets(const QList<QWidget *> &widgets, int stretch, Qt::Alignment alignment)
{
    for (auto w : widgets) {
        addWidget(w, stretch, alignment);
    }
}

void VBoxLayout::addWidget(QWidget *widget, int stretch, Qt::Alignment alignment)
{
    QVBoxLayout::addWidget(widget, stretch, alignment);
    m_widgets.append(widget);
    widget->show();
}

// remove widget from layout but not delete it
void VBoxLayout::removeWidget(QWidget *widget)
{
    QVBoxLayout::removeWidget(widget);
    m_widgets.removeAll(widget);
}

// remove widget from layout and delete it
void VBoxLayout::deleteWidget(QWidget *widget)
{
    removeWidget(widget);
    widget->hide();
    widget->deleteLater();
}

void VBoxLayout::removeAllWidget()
{
    qDeleteAll(m_widgets);
    m_widgets.clear();
}

void VBoxLayout::removeAllItems()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

QList<QWidget *> VBoxLayout::widgets() const
{
    return m_widgets;
}
