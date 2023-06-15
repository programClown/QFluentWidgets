#ifndef CYCLELISTWIDGET_H
#define CYCLELISTWIDGET_H

#include <QListWidget>
#include <QToolButton>

#include <Common/Icon.h>

class SmoothScrollBar;

class ScrollIcon : public FluentIconBase
{
    Q_OBJECT
public:
    enum IconType
    {
        UP = 0,
        DOWN
    };
    Q_ENUM(IconType)
    //    const QString UP = "UP";
    //    const QString DOWN     = "DOWN";

    static QString iconName(IconType type);

    ScrollIcon(IconType type, Qfw::Theme t = Qfw::DARK);
    ~ScrollIcon();

    QString iconPath();

    QIcon icon() override;
    QString typeName() const override;
    QString enumName() const override;
    FluentIconBase *clone() override;
    Qfw::Theme theme() const;
    void setTheme(const Qfw::Theme &theme) override;

private:
    Qfw::Theme m_theme;
    IconType m_type;
};

class ScrollButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ScrollButton(FluentIconBase *icon, QWidget *parent = nullptr);

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    bool isPressed;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QScopedPointer<FluentIconBase> m_icon;
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
