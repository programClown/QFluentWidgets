#ifndef FWINDOW_H
#define FWINDOW_H

#include "QFramelessWindow.h"

class FWindow : public FramelessMainWindow
{
public:
    FWindow(QWidget *parent = nullptr);

private:
    void showHelpDialog();
};

#endif  // FWINDOW_H
