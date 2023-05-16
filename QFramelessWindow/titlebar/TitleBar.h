#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

class MinimizeButton;
class CloseButton;
class MaximizeButton;

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    QHBoxLayout *hBoxLayout;
    MinimizeButton *minimizeButton() const;
    CloseButton *closeButton() const;
    MaximizeButton *maximizeButton() const;

private slots:
    void onToggleMaxState();

private:
    MinimizeButton *minBtn;
    CloseButton *closeBtn;
    MaximizeButton *maxBtn;
    bool isDoubleClickEnabled;

public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

    void setDoubleClickEnabled(bool isEnabled);
    bool canDrag(QPoint pos);

protected:
    bool isDragRegion(QPoint pos);
    bool hasButtonPressed();
};

class StandardTitleBar : public TitleBar
{
    Q_OBJECT

public:
    explicit StandardTitleBar(QWidget *parent = nullptr);

    void setTitle(const QString &title);
    void setIcon(const QIcon &icon);

private:
    QLabel *iconLabel;
    QLabel *titleLabel;
};

#endif  // TITLEBAR_H
