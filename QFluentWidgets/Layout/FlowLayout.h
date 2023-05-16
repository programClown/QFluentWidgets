#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

class FlowLayout : public QLayout
{
public:
    /**
     * @brief FlowLayout
     * @param parent: parent window or layout
     * @param needAni: whether to add moving animation
     * @param isTight: whether to use the tight layout when widgets are hidden
     */
    FlowLayout(QWidget *parent = nullptr, bool needAni = false, bool isTight = false);
    ~FlowLayout();

public:
    QSize sizeHint() const override;
    void addItem(QLayoutItem *) override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    int count() const override;

    QSize minimumSize() const override;
    Qt::Orientations expandingDirections() const override;
    void setGeometry(const QRect &) override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;

    void addWidget(QWidget *w);
    void setAnimation(int duration, QEasingCurve ease = QEasingCurve::Linear);
    void removeAllItems();
    void takeAllWidgets();

    int verticalSpacing() const;
    void setVerticalSpacing(int verticalSpacing);

    int horizontalSpacing() const;
    void setHorizontalSpacing(int horizontalSpacing);

protected:
    int doLayout(const QRect &rect, bool move) const;

private:
    QList<QLayoutItem *> m_items;
    QList<QPropertyAnimation *> m_anis;
    QParallelAnimationGroup *m_aniGroup;
    int m_verticalSpacing;
    int m_horizontalSpacing;
    bool m_needAni;
    bool m_isTight;
};

#endif  // FLOWLAYOUT_H
