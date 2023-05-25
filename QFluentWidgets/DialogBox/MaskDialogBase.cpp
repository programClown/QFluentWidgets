#include "MaskDialogBase.h"
#include "QFluentWidgets.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QDebug>

MaskDialogBase::MaskDialogBase(QWidget *parent) : QDialog(parent)
{
    m_hBoxLayout = new QHBoxLayout(this);
    windowMask   = new QWidget(this);

    // dialog box in the center of mask, all widgets take it as parent
    widget = new QFrame(this);
    widget->setObjectName("centerWidget");

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    if (parent) {
        this->setGeometry(0, 0, parent->width(), parent->height());
    } else {
        this->setGeometry(0, 0, this->window()->width(), this->window()->height());
    }

    int c = QFWIns.isDarkTheme() ? 0 : 255;

    windowMask->resize(this->size());
    windowMask->setStyleSheet(QString("background:rgba({%1}, {%2}, {%3}, 0.6)").arg(c).arg(c).arg(c));
    m_hBoxLayout->addWidget(widget);
    setLayout(m_hBoxLayout);
    this->setShadowEffect();
    this->window()->installEventFilter(this);
}

void MaskDialogBase::setShadowEffect(float blurRadius, const QPointF &offset, const QColor &color)
{
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(widget);
    shadowEffect->setBlurRadius(blurRadius);
    shadowEffect->setOffset(offset);
    shadowEffect->setColor(color);
    widget->setGraphicsEffect(nullptr);
    widget->setGraphicsEffect(shadowEffect);
}

void MaskDialogBase::setMaskColor(const QColor &color)
{
    windowMask->setStyleSheet(QString("background: rgba(%1, %2, %3, %4)")
                                      .arg(color.red())
                                      .arg(color.green())
                                      .arg(color.blue())
                                      .arg(color.alpha()));
}

bool MaskDialogBase::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->window()) {
        if (event->type() == QEvent::Resize) {
            QResizeEvent *re = static_cast<QResizeEvent *>(event);
            this->resize(re->size());
        }
    }

    return QDialog::eventFilter(watched, event);
}

void MaskDialogBase::resizeEvent(QResizeEvent * /*event*/)
{
    windowMask->resize(this->size());
}

/// fade out
void MaskDialogBase::closeEvent(QCloseEvent *event)
{
    widget->setGraphicsEffect(nullptr);
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *opacityAni = new QPropertyAnimation(opacityEffect, "opacity", this);
    opacityAni->setStartValue(1);
    opacityAni->setEndValue(0);
    opacityAni->setDuration(100);
    opacityAni->setEasingCurve(QEasingCurve::OutCubic);
    connect(opacityAni, &QPropertyAnimation::finished, opacityAni, &QPropertyAnimation::deleteLater);
    opacityAni->start();
    event->accept();  //原作是ignore，有问题
}

/// fade in
void MaskDialogBase::showEvent(QShowEvent *event)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *opacityAni = new QPropertyAnimation(opacityEffect, "opacity", this);
    opacityAni->setStartValue(0);
    opacityAni->setEndValue(1);
    opacityAni->setDuration(200);
    opacityAni->setEasingCurve(QEasingCurve::InSine);
    connect(opacityAni, &QPropertyAnimation::finished, opacityEffect, &QGraphicsOpacityEffect::deleteLater);
    opacityAni->start();

    QDialog::showEvent(event);
}

QHBoxLayout *MaskDialogBase::hBoxLayout() const
{
    return m_hBoxLayout;
}
