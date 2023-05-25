#ifndef DWINDOW_H
#define DWINDOW_H

#include <WindowsFramelessWindow.h>
#include <TitleBar.h>

class CustomTitleBar : public StandardTitleBar
{
public:
    CustomTitleBar(QWidget *parent = nullptr);
};

class DWindow : public WindowsFramelessWindow
{
public:
    DWindow(QWidget *parent = nullptr);

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QLabel *m_label;
};

#endif  // DWINDOW_H
