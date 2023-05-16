#ifndef WINDOWSFRAMELESSWINDOW_H
#define WINDOWSFRAMELESSWINDOW_H

#include <QMainWindow>

class WindowsWindowEffect;
class TitleBar;

class WindowsFramelessWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit WindowsFramelessWindow(QWidget *parent = nullptr);

    void setTitleBar(TitleBar *tBar);
    void setResizeEnabled(bool isEnabled);
    bool getResizeEnabled() const;
    WindowsWindowEffect *getWindowEffect() const;
    TitleBar *getTitleBar() const;

protected:
    virtual QString effectType() const { return ""; };

private:
    WindowsWindowEffect *windowEffect;
    TitleBar *titleBar;
    bool isResizeEnabled;

private slots:
    void onScreenChanged(QScreen *screen);

signals:

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;
};

class AcrylicWindow : public WindowsFramelessWindow
{
    Q_OBJECT
public:
    explicit AcrylicWindow(QWidget *parent = nullptr);

    // interface
protected:
    QString effectType() const Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;

private:
    bool closedByKey;
};

#endif  // WINDOWSFRAMELESSWINDOW_H
