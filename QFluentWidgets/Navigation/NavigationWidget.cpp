#include "NavigationWidget.h"
#include "QFluentWidgets.h"

#include <QPainter>

int NavigationWidget::EXPAND_WIDTH = 312;

NavigationWidget::NavigationWidget(bool selectable, QWidget *parent)
    : QWidget(parent), isCompacted(true), isSelected(false), isPressed(false), isEnter(false), isSelectable(selectable)
{
    setFixedSize(40, 36);
}

/// set whether the widget is compacted
void NavigationWidget::setCompacted(bool compacted)
{
    if (isCompacted == compacted) {
        return;
    }

    isCompacted = compacted;
    if (isCompacted) {
        setFixedSize(40, 36);
    } else {
        setFixedSize(EXPAND_WIDTH, 36);
    }

    update();
}

/**
 * @brief set whether the button is selected
 * @param isSelected: whether the button is selected
 */
void NavigationWidget::setSelected(bool selected)
{
    if (!isSelectable || (isSelected == selected)) {
        return;
    }

    isSelected = selected;
    update();
}

void NavigationWidget::mousePressEvent(QMouseEvent * /*event*/)
{
    isPressed = true;
    update();
}

void NavigationWidget::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    isPressed = false;
    update();
    emit clicked(true);
}

void NavigationWidget::enterEvent(QEvent * /*event*/)
{
    isEnter = true;
    update();
}

void NavigationWidget::leaveEvent(QEvent * /*event*/)
{
    isEnter   = false;
    isPressed = false;
    update();
}

NavigationPushButton::NavigationPushButton(FluentIconBase *ficon, const QString &text, bool selectable, QWidget *parent)
    : NavigationWidget(selectable, parent), m_ficon(ficon), m_text(text)
{
    setStyleSheet("NavigationPushButton{font: 14px \"Segoe UI\", \"Microsoft YaHei\"}");
}

QString NavigationPushButton::text() const
{
    return m_text;
}

void NavigationPushButton::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    if (isPressed) {
        painter.setOpacity(0.7);
    } else {
        painter.setOpacity(0.4);
    }

    // draw background
    int c = QFWIns.isDarkTheme() ? 255 : 0;
    if (isSelected) {
        if (isEnter) {
            painter.setBrush(QColor(c, c, c, 6));
        } else {
            painter.setBrush(QColor(c, c, c, 10));
        }
        painter.drawRoundedRect(rect(), 5, 5);

        // draw indicator
        painter.setBrush(themeColor());
        painter.drawRoundedRect(0, 10, 3, 16, 1.5, 1.5);
    } else {
        painter.setBrush(QColor(c, c, c, 10));
        painter.drawRoundedRect(rect(), 5, 5);
    }

    m_ficon->render(&painter, QRectF(11.5, 10, 16, 16).toRect());
    // draw text
    if (!isCompacted) {
        painter.setFont(font());
        painter.setPen(QColor(c, c, c));
        painter.drawText(QRect(44, 0, width() - 57, height()), Qt::AlignVCenter, m_text);
    }
}

NavigationToolButton::NavigationToolButton(FluentIconBase *ficon, QWidget *parent)
    : NavigationPushButton(ficon, "", false, parent)
{
}

void NavigationToolButton::setCompacted(bool /*compacted*/)
{
    setFixedSize(40, 36);
}

NavigationSeparator::NavigationSeparator(QWidget *parent) : NavigationWidget(false, parent)
{
    setCompacted(true);
}

void NavigationSeparator::setCompacted(bool compacted)
{
    if (compacted) {
        setFixedSize(48, 3);
    } else {
        setFixedSize(EXPAND_WIDTH + 10, 3);
    }

    update();
}

void NavigationSeparator::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    int c = QFWIns.isDarkTheme() ? 255 : 0;
    QPen pen(QColor(c, c, c, 15));
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.drawLine(0, 1, width(), 1);
}
