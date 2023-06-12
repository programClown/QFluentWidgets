#include "Button.h"
#include "Common/StyleSheet.h"
#include "Common/Icon.h"
#include "QFluentWidgets.h"

#include <QDesktopServices>
#include <QPainter>

PushButton::PushButton(const QString &text, QWidget *parent) : PushButton(text, nullptr, parent) { }

PushButton::PushButton(const QString &text, FluentIconBase *ficon, QWidget *parent)
    : QPushButton(text, parent), m_isPressed(false), m_ficon(ficon)
{
    FluentStyleSheet::apply("BUTTON", this);
    if (m_ficon.isNull()) {
        setProperty("hasIcon", false);
    } else {
        if (ficon->icon().isNull()) {
            setProperty("hasIcon", false);
        } else {
            setProperty("hasIcon", true);
        }
    }

    setIconSize(QSize(16, 16));
}

void PushButton::setIcon(FluentIconBase *ficon)
{
    m_ficon.reset(ficon);
    update();
}

FluentIconBase *PushButton::ficon() const
{
    return m_ficon.data();
}

QIcon PushButton::icon() const
{
    if (!m_ficon) {
        return QIcon();
    }
    return m_ficon->icon();
}

void PushButton::drawIcon(QPainter *painter, const QRect &rect)
{
    if (m_ficon) {
        m_ficon->render(painter, rect);
    }
}

void PushButton::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    QPushButton::mousePressEvent(event);
}

void PushButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}

void PushButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    if (!m_ficon) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (!isEnabled()) {
        painter.setOpacity(0.43);
    } else if (m_isPressed) {
        painter.setOpacity(0.63);
    }

    int w  = iconSize().width();
    int h  = iconSize().height();
    int y  = (height() - h) / 2;
    int mw = this->minimumSizeHint().width();
    if (mw > 0) {
        drawIcon(&painter, QRectF(12 + (width() - mw) / 2, y, w, h).toRect());
    } else {
        drawIcon(&painter, QRect(12, y, w, h));
    }
}

PrimaryPushButton::PrimaryPushButton(const QString &text, QWidget *parent) : PushButton(text, nullptr, parent) { }

PrimaryPushButton::PrimaryPushButton(const QString &text, FluentIconBase *ficon, QWidget *parent)
    : PushButton(text, ficon, parent)
{
}

void PrimaryPushButton::drawIcon(QPainter *painter, const QRect &rect)
{
    FluentIconBase *ic = ficon();
    if (ic && this->isEnabled()) {
        // reverse icon color
        Qfw::Theme theme;
        if (QFWIns.isDarkTheme()) {
            theme = Qfw::Theme::DARK;
        } else {
            theme = Qfw::Theme::LIGHT;
        }
        ic->setTheme(theme);

        return PushButton::drawIcon(painter, rect);
    } else if (!this->isEnabled()) {
        painter->setOpacity((QFWIns.isDarkTheme() ? 0.786 : 0.9));
        return PushButton::drawIcon(painter, rect);
    }

    return PushButton::drawIcon(painter, rect);
}

HyperlinkButton::HyperlinkButton(const QString &url, const QString &text, QWidget *parent)
    : QPushButton(text, parent), m_url(QUrl(url))
{
    connect(this, &QPushButton::clicked, [this]() { QDesktopServices::openUrl(m_url); });
    FluentStyleSheet::apply("BUTTON", this);
    setCursor(Qt::PointingHandCursor);
}

RadioButton::RadioButton(const QString &text, QWidget *parent) : QRadioButton(text, parent)
{
    FluentStyleSheet::apply("BUTTON", this);
}

ToolButton::ToolButton(FluentIconBase *ficon, QWidget *parent) : QToolButton(parent), m_isPressed(false), m_ficon(ficon)
{
    FluentStyleSheet::apply("BUTTON", this);
}

void ToolButton::setIcon(FluentIconBase *icon)
{
    m_ficon.reset(icon);
    update();
}

FluentIconBase *ToolButton::ficon() const
{
    return m_ficon.data();
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    QToolButton::mousePressEvent(event);
}

void ToolButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    QToolButton::mouseReleaseEvent(event);
}

void ToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    if (!m_ficon) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (!this->isEnabled()) {
        painter.setOpacity(0.43);
    } else {
        painter.setOpacity(0.63);
    }

    int w = this->iconSize().width();
    int h = this->iconSize().height();

    int x = (this->width() - w) / 2;
    int y = (this->height() - h) / 2;

    m_ficon->render(&painter, QRect(x, y, w, h));
}

TransparentToolButton::TransparentToolButton(FluentIconBase *ficon, QWidget *parent)
    : QToolButton(parent), m_ficon(ficon)
{
    setCursor(Qt::PointingHandCursor);
    FluentStyleSheet::apply("BUTTON", this);
}

void TransparentToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    if (!m_ficon) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    int iw = iconSize().width();

    int ih = iconSize().height();
    int w  = width();
    int h  = height();
    QRect rect((w - iw) / 2, (h - ih) / 2, iw, ih);

    m_ficon->render(&painter, rect);
}
