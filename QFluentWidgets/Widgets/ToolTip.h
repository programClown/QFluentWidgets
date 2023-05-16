#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QFrame>
#include <QLabel>

class ToolTip : public QFrame
{
    Q_OBJECT
public:
    explicit ToolTip(const QString &text = "", QWidget *parent = nullptr);

    QString text() const;
    void setText(const QString &text);

    int duration() const;
    void setDuration(int duration);

    void adjustPos(const QPoint &pos, const QSize &size);

    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;

private:
    void setQss();

signals:

private:
    QString m_text;
    int m_duration;
    QFrame *m_container;
    QTimer *m_timer;
    QLabel *m_label;
};

class ToolTipFilter : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief ToolTipFilter
     * @param parent: the widget to install tool tip
     * @param showDelay: show tool tip after how long the mouse hovers in milliseconds
     */
    explicit ToolTipFilter(QWidget *parent, int showDelay = 300);

    void hideToolTip();
    void showToolTip();

    void setTooltipDelay(int delay);

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    bool m_isEnter;
    ToolTip *m_toolTip;
    int m_tooltipDelay;
    QTimer *m_timer;
};
#endif  // TOOLTIP_H
