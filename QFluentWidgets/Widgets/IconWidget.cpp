#include "IconWidget.h"

#include <QPainter>

IconWidget::IconWidget(FluentIconBase *ficon, QWidget *parent) : QWidget(parent), m_ficon(ficon) { }

void IconWidget::setIcon(FluentIconBase *icon)
{
    m_ficon.reset(icon);
    update();
}

void IconWidget::paintEvent(QPaintEvent * /*event*/)
{
    if (!m_ficon) {
        return;
    }
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_ficon->render(&painter, rect());
}
