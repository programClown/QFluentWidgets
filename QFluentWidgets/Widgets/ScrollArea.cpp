#include "ScrollArea.h"

#include <QWheelEvent>

ScrollArea::ScrollArea(QWidget *parent, Qt::Orientation orient) : QScrollArea(parent)
{
    m_smoothScroll = new SmoothScroll(this, orient);
}

void ScrollArea::setSmoothMode(const SmoothMode &mode)
{
    m_smoothScroll->setSmoothMode(mode);
}

void ScrollArea::setViewportMargins(int left, int top, int right, int bottom)
{
    QScrollArea::setViewportMargins(left, top, right, bottom);
}

void ScrollArea::wheelEvent(QWheelEvent *event)
{
    // only process the wheel events triggered by mouse
    if ((m_smoothScroll->smoothMode() == SmoothMode::NO_SMOOTH) || (abs(event->angleDelta().y()) % 120 != 0)) {
        QScrollArea::wheelEvent(event);
    } else {
        m_smoothScroll->wheelEvent(event);
    }

    event->setAccepted(true);
}

SmoothScrollBar::SmoothScrollBar(QWidget *parent) : QScrollBar(parent), m_duration(500)
{
    m_ani = new QPropertyAnimation(this);
    m_ani->setTargetObject(this);
    m_ani->setPropertyName("value");
    m_ani->setEasingCurve(QEasingCurve::OutCubic);
    m_ani->setDuration(m_duration);

    m_value = this->value();
}

void SmoothScrollBar::setValue(int value)
{
    if (value == this->value()) {
        return;
    }

    // stop running animation
    m_ani->stop();

    // adjust the duration
    int dv = abs(value - this->value());
    if (dv < 50) {
        m_ani->setDuration(int(m_duration * dv / 70));
    } else {
        m_ani->setDuration(m_duration);
    };
    m_ani->setStartValue(this->value());
    m_ani->setEndValue(value);
    m_ani->start();
}

/// scroll the specified distance
void SmoothScrollBar::scrollValue(int value)
{
    m_value = value;
    m_value = qMax(this->minimum(), m_value);
    m_value = qMin(this->maximum(), m_value);
    this->setValue(m_value);
}

/// scroll to the specified position
void SmoothScrollBar::scrollTo(int value)
{
    m_value = value;
    m_value = qMax(this->minimum(), m_value);
    m_value = qMin(this->maximum(), m_value);
    this->setValue(m_value);
}

void SmoothScrollBar::resetValue(int value)
{
    m_value = value;
}

/**
 * @brief set scroll animation
 * @param durationL: scroll duration
 * @param easing: animation type
 */
void SmoothScrollBar::setScrollAnimation(int duration, QEasingCurve easing)
{
    m_duration = duration;
    m_ani->setDuration(duration);
    m_ani->setEasingCurve(easing);
}

void SmoothScrollBar::mousePressEvent(QMouseEvent *event)
{
    m_ani->stop();
    QScrollBar::mousePressEvent(event);
    m_value = this->value();
}

void SmoothScrollBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_ani->stop();
    QScrollBar::mouseReleaseEvent(event);
    m_value = this->value();
}

void SmoothScrollBar::mouseMoveEvent(QMouseEvent *event)
{
    m_ani->stop();
    QScrollBar::mouseMoveEvent(event);
    m_value = this->value();
}

SmoothScrollArea::SmoothScrollArea(QWidget *parent) : QScrollArea(parent)
{
    m_hScrollBar = new SmoothScrollBar(this);
    m_vScrollBar = new SmoothScrollBar(this);
    m_hScrollBar->setOrientation(Qt::Horizontal);
    m_vScrollBar->setOrientation(Qt::Vertical);
    setVerticalScrollBar(m_vScrollBar);
    setHorizontalScrollBar(m_hScrollBar);
}

/**
 * @brief set scroll animation
 * @param orientL: scroll orientation
 * @param duration: scroll duration
 * @param easing: animation type
 */
void SmoothScrollArea::setScrollAnimation(Qt::Orientation orient, int duration, QEasingCurve easing)
{
    SmoothScrollBar *bar = (orient == Qt::Horizontal ? m_hScrollBar : m_vScrollBar);
    bar->setScrollAnimation(duration, easing);
}

void SmoothScrollArea::setViewportMargins(int left, int top, int right, int bottom)
{
    QScrollArea::setViewportMargins(left, top, right, bottom);
}

void SmoothScrollArea::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::NoModifier) {
        m_vScrollBar->scrollValue(-event->angleDelta().y());
    } else {
        m_hScrollBar->scrollValue(-event->angleDelta().x());
    }

    event->setAccepted(true);
}
