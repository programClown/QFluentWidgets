#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Navigation/NavigationInterface.h"

#include <FramelessHelper/Widgets/framelessmainwindow.h>

#include <QFrame>

class QHBoxLayout;
class PopUpAniStackedWidget;
class GalleryTitleBar;
class FluentIcon;

class HomeInterface;
class IconInterface;
class BasicInputInterface;
class DateTimeInterface;
class DialogInterface;
class LayoutInterface;
class MenuInterface;
class MaterialInterface;
class ScrollInterface;
class StatusInfoInterface;
class TextInterface;
class ViewInterface;
class SettingInterface;

FRAMELESSHELPER_BEGIN_NAMESPACE
class StandardTitleBar;
FRAMELESSHELPER_END_NAMESPACE

class StackedWidget : public QFrame
{
    Q_OBJECT
public:
    StackedWidget(QWidget *parent = nullptr);

    void addWidget(QWidget *widget);

    void setCurrentWidget(QWidget *widget, bool popOut = false);

    void setCurrentIndex(int index, bool popOut = false);

    PopUpAniStackedWidget *view() const;

signals:
    void currentWidgetChanged(QWidget *);

private:
    QHBoxLayout *m_hBoxLayout;
    PopUpAniStackedWidget *m_view;
};

// namespace Ui
//{
//    class MainWindow;
//}

class MainWindow : public FRAMELESSHELPER_PREPEND_NAMESPACE(FramelessMainWindow)
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget *parent = nullptr, const Qt::WindowFlags flags = {});
    ~MainWindow() override;

    void waitReady();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initialize();

    void initLayout();

    void initNavigation();

    void addSubInterface(QWidget *interface, QString objectName, QSharedPointer<FluentIconBase> icon, QString text,
                         NavigationItemPosition position = NavigationItemPosition::SCROLL);

    void initWindow();

    void switchTo(QWidget *widget, bool triggerByUser = true);

    void resizeEvent(QResizeEvent *event) override;

    void switchToSample(QString routeKey, int index);

public slots:
    void showMessageBox();
    void homeInterfaceClicked();
    void iconInterfaceClicked();
    void basicInputInterfaceClicked();
    void dateTimeInterfaceClicked();
    void dialogInterfaceClicked();
    void layoutInterfaceClicked();
    void materialInterfaceClicked();
    void menuInterfaceClicked();
    void scrollInterfaceClicked();
    void statusInfoInterfaceClicked();
    void textInterfaceClicked();
    void viewInterfaceClicked();
    void settingInterfaceClicked();

private:
    GalleryTitleBar *m_titleBar = nullptr;
    //    Ui::MainWindow *m_mainWindow = nullptr;

    QHBoxLayout *m_hBoxLayout;
    QHBoxLayout *m_widgetLayout;
    StackedWidget *m_stackWidget;
    NavigationInterface *m_navigationInterface;

    HomeInterface *m_homeInterface;
    IconInterface *m_iconInterface;
    BasicInputInterface *m_basicInputInterface;
    DateTimeInterface *m_dateTimeInterface;
    DialogInterface *m_dialogInterface;
    LayoutInterface *m_layoutInterface;
    MenuInterface *m_menuInterface;
    MaterialInterface *m_materialInterface;
    ScrollInterface *m_scrollInterface;
    StatusInfoInterface *m_statusInfoInterface;
    TextInterface *m_textInterface;
    ViewInterface *m_viewInterface;
    SettingInterface *m_settingInterface;
};

#endif  // MAINWINDOW_H
