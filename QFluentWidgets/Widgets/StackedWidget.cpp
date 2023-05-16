#include "StackedWidget.h"

#include <QDebug>

OpacityAniStackedWidget::OpacityAniStackedWidget(QWidget *parent) : QStackedWidget(parent), m_nextIndex(0) { }

void OpacityAniStackedWidget::addWidget(QWidget *w)
{
    QStackedWidget::addWidget(w);

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(1);

    QPropertyAnimation *ani = new QPropertyAnimation(effect, "opacity", this);
    ani->setDuration(220);

    connect(ani, &QPropertyAnimation::finished, this, &OpacityAniStackedWidget::onAniFinished);
    m_anis << ani;
    m_effects << effect;
    w->setGraphicsEffect(effect);
}

void OpacityAniStackedWidget::setCurrentIndex(int index)
{
    int i = this->currentIndex();
    if (index == i) {
        return;
    }

    QPropertyAnimation *ani = nullptr;
    if (index > i) {
        ani = m_anis[index];
        ani->setStartValue(0);
        ani->setEndValue(1);
        QStackedWidget::setCurrentIndex(index);
    } else {
        ani = m_anis[index];
        ani->setStartValue(1);
        ani->setEndValue(0);
    }

    this->widget(i)->show();
    m_nextIndex = index;
}

void OpacityAniStackedWidget::setCurrentWidget(QWidget *w)
{
    this->setCurrentIndex(this->indexOf(w));
}

void OpacityAniStackedWidget::onAniFinished()
{
    QStackedWidget::setCurrentIndex(m_nextIndex);
}

PopUpAniStackedWidget::PopUpAniStackedWidget(QWidget *parent) : QStackedWidget(parent), m_nextIndex(-1), m_ani(nullptr)
{
}

/**
 * @brief add widget to window
 * @param widget: widget to be added
 * @param deltaX: the x-axis offset from the beginning to the end of animation
 * @param deltaY: the y-axis offset from the beginning to the end of animation
 */
void PopUpAniStackedWidget::addWidget(QWidget *widget, int deltaX, int deltaY)
{
    QStackedWidget::addWidget(widget);

    m_aniInfos << PopUpAniInfo(widget, deltaX, deltaY, new QPropertyAnimation(widget, "pos"));
}

/**
 * @brief set current window to display
 * @param index: the index of widget to display
 * @param needPopOut: need pop up animation or not
 * @param showNextWidgetDirectly: whether to show next widget directly when animation started
 * @param duration: animation duration
 * @param easingCurve: the interpolation mode of animation
 */
void PopUpAniStackedWidget::setCurrentIndex(int index, bool needPopOut, bool showNextWidgetDirectly, int duration,
                                            QEasingCurve easingCurve)
{
    if (index < 0 || index >= this->count()) {
        qCritical() << QString("The index `%1` is illegal").arg(index);
        return;
    }

    if (index == this->currentIndex()) {
        return;
    }

    if (m_ani && m_ani->state() == QAbstractAnimation::Running) {
        m_ani->stop();
        onAniFinished();
    }

    // get the index of widget to be displayed
    m_nextIndex = index;

    // get animation
    PopUpAniInfo nextAniInfo    = m_aniInfos[index];
    PopUpAniInfo currentAniInfo = m_aniInfos[this->currentIndex()];

    QWidget *currentWidget = this->currentWidget();
    QWidget *nextWidget    = nextAniInfo.widget;

    QPropertyAnimation *ani = needPopOut ? currentAniInfo.ani : nextAniInfo.ani;
    m_ani                   = ani;

    if (needPopOut) {
        int deltaX = currentAniInfo.deltaX;
        int deltaY = currentAniInfo.deltaY;
        QPoint pos = currentWidget->pos() + QPoint(deltaX, deltaY);
        setAnimation(ani, currentWidget->pos(), pos, duration, easingCurve);
        nextWidget->setVisible(showNextWidgetDirectly);
    } else {
        int deltaX = nextAniInfo.deltaX;
        int deltaY = nextAniInfo.deltaY;
        QPoint pos = nextWidget->pos() + QPoint(deltaX, deltaY);
        setAnimation(ani, pos, QPoint(nextWidget->x(), this->y()), duration, easingCurve);
        QStackedWidget::setCurrentIndex(index);
    }

    // start animation
    connect(ani, &QPropertyAnimation::finished, this, &PopUpAniStackedWidget::onAniFinished);
    ani->start();
    emit aniStart();
}

/**
 * @brief set currect widget
 * @param widget: the widget to be displayed
 * @param needPopOut: need pop up animation or not
 * @param showNextWidgetDirectly: whether to show next widget directly when animation started
 * @param duration: animation duration
 * @param easingCurve: the interpolation mode of animation
 */
void PopUpAniStackedWidget::setCurrentWidget(QWidget *widget, bool needPopOut, bool showNextWidgetDirectly,
                                             int duration, QEasingCurve easingCurve)
{
    this->setCurrentIndex(this->indexOf(widget), needPopOut, showNextWidgetDirectly, duration, easingCurve);
}

/// set the config of animation
void PopUpAniStackedWidget::setAnimation(QPropertyAnimation *ani, const QPoint &startValue, const QPoint &endValue,
                                         int duration, QEasingCurve easingCurve)
{
    ani->setEasingCurve(easingCurve);
    ani->setStartValue(startValue);
    ani->setEndValue(endValue);
    ani->setDuration(duration);
}

void PopUpAniStackedWidget::onAniFinished() { }
