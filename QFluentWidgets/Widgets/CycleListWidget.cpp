#include "CycleListWidget.h"
#include "QFluentWidgets.h"
#include "ScrollArea.h"

#include <QEvent>
#include <QEvent>
#include <QPainter>
#include <QWheelEvent>

ScrollIcon::ScrollIcon(IconType type, Qfw::Theme t) : FluentIconBase(""), m_theme(t), m_type(type)
{
    iconEngine->setIconPath(iconPath());
}

ScrollIcon::~ScrollIcon() { }

QString ScrollIcon::iconName(ScrollIcon::IconType type)
{
    switch (type) {
        case UP:
            return "Up";

        case DOWN:
            return "Down";
    }

    return "Unknown";
}

QString ScrollIcon::iconPath()
{
    QString colorName;
    if (m_theme == Qfw::Theme::AUTO) {
        colorName = QFWIns.isDarkTheme() ? "white" : "black";
    } else {
        if (m_theme == Qfw::DARK) {
            colorName = "white";
        } else {
            colorName = "black";
        }
    }

    return QString(":/qfluentwidgets/images/time_picker/%1_%2.svg").arg(iconName(m_type)).arg(colorName);
}

QIcon ScrollIcon::icon()
{
    return QIcon(iconEngine->clone());
}

QString ScrollIcon::typeName() const
{
    return iconName(m_type);
}

QString ScrollIcon::enumName() const
{
    QMetaEnum metaEnum = QMetaEnum::fromType<IconType>();
    return metaEnum.valueToKey(m_type);
}

FluentIconBase *ScrollIcon::clone()
{
    return new ScrollIcon(m_type, m_theme);
}

Qfw::Theme ScrollIcon::theme() const
{
    return m_theme;
}

void ScrollIcon::setTheme(const Qfw::Theme &theme)
{
    m_theme = theme;
    iconEngine->setIconPath(iconPath());
}

ScrollButton::ScrollButton(FluentIconBase *icon, QWidget *parent) : QToolButton(parent), isPressed(false), m_icon(icon)
{
    installEventFilter(this);
}

bool ScrollButton::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this) {
        if (event->type() == QEvent::MouseButtonPress) {
            isPressed = true;
            update();
        } else if (event->type() == QEvent::MouseButtonRelease) {
            isPressed = false;
            update();
        }
    }

    return QToolButton::eventFilter(watched, event);
}

void ScrollButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    int w, h;
    if (!isPressed) {
        w = 10;
        h = 10;
    } else {
        w = 8;
        h = 8;
    }

    int x = (this->width() - w) / 2;
    int y = (this->height() - h) / 2;

    m_icon->render(&painter, QRect(x, y, w, h));
}

CycleListWidget::CycleListWidget(const QVariantList &items, const QSize &itemSize, Qt::AlignmentFlag align,
                                 QWidget *parent)
    : QListWidget(parent), m_itemSize(itemSize), m_align(align), m_isCycle(false), m_currentIndex(-1)
{
    m_upButton       = new ScrollButton(new ScrollIcon(ScrollIcon::UP), this);
    m_downButton     = new ScrollButton(new ScrollIcon(ScrollIcon::DOWN), this);
    m_scrollDuration = 250;
    m_originItems    = items;

    vScrollBar      = new SmoothScrollBar(this);
    m_visibleNumber = 9;

    // repeat adding items to achieve circular scrolling
    setItems(items);

    this->setVerticalScrollMode(ScrollPerPixel);
    this->setVerticalScrollBar(vScrollBar);
    this->vScrollBar->setScrollAnimation(m_scrollDuration);

    this->setViewportMargins(0, 0, 0, 0);
    this->setFixedSize(itemSize.width() + 8, itemSize.height() * m_visibleNumber);

    // hide scroll bar
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_upButton->hide();
    m_downButton->hide();

    connect(m_upButton, &ScrollButton::clicked, this, &CycleListWidget::scrollUp);
    connect(m_downButton, &ScrollButton::clicked, this, &CycleListWidget::scrollDown);
    connect(this, &CycleListWidget::itemClicked, this, &CycleListWidget::onItemClicked);

    this->installEventFilter(this);
}

void CycleListWidget::setItems(const QVariantList &items)
{
    this->clear();
    createItems(items);
}

int CycleListWidget::currentIndex() const
{
    return m_currentIndex;
}

void CycleListWidget::setCurrentIndex(int index)
{
    if (!m_isCycle) {
        int n          = m_visibleNumber / 2;
        m_currentIndex = qMax(n, qMin(n + m_originItems.count() - 1, index));
    } else {
        int N          = this->count() / 2;
        int m          = (m_visibleNumber + 1) / 2;
        m_currentIndex = index;

        // scroll to center to achieve circular scrolling
        if (index >= this->count() - m) {
            m_currentIndex = N + index - this->count();
            QListWidget::scrollToItem(this->item(this->currentIndex() - 1), PositionAtCenter);
        } else if (index <= m - 1) {
            m_currentIndex = N + index;
            QListWidget::scrollToItem(this->item(N + index + 1), PositionAtCenter);
        }
    }
}

QListWidgetItem *CycleListWidget::currentItem() const
{
    return this->item(this->currentIndex());
}

/// scroll to item, scroll to center position
void CycleListWidget::scrollToItem(QListWidgetItem *item, QAbstractItemView::ScrollHint /*hint*/)
{
    int index = this->row(item);
    int y     = item->sizeHint().height() * (index - m_visibleNumber / 2);
    vScrollBar->scrollTo(y);

    // clear selection
    clearSelection();
    item->setSelected(false);

    emit currentItemChanged(item);
}

void CycleListWidget::setSelectedItem(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }

    auto items = this->findItems(text, Qt::MatchExactly);
    if (items.isEmpty()) {
        return;
    }

    if (items.count() >= 2) {
        setCurrentIndex(this->row(items.at(1)));
    } else {
        setCurrentIndex(this->row(items.at(0)));
    }

    QListWidget::scrollToItem(this->currentItem(), QListWidget::PositionAtCenter);
}

QSize CycleListWidget::itemSize() const
{
    return m_itemSize;
}

bool CycleListWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != this || event->type() != QEvent::KeyPress) {
        return QListWidget::eventFilter(watched, event);
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvt = static_cast<QKeyEvent *>(event);
        if (keyEvt->key() == Qt::Key_Down) {
            scrollDown();
            return true;
        } else if (keyEvt->key() == Qt::Key_Up) {
            scrollUp();
            return true;
        }
    }

    return QListWidget::eventFilter(watched, event);
}

void CycleListWidget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() < 0) {
        scrollDown();
    } else {
        scrollUp();
    }
}

void CycleListWidget::leaveEvent(QEvent * /*event*/)
{
    m_upButton->hide();
    m_downButton->hide();
}

void CycleListWidget::resizeEvent(QResizeEvent * /*event*/)
{
    m_upButton->resize(this->width(), 34);
    m_downButton->resize(this->width(), 34);
    m_downButton->move(0, this->height() - 34);
}

void CycleListWidget::enterEvent(QEvent * /*event*/)
{
    m_upButton->show();
    m_downButton->show();
}

void CycleListWidget::createItems(const QVariantList &items)
{
    const int N = items.count();

    m_isCycle = (N > m_visibleNumber);
    if (m_isCycle) {
        addColumnItems(items);
        addColumnItems(items);

        m_currentIndex = items.count();

        QListWidget::scrollToItem(this->item(this->currentIndex() - m_visibleNumber / 2), QListWidget::PositionAtTop);
    } else {
        int n = m_visibleNumber / 2;  // add empty items to enable scrolling

        QVariantList emptyList;
        for (int i = 0; i < n; ++i) {
            emptyList << "";
        }

        addColumnItems(emptyList, true);
        addColumnItems(items);
        addColumnItems(emptyList, true);

        m_currentIndex = n;
    }
}

void CycleListWidget::addColumnItems(const QVariantList &items, bool disabled)
{
    for (auto i : items) {
        auto item = new QListWidgetItem(i.toString(), this);
        item->setSizeHint(this->m_itemSize);
        item->setTextAlignment(this->m_align | Qt::AlignVCenter);
        if (disabled) {
            item->setFlags(Qt::NoItemFlags);
        }

        this->addItem(item);
    }
}

// scroll up an item
void CycleListWidget::scrollUp()
{
    this->setCurrentIndex(m_currentIndex - 1);
    this->scrollToItem(this->currentItem());
}

// scroll down an item
void CycleListWidget::scrollDown()
{
    this->setCurrentIndex(m_currentIndex + 1);
    this->scrollToItem(this->currentItem());
}

void CycleListWidget::onItemClicked(QListWidgetItem *item)
{
    this->setCurrentIndex(this->row(item));
    this->scrollToItem(this->currentItem());
}
