#ifndef NAVIGATIONPANEL_H
#define NAVIGATIONPANEL_H

#include "NavigationWidget.h"

#include <QBoxLayout>
#include <QFrame>
#include <QPropertyAnimation>

class ScrollArea;
class NavigationToolButton;
class NavigationHistory;
class NavigationWidget;

enum NavigationDisplayMode
{
    MINIMAL = 0,
    COMPACT = 1,
    EXPAND  = 2,
    MENU    = 3
};

enum NavigationItemPosition
{
    TOP    = 0,
    SCROLL = 1,
    BOTTOM = 2
};

class NavigationItemLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit NavigationItemLayout(QWidget *parent = nullptr) : QVBoxLayout(parent){};

    // QLayoutItem interface
public:
    void setGeometry(const QRect &rect) override
    {
        QVBoxLayout::setGeometry(rect);
        for (int i = 0; i < this->count(); ++i) {
            QLayoutItem *item       = this->itemAt(i);
            NavigationSeparator *ns = dynamic_cast<NavigationSeparator *>(item->widget());
            if (ns) {
                QRect geo = item->geometry();
                item->widget()->setGeometry(0, geo.y(), geo.width(), geo.height());
            }
        }
    }
};

class NavigationHistory : public QObject
{
    Q_OBJECT
public:
    explicit NavigationHistory(const QHash<QString, NavigationWidget *> &items, QObject *parent = nullptr);

    QString defaultRouteKey() const;
    void setDefaultRouteKey(const QString &key);

    void push(const QString &routeKey);
    void pop();
    void remove(const QString &routeKey, bool all = false);

protected:
    void navigate();

signals:
    void emptyChanged(bool);

private:
    QString m_defaultRouteKey;
    QStringList m_history;
    QHash<QString, NavigationWidget *> m_items;

    friend class NavigationPanel;
};

class NavigationPanel : public QFrame
{
    Q_OBJECT
public:
    explicit NavigationPanel(bool minimalEnabled = false, QWidget *parent = nullptr);

    /**
     * @brief add navigation item
     * @param routeKey: the unique name of item
     * @param icon: the icon of navigation item
     * @param text: the text of navigation item
     * @param onClick: the slot connected to item clicked signal
     * @param selectable: whether the item is selectable
     * @param position: where the button is added
     */
    void addItem(const QString &routeKey, FluentIconBase *icon, const QString &text, const QObject *receiver,
                 const char *onClick, bool selectable = true,
                 NavigationItemPosition position = NavigationItemPosition::TOP);

    /**
     * @brief add custom widget
     * @param routeKey: the unique name of item
     * @param widget: the custom widget to be added
     * @param onClick: the slot connected to item clicked signal
     * @param position: where the button is added
     */
    void addWidget(const QString &routeKey, NavigationWidget *widget, const QObject *receiver, const char *onClick,
                   NavigationItemPosition position = NavigationItemPosition::TOP);

    void removeWidget(const QString &routeKey);

    void setMenuButtonVisible(bool visible);
    void setReturnButtonVisible(bool visible);
    void setExpandWidth(int width);
    void expand();
    void collapse();
    void addSeparator(NavigationItemPosition position = NavigationItemPosition::TOP);
    void setCurrentItem(const QString &routeKey);
    int layoutMinHeight();
    void setDefaultRouteKey(const QString &routeKey);

    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

public:
    ScrollArea *scrollArea;
    QWidget *scrollWidget;

    NavigationToolButton *menuButton;
    NavigationToolButton *returnButton;

    NavigationItemLayout *vBoxLayout;
    NavigationItemLayout *topLayout;
    NavigationItemLayout *bottomLayout;
    NavigationItemLayout *scrollLayout;

    NavigationHistory *history;

    bool isMinimalEnabled;
    NavigationDisplayMode displayMode;

public slots:
    void toggle();

signals:
    void displayModeChanged(NavigationDisplayMode);

private:
    void initWidget();
    void initLayout();
    void addWidgetToLayout(NavigationWidget *widget, NavigationItemPosition position);
    void setWidgetCompacted(bool compacted);

private slots:
    void onWidgetClicked();
    void onExpandAniFinished();

private:
    QWidget *m_parent;
    bool m_isMenuButtonVisible;
    bool m_isReturnButtonVisible;

    //    QHash<QString, NavigationWidget *> m_items;
    QPropertyAnimation *m_expandAni;
    int m_expandWidth;
};

#endif  // NAVIGATIONPANEL_H
