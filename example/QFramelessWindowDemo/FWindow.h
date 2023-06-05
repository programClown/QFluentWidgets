#ifndef FWINDOW_H
#define FWINDOW_H

#include "QFramelessWindow.h"

#include <WindowsFramelessWindow.h>

class FWindow : public WindowsFramelessWindow
{
public:
    FWindow(QWidget *parent = nullptr);
};

#endif  // FWINDOW_H
