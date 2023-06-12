#include "NavigationInterface.h"

#include <QResizeEvent>

NavigationInterface::NavigationInterface(bool showMenuButton, bool showReturnButton, QWidget *parent) : QWidget(parent)
{
    panel = new NavigationPanel(false, this);
    panel->setMenuButtonVisible(showMenuButton);
    panel->setReturnButtonVisible(showReturnButton);
    panel->installEventFilter(this);
    connect(panel, &NavigationPanel::displayModeChanged, this, &NavigationInterface::displayModeChanged);

    resize(48, this->height());
    setMinimumWidth(48);
    setAttribute(Qt::WA_StyledBackground);
    FluentStyleSheet::apply("NAVIGATION_INTERFACE", this);
}

void NavigationInterface::addItem(const QString &routeKey, FluentIconBase *icon, const QString &text,
                                  const QObject *receiver, const char *onClick, bool selectable,
                                  NavigationItemPosition position)
{
    panel->addItem(routeKey, icon, text, receiver, onClick, selectable, position);
    setMinimumHeight(panel->layoutMinHeight());
}

void NavigationInterface::addWidget(const QString &routeKey, NavigationWidget *widget, const QObject *receiver,
                                    const char *onClick, NavigationItemPosition position)
{
    panel->addWidget(routeKey, widget, receiver, onClick, position);
    setMinimumHeight(panel->layoutMinHeight());
}

void NavigationInterface::addSeparator(NavigationItemPosition position)
{
    panel->addSeparator(position);
    setMinimumHeight(panel->layoutMinHeight());
}

void NavigationInterface::removeWidget(const QString &routeKey)
{
    panel->removeWidget(routeKey);
}

void NavigationInterface::setCurrentItem(const QString &name)
{
    panel->setCurrentItem(name);
}

/// set the routing key to use when the navigation history is empty
void NavigationInterface::setDefaultRouteKey(const QString &routeKey)
{
    panel->setDefaultRouteKey(routeKey);
}

/// set the maximum width
void NavigationInterface::setExpandWidth(int width)
{
    panel->setExpandWidth(width);
}

bool NavigationInterface::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != panel || event->type() != QEvent::Resize) {
        return QWidget::eventFilter(watched, event);
    }

    if (panel->displayMode != NavigationDisplayMode::MENU) {
        QResizeEvent *resizeEvt = dynamic_cast<QResizeEvent *>(event);
        if (resizeEvt->oldSize().width() != resizeEvt->size().width()) {
            setFixedWidth(resizeEvt->size().width());
        }
    }

    return QWidget::eventFilter(watched, event);
}

void NavigationInterface::resizeEvent(QResizeEvent *event)
{
    if (event->oldSize().height() != this->height()) {
        panel->setFixedHeight(this->height());
    }
}
