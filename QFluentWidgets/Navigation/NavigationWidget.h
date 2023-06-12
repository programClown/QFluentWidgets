#ifndef NAVIGATIONWIDGET_H
#define NAVIGATIONWIDGET_H

#include <QWidget>
#include "Common/Icon.h"

class NavigationWidget : public QWidget
{
    Q_OBJECT
public:
    static int EXPAND_WIDTH;

    explicit NavigationWidget(bool selectable, QWidget *parent = nullptr);

    virtual void setCompacted(bool compacted);
    void setSelected(bool selected);

    bool isCompacted;
    bool isSelected;
    bool isPressed;
    bool isEnter;
    bool isSelectable;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void clicked(bool);
};

class NavigationPushButton : public NavigationWidget
{
    Q_OBJECT
public:
    explicit NavigationPushButton(FluentIconBase *ficon, const QString &text, bool selectable,
                                  QWidget *parent = nullptr);

    QString text() const;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QScopedPointer<FluentIconBase> m_ficon;
    QString m_text;
};

class NavigationToolButton : public NavigationPushButton
{
    Q_OBJECT
public:
    explicit NavigationToolButton(FluentIconBase *ficon, QWidget *parent = nullptr);

    void setCompacted(bool compacted);
};

class NavigationSeparator : public NavigationWidget
{
    Q_OBJECT
public:
    explicit NavigationSeparator(QWidget *parent = nullptr);

    void setCompacted(bool compacted) override;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif  // NAVIGATIONWIDGET_H
