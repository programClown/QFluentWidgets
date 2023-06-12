#include "ExpandGroupSettingCard.h"
#include "QFluentWidgets.h"
#include "SettingCard.h"
#include "Layout/VBoxLayout.h"

#include <QPainter>
#include <QApplication>
#include <QMouseEvent>

ExpandButton::ExpandButton(QWidget *parent) : QAbstractButton(parent), isHover(false), isPressed(false), m_angle(0)
{
    setFixedSize(30, 30);
    m_rotateAni = new QPropertyAnimation(this, "angle", this);
}

float ExpandButton::getAngle() const
{
    return m_angle;
}

void ExpandButton::setAngle(float angle)
{
    m_angle = angle;
    update();
}

void ExpandButton::setPressed(bool pressed)
{
    isPressed = pressed;
    update();
}

void ExpandButton::setHover(bool hover)
{
    isHover = hover;
    update();
}

void ExpandButton::mousePressEvent(QMouseEvent *event)
{
    QAbstractButton::mousePressEvent(event);
    setPressed(true);
}

void ExpandButton::mouseReleaseEvent(QMouseEvent *event)
{
    QAbstractButton::mouseReleaseEvent(event);
    setPressed(false);
}

void ExpandButton::enterEvent(QEvent * /*event*/)
{
    setHover(true);
}

void ExpandButton::leaveEvent(QEvent * /*event*/)
{
    setHover(false);
}

void ExpandButton::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    // draw background
    int r = QFWIns.isDarkTheme() ? 255 : 0;
    QColor color;
    if (isPressed) {
        color = QColor(r, r, r, 10);
    } else if (isHover) {
        color = QColor(r, r, r, 14);
    } else {
        color = Qt::transparent;
    }

    painter.setBrush(color);
    painter.drawRoundedRect(rect(), 4, 4);

    // draw icon
    painter.translate(width() / 2, height() / 2);
    painter.rotate(m_angle);

    QScopedPointer<FluentIcon> ficon(NEWFLICON(FluentIcon, ARROW_DOWN));
    ficon->render(&painter, QRectF(-5, -5, 9.6, 9.6).toRect());
}

void ExpandButton::onClicked()
{
    if (m_angle < 180) {
        m_rotateAni->setEndValue(180);
    } else {
        m_rotateAni->setEndValue(0);
    }
    m_rotateAni->setDuration(200);
    m_rotateAni->start();
}

ExpandSettingCard::ExpandSettingCard(FluentIconBase *icon, const QString &title, const QString &content,
                                     QWidget *parent)
    : QFrame(parent), m_isExpand(false)
{
    m_expandButton = new ExpandButton(this);
    m_view         = new QFrame(this);
    m_card         = new SettingCard(icon, title, content, this);
    viewLayout     = new VBoxLayout(m_view);

    // expand animation
    m_aniGroup  = new QParallelAnimationGroup(this);
    m_slideAni  = new QPropertyAnimation(m_view, "pos", this);
    m_expandAni = new QPropertyAnimation(this, "geometry", this);

    initWidget();
}

/// add widget to tail
void ExpandSettingCard::addWidget(QWidget *widget)
{
    m_card->hBoxLayout->addWidget(widget, 0, Qt::AlignRight);
    m_card->hBoxLayout->addSpacing(19);
    m_card->hBoxLayout->addWidget(m_expandButton, 0, Qt::AlignRight);
    m_card->hBoxLayout->addSpacing(8);
}

/// set the expand status of card
void ExpandSettingCard::setExpand(bool expand)
{
    if (m_isExpand == expand) {
        return;
    }

    // update style sheet
    m_isExpand = expand;
    setProperty("isExpand", expand);
    setStyle(QApplication::style());

    // start expand animation
    int ch = m_card->height();
    int vh = m_view->height();
    if (expand) {
        m_expandAni->setStartValue(this->geometry());
        m_expandAni->setEndValue(QRect(this->pos(), QSize(this->width(), ch + vh)));
        m_slideAni->setStartValue(QPoint(0, ch - vh));
        m_slideAni->setEndValue(QPoint(0, ch));
        m_view->show();
    } else {
        m_expandAni->setStartValue(this->geometry());
        m_expandAni->setEndValue(QRect(this->pos(), m_card->size()));
        m_slideAni->setStartValue(QPoint(0, ch));
        m_slideAni->setEndValue(QPoint(0, ch - vh));
    }
    m_aniGroup->start();
}

void ExpandSettingCard::setValue(const QVariant &value)
{
    Q_UNUSED(value)
}

void ExpandSettingCard::toggleExpand()
{
    setExpand(!m_isExpand);
}

void ExpandSettingCard::resizeEvent(QResizeEvent * /*event*/)
{
    m_card->resize(this->width(), m_card->height());
    m_view->resize(this->width(), m_view->height());
}

bool ExpandSettingCard::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_card) {
        if (event->type() == QEvent::Enter) {
            m_expandButton->setHover(true);
        } else if (event->type() == QEvent::Leave) {
            m_expandButton->setHover(false);
        } else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            if (me->button() == Qt::LeftButton) {
                m_expandButton->setPressed(true);
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            if (me->button() == Qt::LeftButton) {
                m_expandButton->setPressed(false);
                m_expandButton->click();
            }
        }
    }

    return QFrame::eventFilter(watched, event);
}

void ExpandSettingCard::initWidget()
{
    setMinimumHeight(m_card->height());
    m_view->hide();

    // initialize expand animation
    m_aniGroup->addAnimation(m_expandAni);
    m_aniGroup->addAnimation(m_slideAni);
    m_slideAni->setEasingCurve(QEasingCurve::OutQuad);
    m_expandAni->setEasingCurve(QEasingCurve::OutQuad);
    m_slideAni->setDuration(200);
    m_expandAni->setDuration(200);

    // initialize style sheet
    m_view->setObjectName("view");
    setProperty("isExpand", false);

    FluentStyleSheet::apply("EXPAND_SETTING_CARD", m_card);
    FluentStyleSheet::apply("EXPAND_SETTING_CARD", this);

    m_card->installEventFilter(this);
    connect(m_aniGroup, &QParallelAnimationGroup::finished, this, &ExpandSettingCard::onAniFinished);
    connect(m_expandButton, &ExpandButton::clicked, this, &ExpandSettingCard::toggleExpand);
}

void ExpandSettingCard::adjustViewSize()
{
    int h = 0;
    for (auto w : viewLayout->widgets()) {
        h += w->height();
    }

    int spacing     = (viewLayout->widgets().count() - 1) * viewLayout->spacing();
    QMargins margin = viewLayout->contentsMargins();
    h               = h + margin.top() + margin.bottom() + spacing;
    m_view->resize(m_view->width(), h);

    if (m_view->isVisible()) {
        resize(this->width(), h + m_card->height());
    }
}

/// expand animation finished slot
void ExpandSettingCard::onAniFinished()
{
    if (!m_isExpand) {
        m_view->hide();
    }
}

QFrame *ExpandSettingCard::view() const
{
    return m_view;
}

GroupSeparator::GroupSeparator(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(3);
}

void GroupSeparator::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    int c = QFWIns.isDarkTheme() ? 35 : 230;
    painter.setPen(QColor(c, c, c));
    painter.drawLine(0, 1, this->width(), 1);
}

ExpandGroupSettingCard::ExpandGroupSettingCard(FluentIconBase *icon, const QString &title, const QString &content,
                                               QWidget *parent)
    : ExpandSettingCard(icon, title, content, parent)
{
}

void ExpandGroupSettingCard::addGroupWidget(QWidget *widget)
{
    if (viewLayout->widgets().contains(widget)) {
        return;
    }

    // add separator
    if (viewLayout->count() >= 1) {
        viewLayout->addWidget(new GroupSeparator(this->view()));
    }

    widget->setParent(this->view());
    viewLayout->addWidget(widget);
}
