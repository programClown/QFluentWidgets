#include "DWindow.h"

#include "TitleBarButton.h"

#include <QDebug>
#include <math.h>

CustomTitleBar::CustomTitleBar(QWidget *parent) : StandardTitleBar(parent)
{
    // customize the style of title bar button
    this->minimizeButton()->setHoverColor(Qt::white);
    this->minimizeButton()->setHoverBackgroundColor(QColor(0, 100, 182));
    this->minimizeButton()->setPressedColor(Qt::white);
    this->minimizeButton()->setPressedBackgroundColor(QColor(54, 57, 65));
    // use qss to customize title bar button
    this->minimizeButton()->setStyleSheet("TitleBarButton {"
                                          "qproperty - hoverColor : white;"
                                          "qproperty - hoverBackgroundColor : rgb(0, 100, 182);"
                                          "qproperty - pressedColor : white;"
                                          "qproperty - pressedBackgroundColor : rgb(54, 57, 65);"
                                          "}");
}

DWindow::DWindow(QWidget *parent) : WindowsFramelessWindow(parent)
{
    CustomTitleBar *ttb = new CustomTitleBar(this);
    setTitleBar(ttb);

    m_label = new QLabel(this);
    m_label->setScaledContents(true);

    QPixmap pixmap = QPixmap(":/images/shoko.png");

    m_label->setPixmap(pixmap);
    setCentralWidget(m_label);

    QIcon icon = QIcon(":/images/logo.png");
    ttb->setIcon(icon);
    setWindowIcon(icon);
    setWindowTitle("PyQt-Frameless-Window");
    setStyleSheet("background:white");

    getTitleBar()->raise();

    //    getWindowEffect()->setAeroEffect((HWND)winId());
}

void DWindow::resizeEvent(QResizeEvent *event)
{
    WindowsFramelessWindow::resizeEvent(event);

    int length = std::min(width(), height());
    m_label->resize(length, length);
    m_label->move((width() - length) / 2, (height() - length) / 2);
}
