#ifndef WIDGET_H
#define WIDGET_H

#include <FramelessHelper/Widgets/framelesswidget.h>

QT_BEGIN_NAMESPACE
class QLabel;
class QShortcut;
QT_END_NAMESPACE

FRAMELESSHELPER_BEGIN_NAMESPACE
class StandardTitleBar;
FRAMELESSHELPER_END_NAMESPACE

class Widget : public FRAMELESSHELPER_PREPEND_NAMESPACE(FramelessWidget)
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Widget)

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

    void waitReady();

protected:
    void timerEvent(QTimerEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void initialize();

private Q_SLOTS:
    void updateStyleSheet();

private:
    QLabel *m_clockLabel                                            = nullptr;
    FRAMELESSHELPER_PREPEND_NAMESPACE(StandardTitleBar) *m_titleBar = nullptr;
    QShortcut *m_fullScreenShortcut                                 = nullptr;
    QShortcut *m_cancelShortcut                                     = nullptr;
};

#endif  // WIDGET_H
