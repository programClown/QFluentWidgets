#include "Label.h"

#include <QPainter>

PixmapLabel::PixmapLabel(QWidget *parent) : QLabel(parent), m_pixmap(QPixmap()) { }

void PixmapLabel::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    setFixedSize(pixmap.size());
    update();
}

QPixmap PixmapLabel::pixmap() const
{
    return m_pixmap;
}

void PixmapLabel::paintEvent(QPaintEvent * /*event*/)
{
    if (m_pixmap.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.drawPixmap(rect(), m_pixmap);
}
