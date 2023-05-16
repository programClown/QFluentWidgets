#ifndef THREESTATEBUTTON_H
#define THREESTATEBUTTON_H

#include <QMap>
#include <QToolButton>

enum ButtonState
{
    NORMAL  = 0,
    HOVER   = 1,
    PRESSED = 2
};

class ThreeStateButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ThreeStateButton(const QMap<ButtonState, QString> &iconPaths, QWidget *parent = nullptr,
                              const QSize &buttonSize = QSize(40, 40), const QSize &iconSize = QSize());

    void setState(ButtonState state);

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

signals:

private:
    QMap<ButtonState, QString> m_iconPaths;
    QSize m_buttonSize;
    QSize m_iconSize;
};

#endif  // THREESTATEBUTTON_H
