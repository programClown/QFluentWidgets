#ifndef NAVIGATIONINTERFACE_H
#define NAVIGATIONINTERFACE_H

#include "NavigationPanel.h"

#include <QWidget>

class NavigationInterface : public QWidget
{
    Q_OBJECT
public:
    explicit NavigationInterface(bool showMenuButton = true, bool showReturnButton = true, QWidget *parent = nullptr);

    void addItem(const QString &routeKey, FluentIconBase *icon, const QString &text, const QObject *receiver,
                 const char *onClick, bool selectable = true,
                 NavigationItemPosition position = NavigationItemPosition::TOP);

    void addWidget(const QString &routeKey, NavigationWidget *widget, const QObject *receiver, const char *onClick,
                   NavigationItemPosition position = NavigationItemPosition::TOP);

    void addSeparator(NavigationItemPosition position = NavigationItemPosition::TOP);
    void removeWidget(const QString &routeKey);
    void setCurrentItem(const QString &name);
    void setDefaultRouteKey(const QString &routeKey);
    void setExpandWidth(int width);

    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

    NavigationPanel *panel;

signals:
    void displayModeChanged(NavigationDisplayMode);
};

#endif  // NAVIGATIONINTERFACE_H
