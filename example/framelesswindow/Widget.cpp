#include "Widget.h"
#include <QtCore/qdatetime.h>
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    #include <QtGui/qshortcut.h>
#else
    #include <QtWidgets/qshortcut.h>
#endif
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qfileiconprovider.h>
#include <FramelessHelper/Core/framelessmanager.h>
#include <FramelessHelper/Core/utils.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include "shared/settings.h"

extern template void Settings::set<QRect>(const QString &, const QString &, const QRect &);
extern template void Settings::set<qreal>(const QString &, const QString &, const qreal &);

extern template QRect Settings::get<QRect>(const QString &, const QString &);
extern template qreal Settings::get<qreal>(const QString &, const QString &);

FRAMELESSHELPER_USE_NAMESPACE

using namespace Global;

FRAMELESSHELPER_STRING_CONSTANT(Geometry)
FRAMELESSHELPER_STRING_CONSTANT(DevicePixelRatio)

Widget::Widget(QWidget *parent) : FramelessWidget(parent)
{
    initialize();
    startTimer(500);
    connect(FramelessManager::instance(), &FramelessManager::systemThemeChanged, this, &Widget::updateStyleSheet);
}

Widget::~Widget() = default;

void Widget::timerEvent(QTimerEvent *event)
{
    FramelessWidget::timerEvent(event);
    if (m_clockLabel) {
        m_clockLabel->setText(QTime::currentTime().toString(FRAMELESSHELPER_STRING_LITERAL("hh:mm:ss")));
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    if (!parent()) {
        const QString objName = objectName();
        Settings::set(objName, kGeometry, geometry());
        Settings::set(objName, kDevicePixelRatio, devicePixelRatioF());
    }
    FramelessWidget::closeEvent(event);
}

void Widget::initialize()
{
    setWindowTitle(tr("FramelessHelper demo application - Qt Widgets"));
    setWindowIcon(QFileIconProvider().icon(QFileIconProvider::Computer));
    resize(800, 600);
    m_titleBar = new StandardTitleBar(this);
    m_titleBar->setWindowIconVisible(true);
    m_clockLabel = new QLabel(this);
    m_clockLabel->setFrameShape(QFrame::NoFrame);
    QFont clockFont = font();
    clockFont.setBold(true);
    clockFont.setPointSize(70);
    m_clockLabel->setFont(clockFont);
    const auto contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addStretch();
    contentLayout->addWidget(m_clockLabel);
    contentLayout->addStretch();
    const auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_titleBar);
    mainLayout->addLayout(contentLayout);
    setLayout(mainLayout);
    updateStyleSheet();

    m_cancelShortcut = new QShortcut(this);
    m_cancelShortcut->setKey(FRAMELESSHELPER_STRING_LITERAL("ESC"));
    connect(m_cancelShortcut, &QShortcut::activated, this, [this]() {
        if (isFullScreen()) {
            Q_EMIT m_fullScreenShortcut->activated();
        } else {
            close();
        }
    });

    m_fullScreenShortcut = new QShortcut(this);
    m_fullScreenShortcut->setKey(FRAMELESSHELPER_STRING_LITERAL("ALT+RETURN"));
    connect(m_fullScreenShortcut, &QShortcut::activated, this, [this]() {
        if (isFullScreen()) {
            setWindowState(windowState() & ~Qt::WindowFullScreen);
        } else {
            showFullScreen();
        }
    });

    connect(this, &Widget::objectNameChanged, this, [this](const QString &name) {
        if (name.isEmpty()) {
            return;
        }
        setWindowTitle(windowTitle() + FRAMELESSHELPER_STRING_LITERAL(" [%1]").arg(name));
    });

    FramelessWidgetsHelper *helper = FramelessWidgetsHelper::get(this);
    helper->setTitleBarWidget(m_titleBar);
#ifndef Q_OS_MACOS
    helper->setSystemButton(m_titleBar->minimizeButton(), SystemButtonType::Minimize);
    helper->setSystemButton(m_titleBar->maximizeButton(), SystemButtonType::Maximize);
    helper->setSystemButton(m_titleBar->closeButton(), SystemButtonType::Close);
#endif  // Q_OS_MACOS
}

void Widget::updateStyleSheet()
{
    const bool dark                  = Utils::shouldAppsUseDarkMode();
    const QColor clockLabelTextColor = (dark ? kDefaultWhiteColor : kDefaultBlackColor);
    m_clockLabel->setStyleSheet(FRAMELESSHELPER_STRING_LITERAL("background-color: transparent; color: %1;")
                                        .arg(clockLabelTextColor.name()));
    if (FramelessWidgetsHelper::get(this)->isBlurBehindWindowEnabled()) {
        setStyleSheet(FRAMELESSHELPER_STRING_LITERAL("background-color: transparent;"));
    } else {
        const QColor windowBackgroundColor = (dark ? kDefaultSystemDarkColor : kDefaultSystemLightColor);
        setStyleSheet(FRAMELESSHELPER_STRING_LITERAL("background-color: %1;").arg(windowBackgroundColor.name()));
    }
    update();
}

void Widget::waitReady()
{
    FramelessWidgetsHelper *helper = FramelessWidgetsHelper::get(this);
    helper->waitForReady();
    const QString objName    = objectName();
    const auto savedGeometry = Settings::get<QRect>(objName, kGeometry);
    if (savedGeometry.isValid() && !parent()) {
        const auto savedDpr = Settings::get<qreal>(objName, kDevicePixelRatio);
        // Qt doesn't support dpr < 1.
        const qreal oldDpr = std::max(savedDpr, qreal(1));
        const qreal scale  = (devicePixelRatioF() / oldDpr);
        setGeometry({ savedGeometry.topLeft() * scale, savedGeometry.size() * scale });
    } else {
        helper->moveWindowToDesktopCenter();
    }
}
