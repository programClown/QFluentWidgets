#include "QFramelessWindow.h"
#include "titlebar/TitleBarButton.h"

#if defined(Q_OS_WIN32)

/**
 * @brief 无边框窗口
 * @param parent
 */
FramelessMainWindow::FramelessMainWindow(QWidget *parent) : WindowsFramelessWindow(parent) { }

/**
 * @brief 无边框对话框
 * @param parent
 */
FramelessDialog::FramelessDialog(QWidget *parent) : WindowsFramelessDialog(parent)
{
    TitleBar *titleBar = getTitleBar();
    titleBar->minimizeButton()->hide();
    titleBar->setDoubleClickEnabled(true);
}

#elif defined(Q_OS_OSX)

#else

#endif
