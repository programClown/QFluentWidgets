#ifndef STATETOOLTIP_H
#define STATETOOLTIP_H

#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPropertyAnimation>
#include <QToolButton>

class StateCloseButton : public QToolButton
{
    Q_OBJECT
public:
    explicit StateCloseButton(QWidget *parent = nullptr);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isEnter;
    bool m_isPressed;
};

class StateToolTip : public QWidget
{
    Q_OBJECT
public:
    explicit StateToolTip(const QString &title, const QString &content, QWidget *parent = nullptr);

    void setTitle(const QString &title);
    void setContent(const QString &content);
    void setState(bool isDone = false);
    QPoint getSuitablePos();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void closedSignal();

private slots:
    void onCloseButtonClicked();
    void rotateTimerFlowSlot();
    void fadeOut();

private:
    void initWidget();
    void setQss();
    void initLayout();

private:
    QString m_title;
    QString m_content;
    QLabel *m_titleLabel;
    QLabel *m_contentLabel;
    QTimer *m_rotateTimer;
    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_animation;
    StateCloseButton *m_closeButton;
    bool m_isDone;
    int m_rotateAngle;
    int m_deltaAngle;
};

#endif  // STATETOOLTIP_H
