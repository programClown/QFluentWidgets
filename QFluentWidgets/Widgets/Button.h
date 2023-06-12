#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QRadioButton>
#include <QToolButton>
#include <QUrl>

class FluentIconBase;

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(const QString &text, QWidget *parent = nullptr);
    explicit PushButton(const QString &text, FluentIconBase *ficon, QWidget *parent = nullptr);

    void setIcon(FluentIconBase *ficon);
    FluentIconBase *ficon() const;
    QIcon icon() const;
    virtual void drawIcon(QPainter *painter, const QRect &rect);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isPressed;
    QScopedPointer<FluentIconBase> m_ficon;
};

class PrimaryPushButton : public PushButton
{
    Q_OBJECT
public:
    explicit PrimaryPushButton(const QString &text, QWidget *parent = nullptr);
    explicit PrimaryPushButton(const QString &text, FluentIconBase *ficon, QWidget *parent = nullptr);

    void drawIcon(QPainter *painter, const QRect &rect) override;
};

class HyperlinkButton : public QPushButton
{
    Q_OBJECT
public:
    explicit HyperlinkButton(const QString &url, const QString &text, QWidget *parent = nullptr);

private:
    QUrl m_url;
};

class RadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit RadioButton(const QString &text, QWidget *parent = nullptr);
};

class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolButton(FluentIconBase *ficon, QWidget *parent = nullptr);

    void setIcon(FluentIconBase *ficon);
    FluentIconBase *ficon() const;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isPressed;
    QScopedPointer<FluentIconBase> m_ficon;
};

class TransparentToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit TransparentToolButton(FluentIconBase *ficon, QWidget *parent = nullptr);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QScopedPointer<FluentIconBase> m_ficon;
};

#endif  // BUTTON_H
