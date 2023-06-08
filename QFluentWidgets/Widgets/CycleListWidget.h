#ifndef CYCLELISTWIDGET_H
#define CYCLELISTWIDGET_H

#include <QListWidget>
#include <QToolButton>

#include <Common/Icon.h>

class SmoothScrollBar;

class ScrollButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ScrollButton(FluentIconSPtr icon, QWidget *parent = nullptr);

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    bool isPressed;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    FluentIconSPtr m_icon;
};

class CycleListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit CycleListWidget(const QVariantList &items, const QSize &itemSize,
                             Qt::AlignmentFlag align = Qt::AlignCenter, QWidget *parent = nullptr);

    void setItems(const QVariantList &items);

    int currentIndex() const;
    void setCurrentIndex(int index);
    QListWidgetItem *currentItem() const;
    void scrollToItem(QListWidgetItem *item, QListWidget::ScrollHint hint = PositionAtCenter);
    void setSelectedItem(const QString &text);
    QSize itemSize() const;

public:
    SmoothScrollBar *vScrollBar;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void enterEvent(QEvent *event) override;

signals:
    void currentItemChanged(QListWidgetItem *item);

private:
    void createItems(const QVariantList &items);
    void addColumnItems(const QVariantList &items, bool disabled = false);

private slots:
    void scrollUp();
    void scrollDown();
    void onItemClicked(QListWidgetItem *item);

private:
    QSize m_itemSize;
    Qt::AlignmentFlag m_align;

    ScrollButton *m_upButton;
    ScrollButton *m_downButton;
    int m_scrollDuration;
    QVariantList m_originItems;
    int m_visibleNumber;
    bool m_isCycle;
    int m_currentIndex;
};

#endif  // CYCLELISTWIDGET_H
