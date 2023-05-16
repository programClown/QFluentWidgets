#ifndef SMOOTHSCROLL_H
#define SMOOTHSCROLL_H

#include <QQueue>
#include <QScrollArea>

enum SmoothMode
{
    NO_SMOOTH = 0,
    CONSTANT  = 1,
    LINEAR    = 2,
    QUADRATI  = 3,
    COSINE    = 4
};

class SmoothScroll : public QObject
{
    Q_OBJECT

public:
    struct StepsLeftElement
    {
        double delta;
        double stepLeft;
    };

    explicit SmoothScroll(QAbstractScrollArea *widget, Qt::Orientation orient = Qt::Vertical);

    void setSmoothMode(const SmoothMode &smoothMode);
    SmoothMode smoothMode() const;

    void wheelEvent(QWheelEvent *event);

private slots:
    void smoothMove();

private:
    double subDelta(double delta, double stepsLeft);

private:
    QAbstractScrollArea *m_widget;
    Qt::Orientation m_orient;
    int m_fps;
    int m_duration;
    double m_stepsTotal;
    double m_stepRatio;
    double m_acceleration;
    QScopedPointer<QWheelEvent> m_lastWheelEvent;
    QQueue<qint64> m_scrollStamps;
    QQueue<StepsLeftElement> m_stepsLeftQueue;
    QScopedPointer<QTimer> m_smoothMoveTimer;
    SmoothMode m_smoothMode;
};

#endif  // SMOOTHSCROLL_H
