#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FramelessHelper/Widgets/framelesswidget.h>

FRAMELESSHELPER_BEGIN_NAMESPACE
class StandardTitleBar;
FRAMELESSHELPER_END_NAMESPACE

class GLWidget;

class MainWindow : public FRAMELESSHELPER_PREPEND_NAMESPACE(FramelessWidget)
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void waitReady();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initialize();

private:
    FRAMELESSHELPER_PREPEND_NAMESPACE(StandardTitleBar) *m_titleBar = nullptr;
    GLWidget *m_glWidget                                            = nullptr;
};

#endif  // MAINWINDOW_H
