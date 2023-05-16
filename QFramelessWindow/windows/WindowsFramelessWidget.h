#ifndef WINDOWSFRAMELESSWIDGET_H
#define WINDOWSFRAMELESSWIDGET_H
#include <QWidget>

class WindowsWindowEffect;
class TitleBar;

class WindowsFramelessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WindowsFramelessWidget(QWidget *parent = nullptr);

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

#endif  // WINDOWSFRAMELESSWIDGET_H
