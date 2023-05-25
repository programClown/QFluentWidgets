#ifndef VBOXLAYOUT_H
#define VBOXLAYOUT_H

#include <QVBoxLayout>

class VBoxLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit VBoxLayout(QWidget *parent = nullptr);
    ~VBoxLayout();

    void addWidgets(const QList<QWidget *> &widgets, int stretch = 0, Qt::Alignment alignment = Qt::AlignTop);
    void addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::AlignTop);
    void removeWidget(QWidget *widget);
    void deleteWidget(QWidget *widget);
    void removeAllWidget();
    void removeAllItems();

    QList<QWidget *> widgets() const;

signals:

private:
    QList<QWidget *> m_widgets;
};

#endif  // VBOXLAYOUT_H
