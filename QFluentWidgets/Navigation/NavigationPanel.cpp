#include "NavigationPanel.h"
#include "Widgets/ScrollArea.h"
#include "Widgets/ToolTip.h"
#include "Common/Icon.h"

#include <QApplication>
#include <QDebug>
#include <QResizeEvent>

NavigationPanel::NavigationPanel(bool minimalEnabled, QWidget *parent) : QFrame(parent)
{
    m_parent                = parent;
    m_isMenuButtonVisible   = true;
    m_isReturnButtonVisible = false;

    scrollArea   = new ScrollArea(this);
    scrollWidget = new QWidget();

    menuButton   = new NavigationToolButton(NEWFLICON(FluentIcon, MENU), this);
    returnButton = new NavigationToolButton(NEWFLICON(FluentIcon, RETURN), this);

    vBoxLayout   = new NavigationItemLayout(this);
    topLayout    = new NavigationItemLayout();
    bottomLayout = new NavigationItemLayout();
    scrollLayout = new NavigationItemLayout(scrollWidget);

    history = new NavigationHistory({}, this);

    m_expandAni   = new QPropertyAnimation(this, "geometry", this);
    m_expandWidth = 322;

    isMinimalEnabled = minimalEnabled;

    if (isMinimalEnabled) {
        displayMode = NavigationDisplayMode::MINIMAL;
    } else {
        displayMode = NavigationDisplayMode::COMPACT;
    }

    initWidget();
}

void NavigationPanel::initWidget()
{
    resize(48, height());
    setAttribute(Qt::WA_StyledBackground);
    window()->installEventFilter(this);

    returnButton->hide();
    returnButton->setDisabled(true);

    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);

    m_expandAni->setEasingCurve(QEasingCurve::OutQuad);
    m_expandAni->setDuration(150);

    connect(menuButton, &NavigationToolButton::clicked, this, &NavigationPanel::toggle);
    connect(m_expandAni, &QPropertyAnimation::finished, this, &NavigationPanel::onExpandAniFinished);
    connect(history, &NavigationHistory::emptyChanged, returnButton, &NavigationToolButton::setDisabled);
    connect(returnButton, &NavigationToolButton::clicked, history, &NavigationHistory::pop);

    // add tool tip;
    returnButton->installEventFilter(new ToolTipFilter(returnButton, 1000));
    returnButton->setToolTip(tr("Back"));

    menuButton->installEventFilter(new ToolTipFilter(menuButton, 1000));
    menuButton->setToolTip(tr("Open Navigation"));

    setProperty("menu", false);
    scrollWidget->setObjectName("scrollWidget");
    FluentStyleSheet::apply("NAVIGATION_INTERFACE", this);

    initLayout();
}

void NavigationPanel::initLayout()
{
    vBoxLayout->setContentsMargins(0, 5, 0, 5);
    topLayout->setContentsMargins(4, 0, 4, 0);
    bottomLayout->setContentsMargins(4, 0, 4, 0);
    scrollLayout->setContentsMargins(4, 0, 4, 0);
    vBoxLayout->setSpacing(4);
    topLayout->setSpacing(4);
    bottomLayout->setSpacing(4);
    scrollLayout->setSpacing(4);

    vBoxLayout->addLayout(topLayout, 0);
    vBoxLayout->addWidget(scrollArea, 1, Qt::AlignTop);
    vBoxLayout->addLayout(bottomLayout, 0);

    vBoxLayout->setAlignment(Qt::AlignTop);
    topLayout->setAlignment(Qt::AlignTop);
    scrollLayout->setAlignment(Qt::AlignTop);
    bottomLayout->setAlignment(Qt::AlignBottom);

    topLayout->addWidget(returnButton, 0, Qt::AlignTop);
    topLayout->addWidget(menuButton, 0, Qt::AlignTop);
}

void NavigationPanel::addItem(const QString &routeKey, FluentIconBase *icon, const QString &text,
                              const QObject *receiver, const char *onClick, bool selectable,
                              NavigationItemPosition position)
{
    if (history->m_items.keys().contains(routeKey)) {
        return;
    }

    NavigationPushButton *button = new NavigationPushButton(icon, text, selectable, this);
    addWidget(routeKey, button, receiver, onClick, position);
}

void NavigationPanel::addWidget(const QString &routeKey, NavigationWidget *widget, const QObject *receiver,
                                const char *onClick, NavigationItemPosition position)
{
    if (history->m_items.keys().contains(routeKey)) {
        return;
    }

    connect(widget, &NavigationWidget::clicked, this, &NavigationPanel::onWidgetClicked);
    connect(widget, SIGNAL(clicked(bool)), receiver, onClick);
    widget->setProperty("routeKey", routeKey);
    history->m_items.insert(routeKey, widget);

    if (displayMode == NavigationDisplayMode::EXPAND || displayMode == NavigationDisplayMode::MENU) {
        widget->setCompacted(false);
    }

    addWidgetToLayout(widget, position);
}

void NavigationPanel::removeWidget(const QString &routeKey)
{
    if (!history->m_items.keys().contains(routeKey)) {
        return;
    }

    NavigationWidget *w = history->m_items.take(routeKey);
    w->deleteLater();
    history->remove(routeKey, true);
}

void NavigationPanel::setMenuButtonVisible(bool visible)
{
    m_isMenuButtonVisible = visible;
    menuButton->setVisible(visible);
}

void NavigationPanel::setReturnButtonVisible(bool visible)
{
    m_isMenuButtonVisible = visible;
    returnButton->setVisible(visible);
}

void NavigationPanel::setExpandWidth(int width)
{
    if (width <= 42) {
        return;
    }

    m_expandWidth                  = width;
    NavigationWidget::EXPAND_WIDTH = width - 10;
}

/// expand navigation panel
void NavigationPanel::expand()
{
    setWidgetCompacted(false);
    m_expandAni->setProperty("expand", true);

    // determine the display mode according to the width of window
    // https://learn.microsoft.com/en-us/windows/apps/design/controls/navigationview#default
    int expandWidth = 1007 + this->m_expandWidth - 322;
    if (this->window()->width() > expandWidth && !isMinimalEnabled) {
        displayMode = NavigationDisplayMode::EXPAND;
    } else {
        setProperty("menu", true);
        setStyle(QApplication::style());
        displayMode = NavigationDisplayMode::MENU;
        if (!m_parent->isWindow()) {
            QPoint pos = m_parent->pos();
            setParent(this->window());
            move(pos);
        }
        this->show();
    }

    emit displayModeChanged(displayMode);
    m_expandAni->setStartValue(QRect(this->pos(), QSize(48, this->height())));
    m_expandAni->setEndValue(QRect(this->pos(), QSize(m_expandWidth, this->height())));
    m_expandAni->start();
}

/// collapse navigation panel
void NavigationPanel::collapse()
{
    if (m_expandAni->state() == QPropertyAnimation::Running) {
        return;
    }

    m_expandAni->setStartValue(QRect(this->pos(), QSize(this->width(), this->height())));
    m_expandAni->setEndValue(QRect(this->pos(), QSize(48, this->height())));
    m_expandAni->setProperty("expand", false);
    m_expandAni->start();
}

void NavigationPanel::onExpandAniFinished()
{
    if (!m_expandAni->property("expand").toBool()) {
        if (isMinimalEnabled) {
            displayMode = NavigationDisplayMode::MINIMAL;
        } else {
            displayMode = NavigationDisplayMode::COMPACT;
        }
        emit displayModeChanged(displayMode);
    }

    if (displayMode == NavigationDisplayMode::MINIMAL) {
        this->hide();
        this->setProperty("menu", false);
        this->setStyle(QApplication::style());
    } else if (displayMode == NavigationDisplayMode::COMPACT) {
        this->setProperty("menu", false);
        this->setStyle(QApplication::style());

        for (auto item : history->m_items.values()) {
            item->setCompacted(true);
        }

        if (!m_parent->isWindow()) {
            this->setParent(m_parent);
            this->move(0, 0);
            this->show();
        }
    }
}

void NavigationPanel::addSeparator(NavigationItemPosition position)
{
    NavigationSeparator *separator = new NavigationSeparator(this);
    addWidgetToLayout(separator, position);
}

void NavigationPanel::setCurrentItem(const QString &routeKey)
{
    if (!history->m_items.keys().contains(routeKey)) {
        return;
    }

    history->push(routeKey);
    QHashIterator<QString, NavigationWidget *> i(history->m_items);
    while (i.hasNext()) {
        i.next();
        i.value()->setSelected(i.key() == routeKey);
    }
}

int NavigationPanel::layoutMinHeight()
{
    int th = topLayout->minimumSize().height();
    int bh = bottomLayout->minimumSize().height();
    int sh = 0;
    for (auto w : this->findChildren<NavigationSeparator *>()) {
        sh += w->height();
    }

    int spacing = topLayout->count() * topLayout->spacing();
    spacing += bottomLayout->count() * bottomLayout->spacing();
    return 36 + th + bh + sh + spacing;
}

/// set the routing key to use when the navigation history is empty
void NavigationPanel::setDefaultRouteKey(const QString &routeKey)
{
    history->setDefaultRouteKey(routeKey);
}

bool NavigationPanel::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->window()) {
        return QFrame::eventFilter(watched, event);
    }

    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvt = static_cast<QMouseEvent *>(event);
        if (!this->geometry().contains(mouseEvt->pos()) && (displayMode == NavigationDisplayMode::MENU)) {
            collapse();
        }
    } else if (event->type() == QEvent::Resize) {
        QResizeEvent *resizeEvt = static_cast<QResizeEvent *>(event);
        int w                   = resizeEvt->size().width();
        if (w < 1008 && displayMode == NavigationDisplayMode::EXPAND) {
            collapse();
        } else if (w >= 1008 && displayMode == NavigationDisplayMode::COMPACT && !m_isMenuButtonVisible) {
            expand();
        }
    }

    return QFrame::eventFilter(watched, event);
}

void NavigationPanel::resizeEvent(QResizeEvent *event)
{
    if (event->oldSize().height() == this->height()) {
        return;
    }

    int th = topLayout->minimumSize().height();
    int bh = bottomLayout->minimumSize().height();
    int h  = this->height() - th - bh - 20;
    scrollArea->setFixedHeight(qMax(h, 36));
}

void NavigationPanel::addWidgetToLayout(NavigationWidget *widget, NavigationItemPosition position)
{
    if (position == NavigationItemPosition::TOP) {
        widget->setParent(this);
        topLayout->addWidget(widget, 0, Qt::AlignTop);
    } else if (position == NavigationItemPosition::SCROLL) {
        widget->setParent(scrollWidget);
        scrollLayout->addWidget(widget, 0, Qt::AlignTop);
    } else {
        widget->setParent(this);
        bottomLayout->addWidget(widget, 0, Qt::AlignBottom);
    }

    widget->show();
}

void NavigationPanel::setWidgetCompacted(bool compacted)
{
    for (auto item : this->findChildren<NavigationWidget *>()) {
        item->setCompacted(compacted);
    }
}

void NavigationPanel::onWidgetClicked()
{
    NavigationWidget *widget = qobject_cast<NavigationWidget *>(sender());
    if (!widget->isSelectable) {
        return;
    }

    setCurrentItem(widget->property("routeKey").toString());

    if (widget != menuButton && displayMode == NavigationDisplayMode::MENU) {
        collapse();
    }
}

/// toggle navigation panel
void NavigationPanel::toggle()
{
    if (displayMode == NavigationDisplayMode::COMPACT || displayMode == NavigationDisplayMode::MINIMAL) {
        this->expand();
    } else {
        this->collapse();
    }
}

NavigationHistory::NavigationHistory(const QHash<QString, NavigationWidget *> &items, QObject *parent)
    : QObject(parent), m_items(items)
{
    m_defaultRouteKey = QString();
}

QString NavigationHistory::defaultRouteKey() const
{
    return m_defaultRouteKey;
}

void NavigationHistory::setDefaultRouteKey(const QString &key)
{
    if (!m_items.keys().contains(key)) {
        qCritical() << "The route key `" + key + " ` has not been registered yet.";
    } else {
        m_defaultRouteKey = key;
    }
}

/// push history
void NavigationHistory::push(const QString &routeKey)
{
    if (m_history.isEmpty() && m_defaultRouteKey != routeKey) {
        m_history.append(routeKey);
        emit emptyChanged(false);
    } else if (!m_history.isEmpty() && m_history.last() != routeKey) {
        m_history.append(routeKey);
    }
}

/// pop history
void NavigationHistory::pop()
{
    if (m_history.isEmpty()) {
        return;
    }

    m_history.pop_back();
    navigate();
}

/// remove history
void NavigationHistory::remove(const QString &routeKey, bool all)
{
    if (!m_history.contains(routeKey)) {
        return;
    }

    if (all) {
        m_history.removeAll(routeKey);
    } else {
        int index = m_history.lastIndexOf(routeKey);
        m_history.removeAt(index);
    }

    navigate();
}

void NavigationHistory::navigate()
{
    if (!m_history.isEmpty()) {
        emit m_items[m_history.last()]->clicked(false);
    } else {
        if (!m_defaultRouteKey.isEmpty()) {
            emit m_items[defaultRouteKey()]->clicked(false);
        }

        emit emptyChanged(true);
    }
}
