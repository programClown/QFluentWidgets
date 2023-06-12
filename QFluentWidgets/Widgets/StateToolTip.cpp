#include "StateToolTip.h"
#include "Common/Icon.h"
#include "Common/StyleSheet.h"
#include "QFluentWidgets.h"

#include <QPainter>
#include <QTimer>

StateCloseButton::StateCloseButton(QWidget *parent) : QToolButton(parent), m_isEnter(false), m_isPressed(false)
{
    setFixedSize(12, 12);
}

void StateCloseButton::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    update();
    QToolButton::mousePressEvent(event);
}

void StateCloseButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    update();
    QToolButton::mouseReleaseEvent(event);
}

void StateCloseButton::enterEvent(QEvent * /*event*/)
{
    m_isPressed = true;
    update();
}

void StateCloseButton::leaveEvent(QEvent * /*event*/)
{
    m_isPressed = false;
    m_isEnter   = false;
    update();
}

void StateCloseButton::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    if (m_isPressed) {
        painter.setOpacity(0.6);
    } else {
        painter.setOpacity(0.8);
    }

    QScopedPointer<FluentIcon> ficon(NEWFLICON(FluentIcon, CLOSE));
    ficon->render(&painter, rect());
}

StateToolTip::StateToolTip(const QString &title, const QString &content, QWidget *parent)
    : QWidget(parent),
      m_title(title),
      m_content(content),
      m_titleLabel(new QLabel(title, this)),
      m_contentLabel(new QLabel(content, this)),
      m_rotateTimer(new QTimer(this)),
      m_opacityEffect(new QGraphicsOpacityEffect(this)),
      m_animation(new QPropertyAnimation(this)),
      m_closeButton(new StateCloseButton(this)),
      m_isDone(false),
      m_rotateAngle(0),
      m_deltaAngle(20)
{
    initWidget();
}

void StateToolTip::setTitle(const QString &title)
{
    m_title = title;
    m_titleLabel->setText(title);
    m_titleLabel->adjustSize();
}

void StateToolTip::setContent(const QString &content)
{
    m_content = content;
    m_contentLabel->setText(content);

    // adjustSize() will mask spinner get stuck
    m_contentLabel->adjustSize();
}

void StateToolTip::setState(bool isDone)
{
    m_isDone = isDone;
    update();
    if (isDone) {
        QTimer::singleShot(1000, this, &StateToolTip::fadeOut);
    }
}

/// get suitable position in main window
QPoint StateToolTip::getSuitablePos()
{
    QPoint pos;

    QWidget *p = qobject_cast<QWidget *>(this->parent());
    for (int i = 0; i < 10; ++i) {
        int dy     = i * (this->height() + 16);
        pos        = QPoint(p->width() - this->width() - 24, 50 + dy);
        QWidget *w = p->childAt(pos + QPoint(2, 2));
        if (qobject_cast<StateToolTip *>(w) != nullptr) {
            pos += QPoint(0, this->height() + 10);
        } else {
            break;
        }
    }

    return pos;
}

void StateToolTip::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    // NOTE: 未完成

    if (!m_isDone) {
        painter.translate(19, 18);
        painter.rotate(m_rotateAngle);
        if (QFWIns.isDarkTheme()) {
            FluentIcon ficon(FluentIcon::SYNC, Qfw::DARK);
            ficon.render(&painter, QRect(-8, -8, 16, 16));
        } else {
            FluentIcon ficon(FluentIcon::SYNC, Qfw::LIGHT);
            ficon.render(&painter, QRect(-8, -8, 16, 16));
        }
    } else {
        if (QFWIns.isDarkTheme()) {
            FluentIcon ficon(FluentIcon::COMPLETED, Qfw::DARK);
            ficon.render(&painter, QRect(-8, -8, 16, 16));
        } else {
            FluentIcon ficon(FluentIcon::COMPLETED, Qfw::LIGHT);
            ficon.render(&painter, QRect(11, 10, 16, 16));
        }
    }
}

void StateToolTip::onCloseButtonClicked()
{
    emit closedSignal();
    hide();
}

void StateToolTip::rotateTimerFlowSlot()
{
    m_rotateAngle = (m_rotateAngle + m_deltaAngle) % 360;
    update();
}

void StateToolTip::fadeOut()
{
    m_rotateTimer->stop();
    m_animation->setDuration(200);
    m_animation->setStartValue(1);
    m_animation->setEndValue(0);
    connect(m_animation, &QPropertyAnimation::finished, this, &StateToolTip::deleteLater);
    m_animation->start();
}

void StateToolTip::initWidget()
{
    setAttribute(Qt::WA_StyledBackground);
    setGraphicsEffect(m_opacityEffect);
    m_opacityEffect->setOpacity(1);
    m_rotateTimer->setInterval(50);
    m_contentLabel->setMinimumWidth(200);

    // connect signal to slot
    connect(m_closeButton, &StateCloseButton::clicked, this, &StateToolTip::onCloseButtonClicked);
    connect(m_rotateTimer, &QTimer::timeout, this, &StateToolTip::rotateTimerFlowSlot);

    setQss();
    initLayout();

    m_rotateTimer->start();
}

void StateToolTip::setQss()
{
    m_titleLabel->setObjectName("titleLabel");
    m_contentLabel->setObjectName("contentLabel");

    FluentStyleSheet::apply("STATE_TOOL_TIP", this);

    m_titleLabel->adjustSize();
    m_contentLabel->adjustSize();
}

void StateToolTip::initLayout()
{
    setFixedSize(qMax(m_titleLabel->width(), m_contentLabel->width()) + 56, 51);
    m_titleLabel->move(32, 9);
    m_contentLabel->move(12, 27);
    m_closeButton->move(this->width() - 24, 19);
}
