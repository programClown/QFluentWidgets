#ifndef TITLEBARBUTTON_H
#define TITLEBARBUTTON_H

#include <QAbstractButton>
#include <QDomDocument>

enum TitleBarButtonState
{
    NORMAL  = 0,
    HOVER   = 1,
    PRESSED = 2
};

class TitleBarButton : public QAbstractButton
{
    Q_OBJECT

    Q_PROPERTY(QColor normalColor READ getNormalColor WRITE setNormalColor)
    Q_PROPERTY(QColor hoverColor READ getHoverColor WRITE setHoverColor)
    Q_PROPERTY(QColor pressedColor READ getPressedColor WRITE setPressedColor)
    Q_PROPERTY(QColor normalBackgroundColor READ getNormalBackgroundColor WRITE setNormalBackgroundColor)
    Q_PROPERTY(QColor hoverBackgroundColor READ getHoverBackgroundColor WRITE setHoverBackgroundColor)
    Q_PROPERTY(QColor pressedBackgroundColor READ getPressedBackgroundColor WRITE setPressedBackgroundColor)

public:
    explicit TitleBarButton(QWidget *parent = nullptr);

    void setState(TitleBarButtonState state);
    bool isPressed();

    QColor getNormalColor();
    QColor getHoverColor();
    QColor getPressedColor();
    QColor getNormalBackgroundColor();
    QColor getHoverBackgroundColor();
    QColor getPressedBackgroundColor();

    void setNormalColor(const QColor &color);
    void setHoverColor(const QColor &color);
    void setPressedColor(const QColor &color);
    void setNormalBackgroundColor(const QColor &color);
    void setHoverBackgroundColor(const QColor &color);
    void setPressedBackgroundColor(const QColor &color);

    QVector<QColor> getColors();

private:
    TitleBarButtonState m_state;

    // icon color
    QColor m_normalColor;
    QColor m_hoverColor;
    QColor m_pressedColor;

    // background color
    QColor m_normalBgColor;
    QColor m_hoverBgColor;
    QColor m_pressedBgColor;

signals:

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
};

class SvgTitleBarButton : public TitleBarButton
{
    Q_OBJECT
public:
    explicit SvgTitleBarButton(const QString &iconPath, QWidget *parent = nullptr);
    ~SvgTitleBarButton();

    void setIcon(const QString &iconPath);

private:
    QDomDocument *m_svgDom;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

class MinimizeButton : public TitleBarButton
{
    Q_OBJECT

public:
    explicit MinimizeButton(QWidget *parent = nullptr);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

class MaximizeButton : public TitleBarButton
{
    Q_OBJECT

public:
    explicit MaximizeButton(QWidget *parent = nullptr);

    void setMaxState(bool isMax);

private:
    bool m_isMax;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

class CloseButton : public SvgTitleBarButton
{
    Q_OBJECT

public:
    explicit CloseButton(QWidget *parent = nullptr);
};

#endif  // TITLEBARBUTTON_H
