#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QPropertyAnimation>
#include <QScrollArea>
#include <QScrollBar>
#include "Common/SmoothScroll.h"

class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ScrollArea(QWidget *parent = nullptr, Qt::Orientation orient = Qt::Vertical);

    void setSmoothMode(const SmoothMode &mode);

    void setViewportMargins(int left, int top, int right, int bottom);

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event) override;

signals:

private:
    SmoothScroll *m_smoothScroll;
};

class SmoothScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit SmoothScrollBar(QWidget *parent = nullptr);

    void setValue(int value);
    void scrollValue(int value);
    void scrollTo(int value);
    void resetValue(int value);
    void setScrollAnimation(int duration, QEasingCurve easing = QEasingCurve::OutCubic);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int m_duration;
    QPropertyAnimation *m_ani;
    int m_value;
};

class SmoothScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit SmoothScrollArea(QWidget *parent = nullptr);

    void setScrollAnimation(Qt::Orientation orient, int duration, QEasingCurve easing = QEasingCurve::OutCubic);

    void setViewportMargins(int left, int top, int right, int bottom);

private:
    SmoothScrollBar *m_hScrollBar;
    SmoothScrollBar *m_vScrollBar;

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif  // SCROLLAREA_H
