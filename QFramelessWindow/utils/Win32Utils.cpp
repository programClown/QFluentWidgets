#include "Win32Utils.h"

#include <QWindow>
#include <QtWin>

namespace Win32Utils
{

    bool isMaximized(HWND hWnd)
    {
        WINDOWPLACEMENT wPos;
        BOOL windowPlacement = GetWindowPlacement(hWnd, &wPos);
        if (!windowPlacement)
            return false;
        return wPos.showCmd == SW_MAXIMIZE;
    }

    bool isFullScreen(HWND hWnd)
    {
        if (!hWnd)
            return false;

        RECT winRect;
        if (!GetWindowRect(hWnd, &winRect)) {
            return false;
        }

        MONITORINFO mi = { sizeof(mi) };
        if (!getMonitorInfo(hWnd, MONITOR_DEFAULTTOPRIMARY, &mi)) {
            return false;
        }

        return (mi.rcMonitor.left == winRect.left) && (mi.rcMonitor.right == winRect.right)
                && (mi.rcMonitor.bottom == winRect.bottom) && (mi.rcMonitor.top == winRect.top);
    }

    /**
     * @brief getMonitorInfo
     * @param hWnd: window handle
     * @param dwFlags: Determines the return value if the window does not intersect any display monitor
     * @param lpmi
     * @return
     */
    bool getMonitorInfo(HWND hWnd, DWORD dwFlags, LPMONITORINFO lpmi)
    {
        HMONITOR monitor = MonitorFromWindow(hWnd, dwFlags);
        if (!monitor) {
            return false;
        }

        return GetMonitorInfo(monitor, lpmi);
    }

    /**
     * @brief getResizeBorderThickness
     * @param hWnd: window handle
     * @param horizontal: (dpiScale) whether to use dpi scale
     * @return
     */
    int getResizeBorderThickness(HWND hWnd, bool horizontal)
    {
        QWindow *window = findWindow(hWnd);
        if (!window) {
            return 0;
        }

        int frame = SM_CXSIZEFRAME;
        if (!horizontal) {
            frame = SM_CYSIZEFRAME;
        }

        UINT dpi = GetDpiForWindow(hWnd);

        int result = GetSystemMetricsForDpi(frame, dpi) + GetSystemMetricsForDpi(92, dpi);

        if (result > 0)
            return result;

        int thickness;
        if (QtWin::isCompositionEnabled()) {
            thickness = 8;
        } else {
            thickness = 4;
        }

        return qRound(thickness * window->devicePixelRatio());
    }

    QWindow *findWindow(HWND hWnd)
    {
        if (!hWnd) {
            return nullptr;
        }

        QWindowList windows = QGuiApplication::topLevelWindows();

        for (auto win : windows) {
            if (win && (SIZE_T(win->winId()) == SIZE_T(hWnd))) {
                return win;
            }
        }

        return nullptr;
    }

    bool isGreaterEqualVersion(QOperatingSystemVersion version)
    {
        return QOperatingSystemVersion::current() >= version;
    }

    bool isWin7()
    {
        return QOperatingSystemVersion::current().majorVersion() == QOperatingSystemVersion::Windows7.majorVersion();
    }

    bool isGreaterEqualWin8_1()
    {
        return isGreaterEqualVersion(QOperatingSystemVersion::Windows8_1);
    }

    bool isGreaterEqualWin10()
    {
        return isGreaterEqualVersion(QOperatingSystemVersion::Windows10);
    }

    /**
     * @brief Win8以后GetVersionEx废弃，操作不正常，用黑魔法实现版本查询
     * @param lpVersionInformation
     * @return
     */
    BOOL GetVersionEx2(LPOSVERSIONINFOW lpVersionInformation)
    {
        HMODULE hNtDll = GetModuleHandle(L"NTDLL");                          // 获取ntdll.dll的句柄
        typedef NTSTATUS(NTAPI * tRtlGetVersion)(PRTL_OSVERSIONINFOW povi);  // RtlGetVersion的原型
        tRtlGetVersion pRtlGetVersion = NULL;
        if (hNtDll) {
            pRtlGetVersion = (tRtlGetVersion)GetProcAddress(hNtDll, "RtlGetVersion");  // 获取RtlGetVersion地址
        }
        if (pRtlGetVersion) {
            return pRtlGetVersion((PRTL_OSVERSIONINFOW)lpVersionInformation) >= 0;  // 调用RtlGetVersion
        }
        return FALSE;
    }

    bool isGreaterEqualWin11()
    {
#if 0
        OSVERSIONINFOW osi;
        osi.dwOSVersionInfoSize = sizeof(osi);
        if (!GetVersionEx2(&osi)) {
            return false;
        }

        if (osi.dwMajorVersion < 10) {
            return false;
        }

        if (osi.dwBuildNumber >= 22000) {
            return true;
        }
        return false;
#endif
        QOperatingSystemVersion os = QOperatingSystemVersion::current();
        if (os.majorVersion() >= 10 && os.microVersion() >= 22000) {
            return true;
        }

        return false;
    }

    /**
     * @brief detect whether the taskbar is hidden automatically
     * @return
     */
    bool Taskbar::isAutoHide()
    {
        APPBARDATA appbarData;
        appbarData.cbSize           = sizeof(APPBARDATA);
        appbarData.hWnd             = 0;
        appbarData.uCallbackMessage = 0;
        appbarData.uEdge            = 0;
        appbarData.rc               = { 0, 0, 0, 0 };
        appbarData.lParam           = 0;

        UINT uState = (UINT)SHAppBarMessage(ABM_GETSTATE, &appbarData);

        return uState == ABS_AUTOHIDE;
    }

    /**
     * @brief get the position of auto-hide task bar`
     * @param hWnd
     * @return
     */
    Taskbar::Position Taskbar::getPosition(HWND hWnd)
    {
        APPBARDATA appbarData;
        if (isGreaterEqualWin8_1()) {
            MONITORINFO mi = { sizeof(mi) };
            if (!getMonitorInfo(hWnd, MONITOR_DEFAULTTONEAREST, &mi)) {
                return NO_POSITION;
            }

            RECT monitor                      = mi.rcMonitor;
            appbarData                        = APPBARDATA{ sizeof(APPBARDATA), 0, 0, 0, monitor, 0 };
            const QVector<Position> positions = { LEFT, TOP, RIGHT, BOTTOM };
            for (auto positon : positions) {
                appbarData.uEdge = positon;
                // ABM_GETAUTOHIDEBAREX = 11
                if (SHAppBarMessage(11, &appbarData)) {
                    return positon;
                }
            }

            return NO_POSITION;
        }

        appbarData = APPBARDATA{ sizeof(APPBARDATA), FindWindow(L"Shell_TrayWnd", NULL), 0, 0, RECT{ 0, 0, 0, 0 }, 0 };
        if (appbarData.hWnd) {
            HMONITOR windowMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
            if (!windowMonitor) {
                return NO_POSITION;
            }

            HMONITOR taskbarMonitor = MonitorFromWindow(appbarData.hWnd, MONITOR_DEFAULTTOPRIMARY);
            if (!taskbarMonitor) {
                return NO_POSITION;
            }

            if (windowMonitor == taskbarMonitor) {
                SHAppBarMessage(ABM_GETTASKBARPOS, &appbarData);
                return (Position)appbarData.uEdge;
            }
        }

        return NO_POSITION;
    }

    void WindowsMoveResize::startSystemMove(QWidget *window, QPoint globalPos)
    {
        Q_UNUSED(globalPos)

        ReleaseCapture();
        SendMessage((HWND)window->winId(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
    }

    void WindowsMoveResize::starSystemResize(QWidget *window, QPoint globalPos, Qt::Edges edges)
    {
        Q_UNUSED(window)
        Q_UNUSED(globalPos)
        Q_UNUSED(edges)
    }

}  // namespace Win32Utils
