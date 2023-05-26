#include "AWindow.h"

#include "TitleBar.h"
#include "WindowsWindowEffect.h"

#include <QDebug>
#include <QPainter>

AWindow::AWindow(QWidget *parent) : AcrylicWindow(parent)
{
    // 窗口效果
    getWindowEffect()->setAcrylicEffect((HWND)winId());
    setStyleSheet("background:rgba(232, 232, 232, 150)");

    // 标题栏
    StandardTitleBar *ttb = new StandardTitleBar(this);
    setTitleBar(ttb);

    QIcon icon = QIcon(":/images/logo.png");
    ttb->setIcon(icon);
    setWindowIcon(icon);
    setWindowTitle("PyQt-Frameless-Window");
}
