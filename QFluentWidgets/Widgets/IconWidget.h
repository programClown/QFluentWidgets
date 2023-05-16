#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>
#include "Common/Icon.h"

class IconWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IconWidget(QSharedPointer<FluentIconBase> ficon, QWidget *parent = nullptr);

    void setIcon(QSharedPointer<FluentIconBase> icon);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

signals:

private:
    QSharedPointer<FluentIconBase> m_ficon;
};

#endif  // ICONWIDGET_H
