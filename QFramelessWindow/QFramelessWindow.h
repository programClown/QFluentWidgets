#ifndef QFRAMELESSWINDOW_H
#define QFRAMELESSWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QtGlobal>
#include "titlebar/TitleBar.h"

#if defined(Q_OS_WIN32)
    #include "windows/WindowsFramelessHelper.h"
using QFramelessHelper = WindowsFramelessHelper;

#else
    #include "widgetResizeHandler/WidgetResizeHandler.h"
using QFramelessHelper = WidgetResizeHandler;
#endif

#endif  // QFRAMELESSWINDOW_H
