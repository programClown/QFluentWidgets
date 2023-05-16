#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FramelessHelper/Widgets/framelessmainwindow.h>

FRAMELESSHELPER_BEGIN_NAMESPACE
class StandardTitleBar;
FRAMELESSHELPER_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class MainWindow : public FRAMELESSHELPER_PREPEND_NAMESPACE(FramelessMainWindow)
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

public:
    explicit MainWindow(QWidget *parent = nullptr, const Qt::WindowFlags flags = {});
    ~MainWindow() override;

    void waitReady();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initialize();

private:
    FRAMELESSHELPER_PREPEND_NAMESPACE(StandardTitleBar) *m_titleBar = nullptr;
    Ui::MainWindow *m_mainWindow                                    = nullptr;
};

#endif  // MAINWINDOW_H
