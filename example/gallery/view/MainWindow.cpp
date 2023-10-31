#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "GalleryTitleBar.h"
#include "HomeInterface.h"
#include "IconInterface.h"
#include "BasicInputInterface.h"
#include "DateTimeInterface.h"
#include "DialogInterface.h"
#include "LayoutInterface.h"
#include "MenuInterface.h"
#include "MaterialInterface.h"
#include "ScrollInterface.h"
#include "StatusInfoInterface.h"
#include "TextInterface.h"
#include "ViewInterface.h"
#include "SettingInterface.h"

#include "GalleryInterface.h"
#include "common/SignalBus.h"
#include "common/GalleryStyleSheet.h"
#include "components/AvatarWidget.h"
#include "common/GalleryIcon.h"

#include <Widgets/StackedWidget.h>
#include <DialogBox/Dialog.h>
#include <Common/Config.h>

#include <QPushButton>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qfileiconprovider.h>
#include <FramelessHelper/Core/utils.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include "shared/settings.h"

#include <QFramelessWindow.h>

#include <QBoxLayout>
#include <QDebug>
#include <QDesktopWidget>

extern template void Settings::set<QRect>(const QString &, const QString &, const QRect &);
extern template void Settings::set<qreal>(const QString &, const QString &, const qreal &);
extern template void Settings::set<QByteArray>(const QString &, const QString &, const QByteArray &);

extern template QRect Settings::get<QRect>(const QString &, const QString &);
extern template qreal Settings::get<qreal>(const QString &, const QString &);
extern template QByteArray Settings::get<QByteArray>(const QString &, const QString &);

StackedWidget::StackedWidget(QWidget *parent) : QFrame(parent)
{
    m_hBoxLayout = new QHBoxLayout(this);
    m_view       = new PopUpAniStackedWidget(this);

    m_hBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_hBoxLayout->addWidget(m_view);

    connect(m_view, &PopUpAniStackedWidget::currentChanged, this,
            [this](int index) { emit currentWidgetChanged(m_view->widget(index)); });
}

void StackedWidget::addWidget(QWidget *widget)
{
    m_view->addWidget(widget);
}

///
/// @todo widget.verticalScrollBar().setValue(0)
///
void StackedWidget::setCurrentWidget(QWidget *widget, bool popOut)
{
    if (popOut) {
        m_view->setCurrentWidget(widget, true, false, 200, QEasingCurve::InQuad);
    } else {
        m_view->setCurrentWidget(widget, false, true, 300);
    }
}

void StackedWidget::setCurrentIndex(int index, bool popOut)
{
    setCurrentWidget(m_view->widget(index), popOut);
}

PopUpAniStackedWidget *StackedWidget::view() const
{
    return m_view;
}

FRAMELESSHELPER_USE_NAMESPACE

using namespace Global;

FRAMELESSHELPER_STRING_CONSTANT(Geometry)
FRAMELESSHELPER_STRING_CONSTANT(State)
FRAMELESSHELPER_STRING_CONSTANT(DevicePixelRatio)

MainWindow::MainWindow(QWidget *parent, const Qt::WindowFlags flags) : FramelessMainWindow(parent, flags)
{
    initialize();

    QWidget *w = new QWidget();

    m_hBoxLayout = new QHBoxLayout(w);
    setCentralWidget(w);

    m_widgetLayout = new QHBoxLayout();

    m_stackWidget         = new StackedWidget(this);
    m_navigationInterface = new NavigationInterface(true, true, this);

    m_homeInterface = new HomeInterface(this);
    //    m_iconInterface       = new IconInterface(this);
    m_basicInputInterface = new BasicInputInterface(this);
    m_dateTimeInterface   = new DateTimeInterface(this);
    m_dialogInterface     = new DialogInterface(this);
    m_layoutInterface     = new LayoutInterface(this);
    m_menuInterface       = new MenuInterface(this);
    m_materialInterface   = new MaterialInterface(this);
    m_scrollInterface     = new ScrollInterface(this);
    m_statusInfoInterface = new StatusInfoInterface(this);
    m_textInterface       = new TextInterface(this);
    m_viewInterface       = new ViewInterface(this);
    m_settingInterface    = new SettingInterface(this);

    initLayout();
    initNavigation();
    initWindow();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!parent()) {
        Settings::set({}, kGeometry, geometry());
        Settings::set({}, kState, saveState());
        Settings::set({}, kDevicePixelRatio, devicePixelRatioF());
    }
    FramelessMainWindow::closeEvent(event);
}

void MainWindow::initialize()
{
    QMenuBar *const mb = menuBar();
    mb->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    mb->setStyleSheet(FRAMELESSHELPER_STRING_LITERAL(R"(
QMenuBar {
    background-color: transparent;
}

QMenuBar::item {
    background: transparent;
}

QMenuBar::item:selected {
    background: #a8a8a8;
}

QMenuBar::item:pressed {
    background: #888888;
}
    )"));

    m_titleBar = new GalleryTitleBar(this);
    m_titleBar->setGeometry(QRect(46, 1, width() - 46, m_titleBar->height()));
    m_titleBar->hBoxLayout->insertWidget(0, mb);

    QFramelessHelper *helper = new QFramelessHelper(this);
    helper->setResizeEnabled(true);
    helper->setTitleBar(m_titleBar);

    setMinimumSize(QSize(800, 600));
}

void MainWindow::initLayout()
{
    m_hBoxLayout->setSpacing(0);
    m_hBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_hBoxLayout->addWidget(m_navigationInterface);
    m_hBoxLayout->addLayout(m_widgetLayout);
    m_hBoxLayout->setStretchFactor(m_widgetLayout, 1);

    m_widgetLayout->addWidget(m_stackWidget);
    m_widgetLayout->setContentsMargins(0, 32, 0, 0);

    connect(SignalBus::signalBus(), &SignalBus::switchToSampleCard, this, &MainWindow::switchToSample);
    connect(m_navigationInterface, &NavigationInterface::displayModeChanged, m_titleBar, &GalleryTitleBar::raise);
    m_titleBar->raise();
}

void MainWindow::initNavigation()
{
    m_homeInterface->setObjectName("homeInterface");
    m_stackWidget->addWidget(m_homeInterface);
    m_navigationInterface->addItem(m_homeInterface->objectName(), NEWFLICON(FluentIcon, HOME), "Home", this,
                                   SLOT(homeInterfaceClicked()));

    //    m_iconInterface->setObjectName("iconInterface");
    //    m_stackWidget->addWidget(m_iconInterface);
    //    m_navigationInterface->addItem(m_iconInterface->objectName(), NEWFLICON(GalleryIcon, EMOJI_TAB_SYMBOLS),
    //    "Icons",
    //                                   this, SLOT(iconInterfaceClicked()));

    m_basicInputInterface->setObjectName("basicINputInterface");
    m_stackWidget->addWidget(m_basicInputInterface);
    m_navigationInterface->addItem(m_basicInputInterface->objectName(), NEWFLICON(FluentIcon, CHECKBOX), "Basic input",
                                   this, SLOT(basicInputInterfaceClicked()));

    m_dateTimeInterface->setObjectName("dateTimeInterface");
    m_stackWidget->addWidget(m_dateTimeInterface);
    m_navigationInterface->addItem(m_dateTimeInterface->objectName(), NEWFLICON(FluentIcon, DATE_TIME), "Date & time",
                                   this, SLOT(dateTimeInterfaceClicked()));

    m_dialogInterface->setObjectName("dialogInterface");
    m_stackWidget->addWidget(m_dialogInterface);
    m_navigationInterface->addItem(m_dialogInterface->objectName(), NEWFLICON(FluentIcon, MESSAGE), "Dialogs", this,
                                   SLOT(dialogInterfaceClicked()));

    m_layoutInterface->setObjectName("layoutInterface");
    m_stackWidget->addWidget(m_layoutInterface);
    m_navigationInterface->addItem(m_layoutInterface->objectName(), NEWFLICON(FluentIcon, LAYOUT), "Layout", this,
                                   SLOT(layoutInterfaceClicked()));

    m_materialInterface->setObjectName("materialInterface");
    m_stackWidget->addWidget(m_materialInterface);
    m_navigationInterface->addItem(m_materialInterface->objectName(), NEWFLICON(FluentIcon, PALETTE), "Material", this,
                                   SLOT(materialInterfaceClicked()));

    m_menuInterface->setObjectName("menuInterface");
    m_stackWidget->addWidget(m_menuInterface);
    m_navigationInterface->addItem(m_menuInterface->objectName(), NEWFLICON(GalleryIcon, MENU), "Menu", this,
                                   SLOT(menuInterfaceClicked()));

    m_scrollInterface->setObjectName("scrollInterface");
    m_stackWidget->addWidget(m_scrollInterface);
    m_navigationInterface->addItem(m_scrollInterface->objectName(), NEWFLICON(FluentIcon, SCROLL), "Scrolling", this,
                                   SLOT(scrollInterfaceClicked()));

    m_statusInfoInterface->setObjectName("statusInfoInterface");
    m_stackWidget->addWidget(m_statusInfoInterface);
    m_navigationInterface->addItem(m_statusInfoInterface->objectName(), NEWFLICON(FluentIcon, CHAT), "Status & info",
                                   this, SLOT(statusInfoInterfaceClicked()));

    m_textInterface->setObjectName("textInterface");
    m_stackWidget->addWidget(m_textInterface);
    m_navigationInterface->addItem(m_textInterface->objectName(), NEWFLICON(GalleryIcon, TEXT), "Text", this,
                                   SLOT(textInterfaceClicked()));

    m_navigationInterface->addSeparator();

    AvatarWidget *avatar = new AvatarWidget(":/resource/images/shoko.png");
    m_navigationInterface->addWidget("avatar", avatar, this, SLOT(showMessageBox()), NavigationItemPosition::BOTTOM);

    m_viewInterface->setObjectName("viewInterface");
    m_stackWidget->addWidget(m_viewInterface);
    m_navigationInterface->addItem(m_viewInterface->objectName(), NEWFLICON(GalleryIcon, GRID), "View", this,
                                   SLOT(viewInterfaceClicked()), NavigationItemPosition::BOTTOM);

    m_settingInterface->setObjectName("settingInterface");
    m_stackWidget->addWidget(m_settingInterface);
    m_navigationInterface->addItem(m_settingInterface->objectName(), NEWFLICON(FluentIcon, SETTING), "Setting", this,
                                   SLOT(settingInterfaceClicked()), false, NavigationItemPosition::BOTTOM);

    m_navigationInterface->setDefaultRouteKey(m_homeInterface->objectName());

    connect(m_stackWidget, &StackedWidget::currentWidgetChanged, this, [this](QWidget *w) {
        m_navigationInterface->setCurrentItem(w->objectName());
        qDebug() << "current" << m_stackWidget->view()->currentIndex();
    });
    m_navigationInterface->setCurrentItem(m_homeInterface->objectName());
    m_stackWidget->setCurrentIndex(0);
}

void MainWindow::addSubInterface(QWidget *interface, QString objectName, QSharedPointer<FluentIconBase> icon,
                                 QString text, NavigationItemPosition position)
{
}

void MainWindow::initWindow()
{
    resize(960, 780);
    setMinimumWidth(760);

    setWindowTitle(tr("Gallery"));

    QIcon icon = QIcon(":/resource/images/logo.png");
    setWindowIcon(icon);
    m_titleBar->setIcon(icon);

    m_titleBar->setAttribute(Qt::WA_StyledBackground);

    QRect desk = QApplication::desktop()->availableGeometry();
    move(desk.width() / 2 - width() / 2, desk.height() / 2 - height() / 2);

    GalleryStyleSheet::apply(GalleryStyleSheet::MAIN_WINDOW, this);
}

void MainWindow::switchTo(QWidget *widget, bool triggerByUser)
{
    qDebug() << __FUNCTION__ << __LINE__ << widget->objectName();
    m_stackWidget->setCurrentWidget(widget, !triggerByUser);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    FRAMELESSHELPER_PREPEND_NAMESPACE(FramelessMainWindow)::resizeEvent(event);

    m_navigationInterface->resize(m_navigationInterface->width(), height());
    m_titleBar->resize(width() - 46, m_titleBar->height());
}

void MainWindow::showMessageBox()
{
    MessageBox *m = new MessageBox(tr("This is a help message"),
                                   tr("You clicked a customized navigation widget. You can add more custom widgets by "
                                      "calling `NavigationInterface.addWidget()` "),
                                   this);
    m->exec();
}

void MainWindow::homeInterfaceClicked()
{
    switchTo(m_homeInterface);
}

void MainWindow::iconInterfaceClicked()
{
    switchTo(m_iconInterface);
}

void MainWindow::basicInputInterfaceClicked()
{
    switchTo(m_basicInputInterface);
}

void MainWindow::dateTimeInterfaceClicked()
{
    switchTo(m_dateTimeInterface);
}

void MainWindow::dialogInterfaceClicked()
{
    switchTo(m_dialogInterface);
}

void MainWindow::layoutInterfaceClicked()
{
    switchTo(m_layoutInterface);
}

void MainWindow::materialInterfaceClicked()
{
    switchTo(m_materialInterface);
}

void MainWindow::menuInterfaceClicked()
{
    switchTo(m_menuInterface);
}

void MainWindow::scrollInterfaceClicked()
{
    switchTo(m_scrollInterface);
}

void MainWindow::statusInfoInterfaceClicked()
{
    switchTo(m_statusInfoInterface);
}

void MainWindow::textInterfaceClicked()
{
    switchTo(m_textInterface);
}

void MainWindow::viewInterfaceClicked()
{
    switchTo(m_viewInterface);
}

void MainWindow::settingInterfaceClicked()
{
    switchTo(m_settingInterface);
}

void MainWindow::switchToSample(QString routeKey, int index)
{
    QList<GalleryInterface *> list = findChildren<GalleryInterface *>();
    for (GalleryInterface *w : list) {
        if (w->objectName() == routeKey) {
            m_stackWidget->setCurrentWidget(w);
            w->scrollToCard(index);
        }
    }
}

void MainWindow::waitReady()
{
    FramelessWidgetsHelper *helper = FramelessWidgetsHelper::get(this);
    helper->waitForReady();
    const auto savedGeometry = Settings::get<QRect>({}, kGeometry);
    if (savedGeometry.isValid() && !parent()) {
        const auto savedDpr = Settings::get<qreal>({}, kDevicePixelRatio);
        // Qt doesn't support dpr < 1.
        const qreal oldDpr = std::max(savedDpr, qreal(1));
        const qreal scale  = (devicePixelRatioF() / oldDpr);
        setGeometry({ savedGeometry.topLeft() * scale, savedGeometry.size() * scale });
    } else {
        helper->moveWindowToDesktopCenter();
    }
    const QByteArray savedState = Settings::get<QByteArray>({}, kState);
    if (!savedState.isEmpty() && !parent()) {
        restoreState(savedState);
    }
}
