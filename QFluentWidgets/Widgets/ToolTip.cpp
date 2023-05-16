#include "ToolTip.h"
#include "Common/StyleSheet.h"

#include <QApplication>
#include <QScreen>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QTimer>
#include <QEvent>

ToolTip::ToolTip(const QString &text, QWidget *parent)
    : QFrame(parent),
      m_text(text),
      m_duration(1000),
      m_container(new QFrame(this)),
      m_timer(new QTimer(this)),
      m_label(new QLabel(text, this))
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    setLayout(mainLayout);

    QHBoxLayout *containerLayout = new QHBoxLayout(m_container);

    mainLayout->setContentsMargins(12, 8, 12, 12);
    mainLayout->addWidget(m_container);
    containerLayout->addWidget(m_label);
    containerLayout->setContentsMargins(8, 6, 8, 6);

    // add shadow
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(25);
    shadowEffect->setColor(QColor(0, 0, 0, 60));
    shadowEffect->setOffset(0, 5);
    m_container->setGraphicsEffect(shadowEffect);

    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &ToolTip::hide);

    // set style
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setQss();
}

void ToolTip::setQss()
{
    m_container->setObjectName("container");
    m_label->setObjectName("contentLabel");
    FluentStyleSheet::apply("TOOL_TIP", this);
    m_label->adjustSize();
    adjustSize();
}

QString ToolTip::text() const
{
    return m_text;
}

/// set text on tooltip
void ToolTip::setText(const QString &text)
{
    m_text = text;
    m_label->setText(text);
    m_container->adjustSize();
    adjustSize();
}

int ToolTip::duration() const
{
    return m_duration;
}

/// set tooltip duration in milliseconds
void ToolTip::setDuration(int duration)
{
    m_duration = duration;
}

/**
 * @brief adjust the position of tooltip relative to widget
 * @param pos: global position of widget
 * @param size: size of widget
 */
void ToolTip::adjustPos(const QPoint &pos, const QSize &size)
{
    int x = pos.x() + size.width() / 2 - this->width() / 2;
    int y = pos.y() - this->height();

    // adjust postion to prevent tooltips from appearing outside the screen
    QRect rect = QApplication::screenAt(QCursor::pos())->availableGeometry();
    if (QCursor::pos().x() >= 0) {
        x = qMin(qMax(0, x), rect.width() - this->width() - 4);
    } else {
        x = qMin(x, rect.width() - this->width() - 4);
    }
    y = qMin(qMax(0, y), rect.height() - this->height() - 4);

    this->move(x, y);
}

void ToolTip::showEvent(QShowEvent *event)
{
    m_timer->stop();
    m_timer->start(m_duration);
    QFrame::showEvent(event);
}

void ToolTip::hideEvent(QHideEvent *event)
{
    m_timer->stop();
    QFrame::hideEvent(event);
}

ToolTipFilter::ToolTipFilter(QWidget *parent, int showDelay)
    : QObject(parent), m_isEnter(false), m_toolTip(nullptr), m_tooltipDelay(showDelay), m_timer(new QTimer(this))
{
}

void ToolTipFilter::hideToolTip()
{
    m_isEnter = false;
    if (m_toolTip) {
        m_toolTip->hide();
    }
}

void ToolTipFilter::showToolTip()
{
    if (!m_isEnter) {
        return;
    }

    QWidget *parent = qobject_cast<QWidget *>(this->parent());
    m_toolTip->setText(parent->toolTip());
    m_toolTip->adjustPos(parent->mapToGlobal(QPoint()), parent->size());
    m_toolTip->show();
}

void ToolTipFilter::setTooltipDelay(int delay)
{
    m_tooltipDelay = delay;
}

bool ToolTipFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        return true;
    } else if ((event->type() == QEvent::Hide) || (event->type() == QEvent::Leave)) {
        hideToolTip();
    } else if (event->type() == QEvent::Enter) {
        m_isEnter       = true;
        QWidget *parent = qobject_cast<QWidget *>(this->parent());
        if (parent->isWidgetType() && !parent->toolTip().isEmpty()) {
            if (!m_toolTip) {
                m_toolTip = new ToolTip(parent->toolTip(), parent->window());
            }

            int t = 1000;
            if (parent->toolTipDuration() > 0) {
                t = parent->toolTipDuration();
            }
            m_toolTip->setDuration(t);

            // show the tool tip after delay
            QTimer::singleShot(m_tooltipDelay, this, &ToolTipFilter::showToolTip);
        }
    }

    return QObject::eventFilter(watched, event);
}
