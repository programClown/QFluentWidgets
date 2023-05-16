#ifndef QFRAMELESSWINDOW_H
#define QFRAMELESSWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include "titlebar/TitleBar.h"

#if defined(Q_OS_WIN32)
    #include "windows/WindowsFramelessWindow.h"
    #include "windows/WindowsFramelessDialog.h"
    #include "windows/WindowsFramelessWidget.h"

class FramelessMainWindow : public WindowsFramelessWindow
{
    Q_OBJECT

public:
    explicit FramelessMainWindow(QWidget *parent = nullptr);
};

class FramelessDialog : public WindowsFramelessDialog
{
    Q_OBJECT

public:
    explicit FramelessDialog(QWidget *parent = nullptr);
};

#elif defined(Q_OS_OSX)

class FramelessMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FramelessMainWindow(QWidget *parent = nullptr);
};

#else

#endif

#endif  // QFRAMELESSWINDOW_H
