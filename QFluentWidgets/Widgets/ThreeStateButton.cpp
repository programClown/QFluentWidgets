#include "ThreeStateButton.h"

#include <QEvent>

ThreeStateButton::ThreeStateButton(const QMap<ButtonState, QString> &iconPaths, QWidget *parent,
                                   const QSize &buttonSize, const QSize &iconSize)
    : QToolButton(parent), m_iconPaths(iconPaths), m_buttonSize(buttonSize), m_iconSize(iconSize)
{
    resize(m_buttonSize);
    if (iconSize.isValid()) {
        setIconSize(iconSize);
    } else {
        setIconSize(this->size());
    }

    setCursor(Qt::ArrowCursor);
    setStyleSheet("border: none; margin: 0px; background: transparent");
    setState(ButtonState::NORMAL);
    installEventFilter(this);
}

void ThreeStateButton::setState(ButtonState state)
{
    setIcon(QIcon(m_iconPaths[state]));
}

bool ThreeStateButton::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this) {
        if (event->type() == QEvent::Enter) {
            setState(ButtonState::HOVER);
        }

        if (event->type() == QEvent::Leave || event->type() == QEvent::MouseButtonRelease) {
            setState(ButtonState::NORMAL);
        }

        if (event->type() == QEvent::MouseButtonPress) {
            setState(ButtonState::PRESSED);
        }
    }

    return QToolButton::eventFilter(watched, event);
}
