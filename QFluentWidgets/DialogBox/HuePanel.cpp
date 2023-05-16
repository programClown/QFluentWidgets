#include "HuePanel.h"

#include <QMouseEvent>
#include <QPainter>

HuePanel::HuePanel(const QColor &c, QWidget *parent) : QWidget(parent)
{
    setFixedSize(256, 256);
    huePixmap = QPixmap(":/qfluentwidgets/images/color_dialog/HuePanel.png");
    setColor(c);
}

void HuePanel::setColor(const QColor &c)
{
    color = c;
    color.setHsv(color.hue(), color.saturation(), 255);
    pickerPos = QPoint(hue() * width() / 360, (255 - saturation()) * height() / 255);
    update();
}

int HuePanel::hue() const
{
    return color.hue();
}

int HuePanel::saturation() const
{
    return color.saturation();
}

void HuePanel::setPickerPosition(const QPoint &pos)
{
    pickerPos = pos;

    color.setHsv(qMax(0.0, qMin(1.0, pos.x() * 1.0 / width())) * 360,
                 qMax(0.0, qMin(1.0, (height() - pos.y()) * 1.0 / height()) * 255), 255);
    update();
    emit colorChanged(color);
}

void HuePanel::mousePressEvent(QMouseEvent *event)
{
    setPickerPosition(event->pos());
}

void HuePanel::mouseMoveEvent(QMouseEvent *event)
{
    setPickerPosition(event->pos());
}

void HuePanel::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // draw hue panel
    painter.setBrush(QBrush(huePixmap));
    painter.setPen(QPen(QBrush(QColor(0, 0, 0, 15)), 2.4));
    painter.drawRoundedRect(this->rect(), 5.6, 5.6);

    // draw picker
    if (saturation() > 153 || hue() < 180) {
        color = QColor(Qt::black);
    } else {
        color = QColor(255, 253, 254);
    }

    painter.setPen(QPen(color, 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(pickerPos.x() - 8, pickerPos.y() - 8, 16, 16);
}
