#include "WindowsFramelessWindow.h"
#include "WindowsWindowEffect.h"
#include "titlebar/TitleBar.h"
#include "utils/Win32Utils.h"

#include <QWindow>
#include <QtWin>
#include <QApplication>
#include <QCloseEvent>
#include <QScreen>
#include <QDesktopWidget>

#include <QDebug>

constexpr int BORDER_WIDTH = 5;

WindowsFramelessWindow::WindowsFramelessWindow(QWidget *parent) : QMainWindow(parent)
{
    Q_INIT_RESOURCE(qframelesswindow);

    windowEffect    = new WindowsWindowEffect(this);
    titleBar        = new TitleBar(this);
    isResizeEnabled = true;

    // remove window border
    if (!Win32Utils::isWin7()) {
        setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    } else {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    }

    // add DWM shadow and window animation
    windowEffect->addWindowAnimation((HWND)this->winId());

    if (effectType() == "AcrylicWindow") {
        windowEffect->addShadowEffect((HWND)this->winId());
    }

    connect(this->windowHandle(), &QWindow::screenChanged, this, &WindowsFramelessWindow::onScreenChanged);

    resize(500, 500);
    titleBar->raise();
}

void WindowsFramelessWindow::setTitleBar(TitleBar *tBar)
{
    if (tBar != titleBar) {
        titleBar->deleteLater();
        titleBar = tBar;
        titleBar->setParent(this);
        titleBar->raise();
    }
}

void WindowsFramelessWindow::setResizeEnabled(bool isEnabled)
{
    isResizeEnabled = isEnabled;
}

bool WindowsFramelessWindow::getResizeEnabled() const
{
    return isResizeEnabled;
}

WindowsWindowEffect *WindowsFramelessWindow::getWindowEffect() const
{
    return windowEffect;
}

TitleBar *WindowsFramelessWindow::getTitleBar() const
{
    return titleBar;
}

void WindowsFramelessWindow::onScreenChanged(QScreen *screen)
{
    HWND hWnd = (HWND)this->windowHandle()->winId();
    SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}

void WindowsFramelessWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    titleBar->resize(this->width(), titleBar->height());
}

/// Handle the Windows message
bool WindowsFramelessWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = static_cast<MSG *>(message);
    if (!msg->hwnd) {
        return QWidget::nativeEvent(eventType, message, result);
    }

    switch (msg->message) {
        case WM_NCHITTEST: {
            if (isResizeEnabled) {
                QPoint pos = QCursor::pos();
                int xPos   = pos.x() - this->x();
                int yPos   = pos.y() - this->y();
                int w      = this->width();
                int h      = this->height();

                bool lx = (xPos < BORDER_WIDTH);
                bool rx = (xPos > w - BORDER_WIDTH);
                bool ty = (yPos < BORDER_WIDTH);
                bool by = (yPos > h - BORDER_WIDTH);
                if (lx && ty) {
                    *result = HTTOPLEFT;
                    return true;
                } else if (rx && by) {
                    *result = HTBOTTOMRIGHT;
                    return true;
                } else if (rx && ty) {
                    *result = HTTOPRIGHT;
                    return true;
                } else if (lx && by) {
                    *result = HTBOTTOMLEFT;
                    return true;
                } else if (ty) {
                    *result = HTTOP;
                    return true;
                } else if (by) {
                    *result = HTBOTTOM;
                    return true;
                } else if (lx) {
                    *result = HTLEFT;
                    return true;
                } else if (rx) {
                    *result = HTRIGHT;
                    return true;
                }
            }
        } break;

        case WM_NCCALCSIZE: {
            RECT rc;
            if (msg->wParam) {
                rc = ((LPNCCALCSIZE_PARAMS)msg->lParam)->rgrc[0];
            } else {
                rc = *(LPRECT)msg->lParam;
            }

            bool isMax  = Win32Utils::isMaximized(msg->hwnd);
            bool isFull = Win32Utils::isFullScreen(msg->hwnd);

            // adjust the size of client rect
            if (isMax && !isFull) {
                int ty = Win32Utils::getResizeBorderThickness(msg->hwnd, false);
                rc.top += ty;
                rc.bottom -= ty;

                int tx = Win32Utils::getResizeBorderThickness(msg->hwnd, true);
                rc.left += tx;
                rc.right -= tx;
            }

            // handle the situation that an auto-hide taskbar is enabled
            if ((isMax || isFull) && Win32Utils::Taskbar::isAutoHide()) {
                Win32Utils::Taskbar::Position position = Win32Utils::Taskbar::getPosition(msg->hwnd);
                if (position == Win32Utils::Taskbar::TOP) {
                    rc.top += Win32Utils::Taskbar::AUTO_HIDE_THICKNESS;
                } else if (position == Win32Utils::Taskbar::BOTTOM) {
                    rc.bottom -= Win32Utils::Taskbar::AUTO_HIDE_THICKNESS;
                } else if (position == Win32Utils::Taskbar::LEFT) {
                    rc.left += Win32Utils::Taskbar::AUTO_HIDE_THICKNESS;
                } else if (position == Win32Utils::Taskbar::RIGHT) {
                    rc.right -= Win32Utils::Taskbar::AUTO_HIDE_THICKNESS;
                }
            }

            if (!msg->wParam) {
                *result = 0;
            } else {
                *result = WVR_REDRAW;
            }
            return true;
        } break;

        case WM_GETMINMAXINFO: {
            //解决QT无边框窗体最大化遮盖Windows任务栏问题
            if (this->isMaximized()) {
                int index          = QApplication::desktop()->screenNumber(this);
                const QRect rc     = QApplication::desktop()->availableGeometry(index);
                MINMAXINFO *p      = (MINMAXINFO *)(msg->lParam);
                p->ptMaxPosition.x = 0;
                p->ptMaxPosition.y = 0;
                p->ptMaxSize.x     = rc.width();
                p->ptMaxSize.y     = rc.height();
                *result            = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
            }
        }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

AcrylicWindow::AcrylicWindow(QWidget *parent) : WindowsFramelessWindow(parent), closedByKey(false)
{
    QtWin::enableBlurBehindWindow(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);

    WindowsWindowEffect *windowEffect = getWindowEffect();
    windowEffect->addWindowAnimation((HWND)this->winId());

    if (Win32Utils::isWin7()) {
        windowEffect->addShadowEffect((HWND)this->winId());
        windowEffect->setAeroEffect((HWND)this->winId());
    } else {
        windowEffect->setAcrylicEffect((HWND)this->winId());
        if (Win32Utils::isGreaterEqualWin11()) {
            windowEffect->addShadowEffect((HWND)this->winId());
        }
    }

    setStyleSheet("AcrylicWindow{background:transparent}");
}

QString AcrylicWindow::effectType() const
{
    return "AcrylicWindow";
}

void AcrylicWindow::closeEvent(QCloseEvent *event)
{
    if (!closedByKey || QApplication::quitOnLastWindowClosed()) {
        closedByKey = false;
        return WindowsFramelessWindow::closeEvent(event);
    }

    // system tray icon
    closedByKey = false;
    this->hide();
}

bool AcrylicWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = static_cast<MSG *>(message);

    if (msg->message == WM_SYSKEYDOWN) {
        if (msg->wParam == VK_F4) {
            closedByKey = true;
            QCloseEvent clsEvt;
            QApplication::sendEvent(this, &clsEvt);
            *result = 0;
            return false;
        }
    }

    return WindowsFramelessWindow::nativeEvent(eventType, message, result);
}
