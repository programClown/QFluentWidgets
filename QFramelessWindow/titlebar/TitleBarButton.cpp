#include "TitleBarButton.h"

#include <QFile>
#include <QPainter>
#include <QSvgRenderer>
#include <QDebug>
#include <QMouseEvent>
#include <QPainterPath>

TitleBarButton::TitleBarButton(QWidget *parent)
    : QAbstractButton(parent),
      m_state(TitleBarButtonState::NORMAL),
      m_normalColor(QColor(0, 0, 0)),
      m_hoverColor(QColor(0, 0, 0)),
      m_pressedColor(QColor(0, 0, 0)),
      m_normalBgColor(QColor(0, 0, 0, 0)),
      m_hoverBgColor(QColor(0, 0, 0, 26)),
      m_pressedBgColor(QColor(0, 0, 0, 51))
{
    this->setCursor(Qt::ArrowCursor);
    this->setFixedSize(46, 32);
}

void TitleBarButton::setState(TitleBarButtonState state)
{
    this->m_state = state;
    this->update();
}

bool TitleBarButton::isPressed()
{
    return m_state == TitleBarButtonState::PRESSED;
}

QColor TitleBarButton::getNormalColor()
{
    return m_normalColor;
}

QColor TitleBarButton::getHoverColor()
{
    return m_hoverColor;
}

QColor TitleBarButton::getPressedColor()
{
    return m_pressedColor;
}

QColor TitleBarButton::getNormalBackgroundColor()
{
    return m_normalBgColor;
}

QColor TitleBarButton::getHoverBackgroundColor()
{
    return m_hoverBgColor;
}

QColor TitleBarButton::getPressedBackgroundColor()
{
    return m_pressedBgColor;
}

void TitleBarButton::setNormalColor(const QColor &color)
{
    m_normalColor = color;
    this->update();
}

void TitleBarButton::setHoverColor(const QColor &color)
{
    m_hoverColor = color;
    this->update();
}

void TitleBarButton::setPressedColor(const QColor &color)
{
    m_pressedColor = color;
    this->update();
}

void TitleBarButton::setNormalBackgroundColor(const QColor &color)
{
    m_normalBgColor = color;
    this->update();
}

void TitleBarButton::setHoverBackgroundColor(const QColor &color)
{
    m_hoverBgColor = color;
    this->update();
}

void TitleBarButton::setPressedBackgroundColor(const QColor &color)
{
    m_pressedBgColor = color;
    this->update();
}

/**
 * @brief get the icon color and background color
 * @return
 */
QVector<QColor> TitleBarButton::getColors()
{
    if (m_state == TitleBarButtonState::NORMAL) {
        return QVector<QColor>() << m_normalColor << m_normalBgColor;
    } else if (m_state == TitleBarButtonState::HOVER) {
        return QVector<QColor>() << m_hoverColor << m_hoverBgColor;
    } else {
        return QVector<QColor>() << m_pressedColor << m_pressedBgColor;
    }
}

void TitleBarButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    setState(TitleBarButtonState::PRESSED);
    QAbstractButton::mousePressEvent(event);
}

void TitleBarButton::enterEvent(QEvent *event)
{
    setState(TitleBarButtonState::HOVER);
    QAbstractButton::enterEvent(event);
}

void TitleBarButton::leaveEvent(QEvent *event)
{
    setState(TitleBarButtonState::NORMAL);
    QAbstractButton::leaveEvent(event);
}

SvgTitleBarButton::SvgTitleBarButton(const QString &iconPath, QWidget *parent)
    : TitleBarButton(parent), m_svgDom(new QDomDocument)
{
    setIcon(iconPath);
}

SvgTitleBarButton::~SvgTitleBarButton()
{
    delete m_svgDom;
}

void SvgTitleBarButton::setIcon(const QString &iconPath)
{
    QFile f(iconPath);
    if (f.open(QFile::ReadOnly)) {
        m_svgDom->setContent(f.readAll());
        f.close();
    }
}

void SvgTitleBarButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    auto c         = this->getColors();
    QColor color   = c.at(0);
    QColor bgColor = c.at(1);

    // draw background
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    // draw icon
    QString colorName      = color.name();
    QDomNodeList pathNodes = m_svgDom->elementsByTagName("path");
    for (int i = 0; i < pathNodes.count(); ++i) {
        QDomElement element = pathNodes.at(i).toElement();
        element.setAttribute("stroke", colorName);
    }

    QSvgRenderer renderer(m_svgDom->toByteArray());
    renderer.render(&painter, QRectF(rect()));
}

MinimizeButton::MinimizeButton(QWidget *parent) : TitleBarButton(parent) { }

void MinimizeButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    auto c         = this->getColors();
    QColor color   = c.at(0);
    QColor bgColor = c.at(1);

    // draw background
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    // draw icon
    painter.setBrush(Qt::NoBrush);
    QPen pen = QPen(color, 1);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.drawLine(18, 16, 28, 16);
}

MaximizeButton::MaximizeButton(QWidget *parent) : TitleBarButton(parent), m_isMax(false) { }

/// update the maximized state and icon
void MaximizeButton::setMaxState(bool isMax)
{
    if (isMax == m_isMax) {
        return;
    }

    m_isMax = isMax;

    setState(TitleBarButtonState::NORMAL);
}

void MaximizeButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    auto c         = this->getColors();
    QColor color   = c.at(0);
    QColor bgColor = c.at(1);

    // draw background
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    // draw icon
    painter.setBrush(Qt::NoBrush);
    QPen pen = QPen(color, 1);
    pen.setCosmetic(true);
    painter.setPen(pen);

    qreal r = this->devicePixelRatioF();
    painter.scale(1 / r, 1 / r);

    if (!m_isMax) {
        painter.drawRect(18 * r, 11 * r, 10 * r, 10 * r);
    } else {
        painter.drawRect(18 * r, 13 * r, 8 * r, 8 * r);
        int x0 = 18 * r + 2 * r;
        int y0 = 13 * r;
        int dw = 2 * r;
        QPainterPath path(QPointF(x0, y0));
        path.lineTo(x0, y0 - dw);
        path.lineTo(x0 + 8 * r, y0 - dw);
        path.lineTo(x0 + 8 * r, y0 - dw + 8 * r);
        path.lineTo(x0 + 8 * r - dw, y0 - dw + 8 * r);
        painter.drawPath(path);
    }
}

CloseButton::CloseButton(QWidget *parent) : SvgTitleBarButton(":/qframelesswindow/images/close.svg", parent)
{
    this->setHoverColor(Qt::white);
    this->setPressedColor(Qt::white);
    this->setHoverBackgroundColor(QColor(232, 17, 35));
    this->setPressedBackgroundColor(QColor(241, 112, 122));
};
