#ifndef EXPANDGROUPSETTINGCARD_H
#define EXPANDGROUPSETTINGCARD_H

#include <QAbstractButton>
#include <QFrame>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "Common/Icon.h"

class VBoxLayout;
class SettingCard;

class ExpandButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(float angle READ getAngle WRITE setAngle)

public:
    explicit ExpandButton(QWidget *parent = nullptr);

    void setHover(bool hover);
    void setPressed(bool pressed);
    float getAngle() const;
    void setAngle(float angle);

    bool isHover;
    bool isPressed;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onClicked();

private:
    float m_angle;
    QPropertyAnimation *m_rotateAni;
};

class ExpandSettingCard : public QFrame
{
    Q_OBJECT
public:
    explicit ExpandSettingCard(FluentIconBaseSPtr icon, const QString &title, const QString &content,
                               QWidget *parent = nullptr);

    void addWidget(QWidget *widget);
    void setExpand(bool expand);
    virtual void setValue(const QVariant &value);
    QFrame *view() const;

    VBoxLayout *viewLayout;

public slots:
    void toggleExpand();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void adjustViewSize();

private:
    void initWidget();

private slots:
    void onAniFinished();

private:
    bool m_isExpand;
    ExpandButton *m_expandButton;
    QFrame *m_view;
    SettingCard *m_card;

    QParallelAnimationGroup *m_aniGroup;
    QPropertyAnimation *m_slideAni;
    QPropertyAnimation *m_expandAni;
};

class GroupSeparator : public QWidget
{
    Q_OBJECT
public:
    explicit GroupSeparator(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
};

class ExpandGroupSettingCard : public ExpandSettingCard
{
    Q_OBJECT
public:
    explicit ExpandGroupSettingCard(FluentIconBaseSPtr icon, const QString &title, const QString &content,
                                    QWidget *parent = nullptr);

    void addGroupWidget(QWidget *widget);

signals:
};

#endif  // EXPANDGROUPSETTINGCARD_H
