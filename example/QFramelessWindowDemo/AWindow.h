#ifndef AWINDOW_H
#define AWINDOW_H

#include "WindowsFramelessWindow.h"

class AWindow : public AcrylicWindow
{
public:
    AWindow(QWidget *parent = nullptr);
};

#endif  // AWINDOW_H
