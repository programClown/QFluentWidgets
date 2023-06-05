#include "AvatarWidget.h"

#include <QFluentWidgets.h>

#include <QPainter>

AvatarWidget::AvatarWidget(const QString imagePath, QWidget *parent) : NavigationWidget(false, parent)
{
    m_avatar = QImage(imagePath).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void AvatarWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter = QPainter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    if (isPressed) {
        painter.setOpacity(0.7);
    }

    // draw background
    if (isEnter) {
        int c = QFluentWidgets::ins().isDarkTheme() ? 255 : 0;
        painter.setBrush(QColor(c, c, c, 10));
        painter.drawRoundedRect(rect(), 5, 5);
    }

    // draw avatar
    painter.setBrush(QBrush(m_avatar));
    painter.translate(8, 6);
    painter.drawEllipse(0, 0, 24, 24);
    painter.translate(-8, -6);

    if (isCompacted == false) {
        painter.setPen(QFluentWidgets::ins().isDarkTheme() ? Qt::white : Qt::black);
        QFont font = QFont("Segoe UI");
        font.setPixelSize(14);
        painter.setFont(font);
        painter.drawText(QRect(44, 0, 255, 36), Qt::AlignCenter, "user");
    }
}
