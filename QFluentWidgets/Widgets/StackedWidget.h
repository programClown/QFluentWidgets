#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class OpacityAniStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit OpacityAniStackedWidget(QWidget *parent = nullptr);
    void addWidget(QWidget *w);
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *w);

private slots:
    void onAniFinished();

private:
    int m_nextIndex;
    QList<QGraphicsOpacityEffect *> m_effects;
    QList<QPropertyAnimation *> m_anis;
};

struct PopUpAniInfo
{
public:
    PopUpAniInfo(QWidget *w, int dx, int dy, QPropertyAnimation *a)
    {
        widget = w;
        deltaX = dx;
        deltaY = dy;
        ani    = a;
    }

    QWidget *widget;
    int deltaX;
    int deltaY;
    QPropertyAnimation *ani;
};

class PopUpAniStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit PopUpAniStackedWidget(QWidget *parent = nullptr);

    void addWidget(QWidget *widget, int deltaX = 0, int deltaY = 76);
    void setCurrentIndex(int index, bool needPopOut = false, bool showNextWidgetDirectly = true, int duration = 250,
                         QEasingCurve easingCurve = QEasingCurve::OutQuad);
    void setCurrentWidget(QWidget *widget, bool needPopOut = false, bool showNextWidgetDirectly = true,
                          int duration = 250, QEasingCurve easingCurve = QEasingCurve::OutQuad);

private slots:
    void setAnimation(QPropertyAnimation *ani, const QPoint &startValue, const QPoint &endValue, int duration,
                      QEasingCurve easingCurve = QEasingCurve::Linear);
    void onAniFinished();

signals:
    void aniFinished();
    void aniStart();

private:
    QList<PopUpAniInfo> m_aniInfos;
    int m_nextIndex;
    QPropertyAnimation *m_ani;
};

#endif  // STACKEDWIDGET_H
