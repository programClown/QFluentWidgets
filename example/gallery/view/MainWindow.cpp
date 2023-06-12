#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "GalleryTitleBar.h"
#include "HomeInterface.h"
#include "GalleryInterface.h"
#include "common/SignalBus.h"
#include "common/GalleryStyleSheet.h"
#include "components/AvatarWidget.h"

#include <Widgets/StackedWidget.h>
#include <DialogBox/Dialog.h>

#include <QPushButton>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qfileiconprovider.h>
#include <FramelessHelper/Core/utils.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include "shared/settings.h"

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

FRAMELESSHELPER_USE_NAMESPACE

using namespace Global;

FRAMELESSHELPER_STRING_CONSTANT(Geometry)
FRAMELESSHELPER_STRING_CONSTANT(State)
FRAMELESSHELPER_STRING_CONSTANT(DevicePixelRatio)

MainWindow::MainWindow(QWidget *parent, const Qt::WindowFlags flags) : FramelessMainWindow(parent, flags)
{
    initialize();

    m_hBoxLayout = new QHBoxLayout();
    setLayout(m_hBoxLayout);
    m_widgetLayout = new QHBoxLayout();

    m_stackWidget         = new StackedWidget(this);
    m_navigationInterface = new NavigationInterface(true, true, this);

    m_homeInterface = new HomeInterface(this);

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
    m_titleBar = new GalleryTitleBar(this);
    m_titleBar->setTitleLabelAlignment(Qt::AlignLeft);
    //    m_mainWindow = new Ui::MainWindow;
    //    m_mainWindow->setupUi(this);

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
    const auto titleBarLayout = static_cast<QHBoxLayout *>(m_titleBar->layout());
    titleBarLayout->insertWidget(0, mb);

    // setMenuWidget(): make the menu widget become the first row of the window.
    setMenuWidget(m_titleBar);

    FramelessWidgetsHelper *helper = FramelessWidgetsHelper::get(this);
    helper->setTitleBarWidget(m_titleBar);
#ifndef Q_OS_MACOS
    helper->setSystemButton(m_titleBar->minimizeButton(), SystemButtonType::Minimize);
    helper->setSystemButton(m_titleBar->maximizeButton(), SystemButtonType::Maximize);
    helper->setSystemButton(m_titleBar->closeButton(), SystemButtonType::Close);
#endif                              // Q_OS_MACOS
    helper->setHitTestVisible(mb);  // IMPORTANT!

    //    connect(m_mainWindow->pushButton, &QPushButton::clicked, this, [this] {});
    //    connect(m_mainWindow->pushButton_2, &QPushButton::clicked, this, [this] {});
}

void MainWindow::initLayout()
{
    m_hBoxLayout->setSpacing(0);
    m_hBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_hBoxLayout->addWidget(m_navigationInterface);
    m_hBoxLayout->addLayout(m_widgetLayout);
    m_hBoxLayout->setStretchFactor(m_widgetLayout, 1);

    m_widgetLayout->addWidget(m_stackWidget);
    m_widgetLayout->setContentsMargins(0, 48, 0, 0);

    connect(SignalBus::signalBus(), &SignalBus::switchToSampleCard, this, &MainWindow::switchToSample);
    connect(m_navigationInterface, &NavigationInterface::displayModeChanged, m_titleBar, &GalleryTitleBar::raise);
    m_titleBar->raise();
}

void MainWindow::initNavigation()
{
    m_navigationInterface->addItem(m_homeInterface->objectName(), NEWFLICON(FluentIcon, HOME), "Home", this,
                                   SLOT(homeInterfaceClicked()));

    m_navigationInterface->addSeparator();

    AvatarWidget *avatar = new AvatarWidget(":/resource/images/shoko.png");
    m_navigationInterface->addWidget("avatar", avatar, this, SLOT(showMessageBox()), NavigationItemPosition::BOTTOM);
    m_navigationInterface->setDefaultRouteKey(m_homeInterface->objectName());

    connect(m_stackWidget, &StackedWidget::currentWidgetChanged, this,
            [this](QWidget *w) { m_navigationInterface->setCurrentItem(w->objectName()); });
    m_navigationInterface->setCurrentItem(m_homeInterface->objectName());
    m_stackWidget->setCurrentIndex(0);
}

void MainWindow::initWindow()
{
    resize(960, 780);
    setMinimumWidth(760);

    setWindowTitle(tr("Gallery"));

    QIcon icon = QFileIconProvider().icon(QFileIconProvider::Computer);

    icon = QIcon(":/resource/images/logo.png");

    setWindowIcon(icon);

    //    setWindowIcon(icon);
    m_titleBar->setWindowIcon(icon);
    m_titleBar->setAttribute(Qt::WA_StyledBackground);

    QRect desk = QApplication::desktop()->availableGeometry();
    move(desk.width() / 2 - width() / 2, desk.height() / 2 - height() / 2);

    GalleryStyleSheet::apply(GalleryStyleSheet::MAIN_WINDOW, this);
}

void MainWindow::switchTo(QWidget *widget, bool triggerByUser)
{
    m_stackWidget->setCurrentWidget(widget, !triggerByUser);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    FRAMELESSHELPER_PREPEND_NAMESPACE(FramelessMainWindow)::resizeEvent(event);
}

void MainWindow::showMessageBox()
{
    qDebug() << __FUNCTION__ << __LINE__;

    MessageBox *m = new MessageBox(tr("This is a help message"),
                                   tr("You clicked a customized navigation widget. You can add more custom widgets by "
                                      "calling `NavigationInterface.addWidget()` "),
                                   this);
    m->exec();
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

void MainWindow::homeInterfaceClicked(bool user)
{
    qDebug() << __FUNCTION__ << __LINE__ << user;
    switchTo(m_homeInterface, user);
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
