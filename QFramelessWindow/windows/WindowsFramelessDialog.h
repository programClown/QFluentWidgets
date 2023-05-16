#ifndef WINDOWSFRAMELESSDIALOG_H
#define WINDOWSFRAMELESSDIALOG_H

#include <QDialog>

class WindowsWindowEffect;
class TitleBar;

class WindowsFramelessDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WindowsFramelessDialog(QWidget *parent = nullptr);

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

#endif  // WINDOWSFRAMELESSDIALOG_H
