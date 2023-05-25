#ifndef EXPANDLAYOUT_H
#define EXPANDLAYOUT_H

#include <QLayout>

class ExpandLayout : public QLayout
{
    Q_OBJECT
public:
    explicit ExpandLayout(QWidget *parent = nullptr);
    ~ExpandLayout();

    void addWidget(QWidget *w);
    QSize sizeHint() const override;
    void addItem(QLayoutItem *item) override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    int count() const override;
    QSize minimumSize() const override;
    Qt::Orientations expandingDirections() const override;
    void setGeometry(const QRect &rect) override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int width) const override;
    bool eventFilter(QObject *watched, QEvent *event) override;

    void removeAllItems();

signals:

private:
    int doLayout(const QRect &rect, bool move) const;

    QList<QLayoutItem *> m_items;
    QList<QWidget *> m_widgets;
};

#endif  // EXPANDLAYOUT_H
