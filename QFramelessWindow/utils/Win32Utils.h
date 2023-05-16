#ifndef WIN32UTILS_H
#define WIN32UTILS_H

#include <QGuiApplication>
#include <QOperatingSystemVersion>
#include <Windows.h>

namespace Win32Utils
{
    bool isMaximized(HWND hWnd);

    bool isFullScreen(HWND hWnd);

    bool getMonitorInfo(HWND hWnd, DWORD dwFlags, LPMONITORINFO lpmi);

    int getResizeBorderThickness(HWND hWnd, bool horizontal = true);

    QWindow *findWindow(HWND hWnd);

    bool isGreaterEqualVersion(QOperatingSystemVersion version);

    bool isWin7();

    bool isGreaterEqualWin8_1();

    bool isGreaterEqualWin10();

    bool isGreaterEqualWin11();

    class Taskbar
    {
    public:
        enum Position
        {
            LEFT = 0,
            TOP,
            RIGHT,
            BOTTOM,
            NO_POSITION
        };

        static const int AUTO_HIDE_THICKNESS = 2;

        static bool isAutoHide();

        static Position getPosition(HWND hWnd);
    };

    /**
     * @brief Tool class for moving and resizing Mac OS window
     */
    class WindowsMoveResize
    {
    public:
        static void startSystemMove(QWidget *window, QPoint globalPos);
        static void starSystemResize(QWidget *window, QPoint globalPos, Qt::Edges edges);
    };
};  // namespace Win32Utils

#endif  // WIN32UTILS_H
