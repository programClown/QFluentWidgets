#include "SmoothScroll.h"

#include <QApplication>
#include <QDateTime>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>
#include <QtMath>

SmoothScroll::SmoothScroll(QAbstractScrollArea *widget, Qt::Orientation orient)
    : QObject(widget),
      m_widget(widget),
      m_orient(orient),
      m_fps(60),
      m_duration(400),
      m_stepsTotal(0),
      m_stepRatio(1.5),
      m_acceleration(1),
      m_lastWheelEvent(nullptr),
      m_smoothMoveTimer(new QTimer(this)),
      m_smoothMode(SmoothMode::LINEAR)
{
    connect(m_smoothMoveTimer.get(), &QTimer::timeout, this, &SmoothScroll::smoothMove);
}

void SmoothScroll::wheelEvent(QWheelEvent *event)
{
    // only process the wheel events triggered by mouse
    //    if ((m_smoothMode == SmoothMode::NO_SMOOTH) || (abs(event->angleDelta().y()) % 120 != 0)) {
    //        m_widget->wheelEvent(event);
    //        return;
    //    }

    // push current time to queque
    qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    m_scrollStamps.append(now);
    while (now - m_scrollStamps.at(0) > 500) {
        m_scrollStamps.pop_front();
    }

    // adjust the acceration ratio based on unprocessed events
    double accerationRatio = qMin(m_scrollStamps.count() / 15.0, 1.0);
    m_lastWheelEvent.reset(new QWheelEvent(event->pos(), event->globalPos(), event->pixelDelta(), event->angleDelta(),
                                           event->delta(), event->orientation(), event->buttons(), event->modifiers()));

    // get the number of steps
    m_stepsTotal = m_fps * m_duration / 1000.0;

    // get the moving distance corresponding to each event
    double delta = event->angleDelta().y() * m_stepRatio;
    if (!qFuzzyCompare(m_acceleration, 0)) {
        delta += delta * m_acceleration * accerationRatio;
    }

    // form a list of moving distances and steps, and insert it into the queue for processing.
    m_stepsLeftQueue.append(StepsLeftElement{ delta, m_stepsTotal });

    // overflow time of timer: 1000ms/frames
    m_smoothMoveTimer->start(int(1000 / m_fps));
}

void SmoothScroll::setSmoothMode(const SmoothMode &smoothMode)
{
    m_smoothMode = smoothMode;
}

SmoothMode SmoothScroll::smoothMode() const
{
    return m_smoothMode;
}

/// scroll smoothly when timer time out
void SmoothScroll::smoothMove()
{
    if (!m_lastWheelEvent) {
        return;
    }

    double totalDelta = 0;

    // Calculate the scrolling distance of all unprocessed events,
    // the timer will reduce the number of steps by 1 each time it overflows.

    for (auto &i : m_stepsLeftQueue) {
        totalDelta += subDelta(i.delta, i.stepLeft);
        i.stepLeft -= 1;
    }

    // If the event has been processed, move it out of the queue
    while (!m_stepsLeftQueue.isEmpty() && qFuzzyIsNull(m_stepsLeftQueue.at(0).stepLeft)) {
        m_stepsLeftQueue.pop_front();
    }

    QScrollBar *bar;
    QPoint p;
    // construct wheel event
    if (m_orient == Qt::Vertical) {
        p   = QPoint(0, qRound(totalDelta));
        bar = m_widget->verticalScrollBar();
    } else {
        p   = QPoint(qRound(totalDelta), 0);
        bar = m_widget->horizontalScrollBar();
    }

    QWheelEvent e(m_lastWheelEvent->pos(), m_lastWheelEvent->globalPos(), QPoint(), p, qRound(totalDelta), m_orient,
                  m_lastWheelEvent->buttons(), Qt::NoModifier);

    // send wheel event to app
    QApplication::sendEvent(bar, &e);

    // stop scrolling if the queque is empty
    if (m_stepsLeftQueue.isEmpty()) {
        m_smoothMoveTimer->stop();
    }
}

/// get the interpolation for each step
double SmoothScroll::subDelta(double delta, double stepsLeft)
{
    double m = m_stepsTotal / 2;
    double x = fabs(m_stepsTotal - stepsLeft - m);

    double res = 0;
    if (m_smoothMode == SmoothMode::NO_SMOOTH) {
        res = 0;
    } else if (m_smoothMode == SmoothMode::CONSTANT) {
        res = delta / m_stepsTotal;
    } else if (m_smoothMode == SmoothMode::LINEAR) {
        res = 2 * delta / m_stepsTotal * (m - x) / m;
    } else if (m_smoothMode == SmoothMode::QUADRATI) {
        res = (1 - x * x / m / m) * delta * 3 / 4 / m;
    } else if (m_smoothMode == SmoothMode::COSINE) {
        res = (cos(x * M_PI / m) + 1) / (2 * m) * delta;
    }

    return res;
}
