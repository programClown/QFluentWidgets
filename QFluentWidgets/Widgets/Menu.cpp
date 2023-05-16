#include "Menu.h"
#include "Common/StyleSheet.h"
#include "Common/Icon.h"
#include "Common/SmoothScroll.h"
#include "QFluentWidgets.h"

#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QScreen>
#include <QEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>

CustomMenuStyle::CustomMenuStyle(int iconSize) : QProxyStyle(), m_iconSize(iconSize) { }

int CustomMenuStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    if (metric == QStyle::PM_SmallIconSize) {
        return m_iconSize;
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

DWMMenu::DWMMenu(const QString &title, QWidget *parent) : QMenu(title, parent), m_customMenuStyle(new CustomMenuStyle)
{
    // WindowEffect 阴影效果
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_StyledBackground);
    setStyle(m_customMenuStyle.data());

    FluentStyleSheet::apply("MENU", this);
}

bool DWMMenu::event(QEvent *event)
{
    if (event->type() == QEvent::WinIdChange) {
        // windowEffect.addMenuShadowEffect(self.winId())
    }

    return QMenu::event(event);
}

MenuSeparator::MenuSeparator(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(9);
}

void MenuSeparator::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    int c = QFWIns.isDarkTheme() ? 255 : 0;
    QPen pen(QColor(c, c, c, 25), 1);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.drawLine(0, 4, width(), 4);
}

/**
 * @brief Round corner menu
 * @param parent
 */
RoundMenu::RoundMenu(const QString &title, QWidget *parent)
    : QWidget(parent),
      m_title(title),
      m_isSubMenu(false),
      m_parentMenu(nullptr),
      m_menuItem(nullptr),
      m_lastHoverItem(nullptr),
      m_lastHoverSubMenuItem(nullptr),
      m_isHideBySystem(true),
      m_itemHeight(28),
      m_hBoxLayout(new QHBoxLayout(this)),
      m_view(new MenuActionListWidget(this)),
      m_ani(new QPropertyAnimation(this, "pos", this)),
      m_timer(new QTimer(this)),
      m_shadowEffect(nullptr),
      m_eventLoop(nullptr)
{
    initWidgets();
}

/// add shadow to dialog
void RoundMenu::setShadowEffect(qreal blurRadius, const QPointF &offset, const QColor &color)
{
    m_shadowEffect = new QGraphicsDropShadowEffect(m_view);
    m_shadowEffect->setBlurRadius(blurRadius);
    m_shadowEffect->setOffset(offset);
    m_shadowEffect->setColor(color);
    m_view->setGraphicsEffect(nullptr);
    m_view->setGraphicsEffect(m_shadowEffect);
}

void RoundMenu::initWidgets()
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    m_timer->setSingleShot(true);
    m_timer->setInterval(400);
    connect(m_timer, &QTimer::timeout, this, &RoundMenu::onShowMenuTimeOut);

    setShadowEffect();
    m_hBoxLayout->setContentsMargins(12, 8, 12, 20);
    m_hBoxLayout->addWidget(m_view, 1, Qt::AlignCenter);
    setLayout(m_hBoxLayout);

    FluentStyleSheet::apply("MENU", this);
    connect(m_view, &MenuActionListWidget::itemClicked, this, &RoundMenu::onItemClicked);
    connect(m_view, &MenuActionListWidget::itemEntered, this, &RoundMenu::onItemEntered);
    connect(m_ani, &QPropertyAnimation::valueChanged, this, &RoundMenu::onSlideValueChanged);
}

QString RoundMenu::title() const
{
    return m_title;
}

QIcon RoundMenu::icon() const
{
    return m_icon;
}

void RoundMenu::setIcon(const QIcon &icon)
{
    m_icon = icon;
}

/// clear all actions
void RoundMenu::clear()
{
    while (!m_actions.isEmpty()) {
        removeAction(m_actions.last());
    }
}

/// remove action from menu
void RoundMenu::removeAction(QAction *action)
{
    if (!m_actions.contains(action)) {
        return;
    }

    int index = m_actions.indexOf(action);
    m_actions.removeAt(index);
    action->setProperty("item", 0);
    QListWidgetItem *item = m_view->takeItem(index);
    item->setData(Qt::UserRole, 0);

    // delete widget
    QWidget *widget = m_view->itemWidget(item);
    if (widget) {
        widget->deleteLater();
        widget = nullptr;
    }

    delete item;
    action->deleteLater();
}

/// add action to menu
void RoundMenu::addAction(QAction *const action)
{
    QListWidgetItem *item = createActionItem(action);
    if (!item) {
        return;
    }
    m_view->addItem(item);
    adjustSize();
}

void RoundMenu::insertAction(QAction *const before, QAction *const action)
{
    if (!m_actions.contains(before)) {
        return;
    }

    QListWidgetItem *beforeItem = before->property("item").value<QListWidgetItem *>();
    if (!beforeItem) {
        return;
    }

    int index             = m_view->row(beforeItem);
    QListWidgetItem *item = createActionItem(action, before);
    m_view->insertItem(index, item);
    adjustSize();
}

void RoundMenu::addActions(const QList<QAction *> &actions)
{
    for (auto a : actions) {
        addAction(a);
    }
}

void RoundMenu::insertActions(QAction *const before, const QList<QAction *> &actions)
{
    for (auto a : actions) {
        insertAction(before, a);
    }
}

void RoundMenu::setDefaultAction(QAction *const action)
{
    if (!m_actions.contains(action)) {
        return;
    }

    int index = m_actions.indexOf(action);
    m_view->setCurrentRow(index);
}

void RoundMenu::addMenu(RoundMenu *const menu)
{
    if (!menu) {
        return;
    }

    auto itemW = createSubMenuItem(menu);
    if (itemW.first == nullptr || itemW.second == nullptr) {
        return;
    }

    m_view->addItem(itemW.first);
    m_view->setItemWidget(itemW.first, itemW.second);
    this->adjustSize();
}

void RoundMenu::insertMenu(QAction *const before, RoundMenu *const menu)
{
    if (!menu) {
        qCritical() << "`menu` should be an instance of `RoundMenu`.";
        return;
    }

    if (!m_actions.contains(before)) {
        qCritical() << "`before` should be in menu action list";
        return;
    }

    auto itemW = createSubMenuItem(menu);
    if (itemW.first == nullptr || itemW.second == nullptr) {
        return;
    }
    m_view->insertItem(m_view->row(before->property("item").value<QListWidgetItem *>()), itemW.first);
    m_view->setItemWidget(itemW.first, itemW.second);
    this->adjustSize();
}

void RoundMenu::setItemHeight(int itemHeight)
{
    if (m_itemHeight == itemHeight) {
        return;
    }
    m_itemHeight = itemHeight;
    m_view->setItemHeight(itemHeight);
}

void RoundMenu::adjustSize()
{
    QMargins m = this->layout()->contentsMargins();
    int w      = m_view->width() + m.left() + m.right();
    int h      = m_view->height() + m.top() + m.bottom();
    setFixedSize(w, h);
}

void RoundMenu::setParentMenu(RoundMenu *parent, QListWidgetItem *item)
{
    m_parentMenu = parent;
    m_menuItem   = item;
    m_isSubMenu  = parent == nullptr ? false : true;
}

QListWidgetItem *RoundMenu::createActionItem(QAction *const action, QAction *const before)
{
    if (!before) {
        m_actions.append(action);
    } else if (m_actions.contains(before)) {
        int index = m_actions.indexOf(before);
        m_actions.insert(index, action);
    } else {
        qCritical() << QString("`before`  is not in the action list");
        return nullptr;
    }

    QListWidgetItem *item = new QListWidgetItem(createItemIcon(action), action->text());
    int w                 = 0;
    if (!hasItemIcon()) {
        w = 28 + m_view->fontMetrics().width(action->text());
    } else {
        // add a blank character to increase space between icon and text
        item->setText(" " + item->text());
        w = 60 + m_view->fontMetrics().width(item->text());
    }

    item->setSizeHint(QSize(w, m_itemHeight));
    item->setData(Qt::UserRole, QVariant::fromValue<QAction *>(action));
    action->setProperty("item", QVariant::fromValue<QListWidgetItem *>(item));
    connect(action, &QAction::changed, this, &RoundMenu::onActionChanged);
    return item;
}

bool RoundMenu::hasItemIcon()
{
    for (auto a : m_actions) {
        if (!a->icon().isNull()) {
            return true;
        }
    }

    for (auto m : m_subMenus) {
        if (!m->icon().isNull()) {
            return true;
        }
    }

    return false;
}

QIcon RoundMenu::createItemIcon(QAction *const action)
{
    bool hasIcon = hasItemIcon();

    if (hasIcon && action->icon().isNull()) {
        QPixmap pixmap(m_view->iconSize());
        pixmap.fill(Qt::transparent);
        return QIcon(pixmap);
    } else if (!hasIcon) {
        return QIcon();
    }

    QIcon icon(new MenuIconEngine(action->icon()));
    return icon;
}

QIcon RoundMenu::createItemIcon(RoundMenu *const menu)
{
    bool hasIcon = hasItemIcon();

    if (hasIcon && menu->icon().isNull()) {
        QPixmap pixmap(m_view->iconSize());
        pixmap.fill(Qt::transparent);
        return QIcon(pixmap);
    } else if (!hasIcon) {
        return QIcon();
    }

    QIcon icon(new MenuIconEngine(menu->icon()));
    return icon;
}

QPair<QListWidgetItem *, SubMenuItemWidget *> RoundMenu::createSubMenuItem(RoundMenu *const menu)
{
    if (!menu) {
        return QPair<QListWidgetItem *, SubMenuItemWidget *>(nullptr, nullptr);
    }

    m_subMenus.append(menu);
    QListWidgetItem *item = new QListWidgetItem(createItemIcon(menu), menu->title());
    int w                 = 0;
    if (!hasItemIcon()) {
        w = 48 + m_view->fontMetrics().width(menu->title());
    } else {
        // add a blank character to increase space between icon and text
        item->setText(" " + item->text());
        w = 60 + m_view->fontMetrics().width(item->text());
    }

    // add submenu item
    menu->setParentMenu(this, item);
    item->setSizeHint(QSize(w, m_itemHeight));
    item->setData(Qt::UserRole, QVariant::fromValue<RoundMenu *>(menu));
    SubMenuItemWidget *subw = new SubMenuItemWidget(menu, item, this);
    connect(subw, &SubMenuItemWidget::showMenuSig, this, &RoundMenu::showSubMenu);
    subw->resize(item->sizeHint());

    return QPair<QListWidgetItem *, SubMenuItemWidget *>(item, subw);
}

void RoundMenu::hideMenu(bool hideBySystem)
{
    m_isHideBySystem = hideBySystem;
    m_view->clearSelection();
    if (m_isSubMenu) {
        hide();
    } else {
        close();
    }
}

void RoundMenu::closeParentMenu()
{
    RoundMenu *menu = this;
    while (menu->m_parentMenu) {
        menu->close();
        menu = menu->parentMenu();
    }

    menu->close();
}

void RoundMenu::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isSubMenu) {
        return;
    }

    // hide submenu when mouse moves out of submenu item
    QPoint pos                 = event->globalPos();
    MenuActionListWidget *view = m_parentMenu->view();

    // get the rect of menu item
    QMargins margin = view->viewportMargins();
    QRect rect      = view->visualItemRect(m_menuItem).translated(view->mapToGlobal(QPoint()));
    rect            = rect.translated(margin.left(), margin.top() + 2);
    if (m_parentMenu->geometry().contains(pos) && !rect.contains(pos) && !this->geometry().contains(pos)) {
        view->clearSelection();
        hideMenu(false);

        // update style
        int index = view->row(m_menuItem);
        if (index > 0) {
            view->item(index - 1)->setFlags(Qt::ItemIsEnabled);
        }
        if (index < view->count() - 1) {
            view->item(index + 1)->setFlags(Qt::ItemIsEnabled);
        }
    }
}

void RoundMenu::closeEvent(QCloseEvent *event)
{
    // 关闭窗口时结束事件循环，在exec()方法中返回选择结果;
    if (m_eventLoop != nullptr) {
        m_eventLoop->exit();
    }

    event->accept();
    emit closedSignal();
}

void RoundMenu::hideEvent(QHideEvent *event)
{
    if (m_isHideBySystem && m_isSubMenu) {
        closeParentMenu();
    }

    m_isHideBySystem = true;
    event->accept();
}

void RoundMenu::onShowMenuTimeOut()
{
    if ((m_lastHoverSubMenuItem == nullptr) || (m_lastHoverItem == m_lastHoverSubMenuItem)) {
        return;
    }

    SubMenuItemWidget *sub = qobject_cast<SubMenuItemWidget *>(m_view->itemWidget(m_lastHoverSubMenuItem));
    RoundMenu *w           = sub->roundMenu();
    if (w && w->parentMenu()->isHidden()) {
        return;
    }

    QPoint pos = w->mapToGlobal(QPoint(w->width() + 5, -5));
    w->exec(pos);
}

void RoundMenu::onItemClicked(QListWidgetItem *item)
{
    QAction *action = item->data(Qt::UserRole).value<QAction *>();
    if (!m_actions.contains(action)) {
        return;
    }

    hideMenu();

    if (!m_isSubMenu) {
        action->trigger();
        return;
    }

    // close parent menu
    closeParentMenu();
    action->trigger();
}

void RoundMenu::onItemEntered(QListWidgetItem *item)
{
    m_lastHoverItem = item;
    RoundMenu *menu = item->data(Qt::UserRole).value<RoundMenu *>();
    if (!menu) {
        return;
    }
    showSubMenu(item);
}

void RoundMenu::onSlideValueChanged(const QVariant &value)
{
    QMargins m = this->layout()->contentsMargins();
    int w      = m_view->width() + m.left() + m.right() + 120;
    int h      = m_view->height() + m.top() + m.bottom() + 20;
    int y      = m_ani->endValue().toPoint().y() - value.toPoint().y();
    this->setMask(QRegion(0, y, w, h));
}

void RoundMenu::onActionChanged()
{
    QAction *action       = static_cast<QAction *>(sender());
    QListWidgetItem *item = action->property("item").value<QListWidgetItem *>();
    item->setIcon(createItemIcon(action));

    int w = 0;
    if (!hasItemIcon()) {
        item->setText(action->text());
        w = 28 + m_view->fontMetrics().width(action->text());
    } else {
        // add a blank character to increase space between icon and text
        item->setText(" " + action->text());
        w = 60 + m_view->fontMetrics().width(item->text());
    }

    item->setSizeHint(QSize(w, m_itemHeight));
    m_view->adjustSize();
    this->adjustSize();
}

void RoundMenu::showSubMenu(QListWidgetItem *const item)
{
    m_lastHoverItem        = item;
    m_lastHoverSubMenuItem = item;

    // delay 400 ms to anti-shake
    m_timer->stop();
    m_timer->start();
}

QList<QAction *> RoundMenu::menuActions() const
{
    return m_actions;
}

MenuActionListWidget *RoundMenu::view() const
{
    return m_view;
}

void RoundMenu::addSeparator()
{
    QMargins m = m_view->viewportMargins();
    int w      = m_view->width() - m.left() - m.right();

    // icon separator
    MenuSeparator *separator = new MenuSeparator(m_view);
    separator->resize(w, separator->height());

    // add separator to list widget
    QListWidgetItem *item = new QListWidgetItem(m_view);
    item->setFlags(Qt::NoItemFlags);
    item->setSizeHint(QSize(w, separator->height()));
    m_view->addItem(item);
    m_view->setItemWidget(item, separator);
    this->adjustSize();
}

RoundMenu *RoundMenu::parentMenu() const
{
    return m_parentMenu;
}

/**
 * @brief show menu
 * @param pos: pop-up position
 * @param ani: Whether to show pop-up animation
 */
void RoundMenu::exec(const QPoint &pos, bool ani)
{
    if (isVisible()) {
        return;
    }

    QPoint showPos;

    QRect rect = QApplication::screenAt(QCursor::pos())->availableGeometry();
    int w      = width() + 5;
    int h      = height() + 5;
    showPos.setX(qMin(pos.x() - layout()->contentsMargins().left(), rect.right() - w));
    showPos.setY(qMin(pos.y() - 4, rect.bottom() - h));

    if (ani) {
        m_ani->setStartValue(showPos - QPoint(0, int(h / 2)));
        m_ani->setEndValue(pos);
        m_ani->setDuration(250);
        m_ani->setEasingCurve(QEasingCurve::OutQuad);
        m_ani->start();
    } else {
        move(showPos);
    }

    this->setWindowModality(Qt::ApplicationModal);  //设置为模态状态

    show();

    if (m_isSubMenu) {
        m_menuItem->setSelected(true);

        // temporarily disable item to change style
        MenuActionListWidget *view = m_parentMenu->view();
        int index                  = view->row(m_menuItem);
        if (index > 0) {
            view->item(index - 1)->setFlags(Qt::NoItemFlags);
        }
        if (index < view->count() - 1) {
            view->item(index + 1)->setFlags(Qt::NoItemFlags);
        }
    }

    // 使用事件循环QEventLoop
    // ,不让exec()方法结束，在用户选择确定或者取消后，关闭窗口结束事件循环，并返回最后用户选择的结果;
    // 根据返回结果得到用户按下了确定还是取消，采取相应的操作。从而模拟出QDialog类的exec()方法;
    m_eventLoop = new QEventLoop(this);
    m_eventLoop->exec();
}

SubMenuItemWidget::SubMenuItemWidget(QMenu *menu, QListWidgetItem *item, QWidget *parent)
    : QWidget(parent), m_menu(menu), m_roundMenu(nullptr), m_item(item)
{
}

SubMenuItemWidget::SubMenuItemWidget(RoundMenu *menu, QListWidgetItem *item, QWidget *parent)
    : QWidget(parent), m_menu(nullptr), m_roundMenu(menu), m_item(item)
{
}

void SubMenuItemWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    emit showMenuSig(m_item);
}

void SubMenuItemWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    // draw right arrow
    // NOTE: 未完成
    FluentIcon ficon(FluentIcon::CHEVRON_RIGHT);
    ficon.render(&painter, QRect(width() - 10, height() / 2 - 9 / 2, 9, 9));
}

RoundMenu *SubMenuItemWidget::roundMenu() const
{
    return m_roundMenu;
}

MenuActionListWidget::MenuActionListWidget(QWidget *parent) : QListWidget(parent)
{
    setViewportMargins(5, 6, 5, 6);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setTextElideMode(Qt::ElideNone);
    setDragEnabled(false);
    setMouseTracking(true);
    setVerticalScrollMode(ScrollPerPixel);
    setIconSize(QSize(14, 14));
    m_smoothScroll.reset(new SmoothScroll(this, Qt::Vertical));
    setStyleSheet("MenuActionListWidget{font: 14px \"Segoe UI\", \"Microsoft YaHei\"}");
}

void MenuActionListWidget::insertItem(int row, QListWidgetItem *item)
{
    QListWidget::insertItem(row, item);
    adjustSize();
}

void MenuActionListWidget::addItem(QListWidgetItem *item)
{
    QListWidget::addItem(item);
    adjustSize();
}

QListWidgetItem *MenuActionListWidget::takeItem(int row)
{
    QListWidgetItem *item = QListWidget::takeItem(row);
    adjustSize();
    return item;
}

void MenuActionListWidget::adjustSize()
{
    QSize size;
    for (int i = 0; i < this->count(); i++) {
        QSize s = this->item(i)->sizeHint();
        size.setWidth(qMax(s.width(), size.width()));
        size.setHeight(size.height() + s.height());
    }

    // adjust the height of viewport
    QSize ss = QApplication::screenAt(QCursor::pos())->availableSize();
    int w    = ss.width() - 100;
    int h    = ss.height() - 100;
    QSize vSize(qMin(w - 12, size.width()), qMin(h - 12, size.height()));
    viewport()->adjustSize();

    // adjust the height of list widget
    QMargins m = viewportMargins();
    size += QSize(m.left() + m.right() + 2, m.top() + m.bottom());
    size.setHeight(qMin(h, size.height() + 3));
    size.setWidth(qMin(w, size.width()));
    setFixedSize(size);
}

void MenuActionListWidget::setItemHeight(int height)
{
    for (int i = 0; i < this->count(); ++i) {
        QListWidgetItem *item = this->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(), height));
    }
    this->adjustSize();
}

void MenuActionListWidget::wheelEvent(QWheelEvent *event)
{
    // only process the wheel events triggered by mouse
    if ((m_smoothScroll->smoothMode() == SmoothMode::NO_SMOOTH) || (abs(event->angleDelta().y()) % 120 != 0)) {
        QListWidget::wheelEvent(event);
    } else {
        m_smoothScroll->wheelEvent(event);
    }
}

EditMenu::EditMenu(QWidget *parent)
    : RoundMenu("", parent),
      cutAct(nullptr),
      copyAct(nullptr),
      pasteAct(nullptr),
      cancelAct(nullptr),
      selectAllAct(nullptr)
{
    createActions();
}

void EditMenu::createActions()
{
    cutAct = new QAction(QIcon(":/qfluentwidgets/images/icons/Cut_black.svg"), tr("Cut"), this);
    cutAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    copyAct = new QAction(QIcon(":/qfluentwidgets/images/icons/Copy_black.svg"), tr("Copy"), this);
    copyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    pasteAct = new QAction(QIcon(":/qfluentwidgets/images/icons/Paste_black.svg"), tr("Paste"), this);
    pasteAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    cancelAct = new QAction(QIcon(":/qfluentwidgets/images/icons/Cancel_black.svg"), tr("Cancel"), this);
    cancelAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    selectAllAct = new QAction(tr("Select all"), this);
    selectAllAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    actionList << cutAct << copyAct << pasteAct << cancelAct << selectAllAct;
}

void EditMenu::exec(const QPoint &pos, bool ani)
{
    //    clear();
    //    createActions();

    if (QApplication::clipboard()->mimeData()->hasText()) {
        if (!parentText().isEmpty()) {
            if (!parentSelectedText().isEmpty()) {
                addActions(actionList);
            } else {
                for (int i = 2; i < actionList.count(); ++i) {
                    addAction(actionList.at(i));
                }
            }
        } else {
            addAction(pasteAct);
        }
    } else {
        if (!parentText().isEmpty()) {
            if (!parentSelectedText().isEmpty()) {
                for (int i = 0; i < actionList.count(); ++i) {
                    if (i != 2) {
                        addAction(actionList.at(i));
                    }
                }
            } else {
                for (int i = 3; i < actionList.count(); ++i) {
                    addAction(actionList.at(i));
                }
            }
        } else {
            return;
        }
    }

    RoundMenu::exec(pos, ani);
}

LineEditMenu::LineEditMenu(QLineEdit *parent) : EditMenu(parent)
{
    m_selectionStart  = parent->selectionStart();
    m_selectionLength = parent->selectionLength();
}

void LineEditMenu::onItemClicked(QListWidgetItem *item)
{
    QLineEdit *p = qobject_cast<QLineEdit *>(parent());
    if (m_selectionStart >= 0) {
        p->setSelection(m_selectionStart, m_selectionLength);
    }

    EditMenu::onItemClicked(item);
}

QString LineEditMenu::parentText()
{
    QLineEdit *p = qobject_cast<QLineEdit *>(parent());
    return p->text();
}

QString LineEditMenu::parentSelectedText()
{
    QLineEdit *p = qobject_cast<QLineEdit *>(parent());
    return p->selectedText();
}

TextEditMenu::TextEditMenu(QTextEdit *parent) : EditMenu(parent), m_textEdit(parent), m_plainTextEdit(nullptr)
{
    m_selectionStart  = parent->textCursor().selectionStart();
    m_selectionLength = parent->textCursor().selectionEnd() - parent->textCursor().selectionStart() + 1;
}

TextEditMenu::TextEditMenu(QPlainTextEdit *parent) : EditMenu(parent), m_textEdit(nullptr), m_plainTextEdit(parent)
{
    m_selectionStart  = parent->textCursor().selectionStart();
    m_selectionLength = parent->textCursor().selectionEnd() - parent->textCursor().selectionStart() + 1;
}

void TextEditMenu::onItemClicked(QListWidgetItem *item)
{
    if (m_selectionStart >= 0) {
        QTextCursor cursor;
        if (m_textEdit) {
            cursor = m_textEdit->textCursor();
        } else {
            cursor = m_plainTextEdit->textCursor();
        }
        cursor.setPosition(m_selectionStart);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, m_selectionLength);
    }

    EditMenu::onItemClicked(item);
}

QString TextEditMenu::parentText()
{
    if (m_textEdit) {
        return m_textEdit->toPlainText();
    } else {
        return m_plainTextEdit->toPlainText();
    }
}

QString TextEditMenu::parentSelectedText()
{
    if (m_textEdit) {
        return m_textEdit->textCursor().selectedText();
    } else {
        return m_plainTextEdit->textCursor().selectedText();
    }
}
