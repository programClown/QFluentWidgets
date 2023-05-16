#ifndef MENU_H
#define MENU_H

#include <QMenu>
#include <QListWidget>
#include <QProxyStyle>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QPair>
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QEventLoop>

class SmoothScroll;
class MenuActionListWidget;
class SubMenuItemWidget;
class CustomMenuStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit CustomMenuStyle(int iconSize = 14);

private:
    int m_iconSize;

    // QStyle interface
public:
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const override;
};

class DWMMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DWMMenu(const QString &title = "", QWidget *parent = nullptr);

    // QObject interface
public:
    virtual bool event(QEvent *event) override;

private:
    QScopedPointer<CustomMenuStyle> m_customMenuStyle;
};

class MenuSeparator : public QWidget
{
    Q_OBJECT
public:
    explicit MenuSeparator(QWidget *parent = nullptr);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

class RoundMenu : public QWidget
{

    Q_OBJECT
public:
    explicit RoundMenu(const QString &title = "", QWidget *parent = nullptr);
    void setShadowEffect(qreal blurRadius = 30, const QPointF &offset = QPoint(0, 8),
                         const QColor &color = QColor(0, 0, 0, 30));

    void setItemHeight(int itemHeight);
    void adjustSize();

    QString title() const;
    QIcon icon() const;
    void setIcon(const QIcon &icon);
    void clear();
    void removeAction(QAction *action);
    void addAction(QAction *const action);
    void insertAction(QAction *const before, QAction *const action);
    void addActions(const QList<QAction *> &actions);
    void insertActions(QAction *const before, const QList<QAction *> &actions);
    void setDefaultAction(QAction *const action);
    void addMenu(RoundMenu *const menu);
    void insertMenu(QAction *const before, RoundMenu *const menu);
    RoundMenu *parentMenu() const;
    QList<QAction *> menuActions() const;
    MenuActionListWidget *view() const;
    void addSeparator();

    virtual void exec(const QPoint &pos, bool ani = true);

protected:
    void setParentMenu(RoundMenu *parent, QListWidgetItem *item);
    QListWidgetItem *createActionItem(QAction *const action, QAction *const before = nullptr);
    bool hasItemIcon();
    QIcon createItemIcon(QAction *const action);
    QIcon createItemIcon(RoundMenu *const menu);
    QPair<QListWidgetItem *, SubMenuItemWidget *> createSubMenuItem(RoundMenu *const menu);
    void hideMenu(bool hideBySystem = false);
    void closeParentMenu();

    void mouseMoveEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    void initWidgets();

protected slots:
    void onShowMenuTimeOut();
    virtual void onItemClicked(QListWidgetItem *item);
    void onItemEntered(QListWidgetItem *item);
    void onSlideValueChanged(const QVariant &value);
    void onActionChanged();
    void showSubMenu(QListWidgetItem *const item);

signals:
    void closedSignal();

private:
    QString m_title;
    QIcon m_icon;
    QList<QAction *> m_actions;
    QList<RoundMenu *> m_subMenus;
    bool m_isSubMenu;
    RoundMenu *m_parentMenu;
    QListWidgetItem *m_menuItem;
    QListWidgetItem *m_lastHoverItem;
    QListWidgetItem *m_lastHoverSubMenuItem;
    bool m_isHideBySystem;
    int m_itemHeight;
    QHBoxLayout *m_hBoxLayout;
    MenuActionListWidget *m_view;
    QPropertyAnimation *m_ani;
    QTimer *m_timer;
    QGraphicsDropShadowEffect *m_shadowEffect;

    QEventLoop *m_eventLoop;  //模态堵塞使用
};

Q_DECLARE_METATYPE(QListWidgetItem *)
Q_DECLARE_METATYPE(QAction *)
Q_DECLARE_METATYPE(RoundMenu *)

class SubMenuItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SubMenuItemWidget(QMenu *menu, QListWidgetItem *item, QWidget *parent = nullptr);
    explicit SubMenuItemWidget(RoundMenu *menu, QListWidgetItem *item, QWidget *parent = nullptr);

    // QWidget interface
    RoundMenu *roundMenu() const;

protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

signals:
    void showMenuSig(QListWidgetItem *const);

private:
    QMenu *m_menu;
    RoundMenu *m_roundMenu;
    QListWidgetItem *m_item;
};

class MenuActionListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MenuActionListWidget(QWidget *parent = nullptr);

    void insertItem(int row, QListWidgetItem *item);
    void addItem(QListWidgetItem *item);
    QListWidgetItem *takeItem(int row);
    void adjustSize();
    void setItemHeight(int height);
    QMargins viewportMargins() { return QListWidget::viewportMargins(); }

    // QWidget interface
protected:
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    QScopedPointer<SmoothScroll> m_smoothScroll;
};

class EditMenu : public RoundMenu
{
    Q_OBJECT
public:
    explicit EditMenu(QWidget *parent = nullptr);

    void createActions();

    void exec(const QPoint &pos, bool ani = true) override;

    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *cancelAct;
    QAction *selectAllAct;
    QList<QAction *> actionList;

protected:
    virtual QString parentText()         = 0;
    virtual QString parentSelectedText() = 0;
};

class LineEditMenu : public EditMenu
{
    Q_OBJECT
public:
    explicit LineEditMenu(QLineEdit *parent);

    // RoundMenu interface
protected slots:
    void onItemClicked(QListWidgetItem *item) override;

    // EditMenu interface
protected:
    QString parentText() override;
    QString parentSelectedText() override;

private:
    int m_selectionStart;
    int m_selectionLength;
};

class TextEditMenu : public EditMenu
{
    Q_OBJECT
public:
    explicit TextEditMenu(QTextEdit *parent);
    explicit TextEditMenu(QPlainTextEdit *parent);

    // RoundMenu interface
protected slots:
    void onItemClicked(QListWidgetItem *item) override;

    // EditMenu interface
protected:
    QString parentText() override;
    QString parentSelectedText() override;

private:
    int m_selectionStart;
    int m_selectionLength;
    QTextEdit *m_textEdit;
    QPlainTextEdit *m_plainTextEdit;
};

#endif  // MENU_H
