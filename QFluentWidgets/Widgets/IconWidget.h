#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>
#include "Common/Icon.h"

class IconWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IconWidget(FluentIconBase *ficon, QWidget *parent = nullptr);

    void setIcon(FluentIconBase *icon);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

signals:

private:
    QScopedPointer<FluentIconBase> m_ficon;
};

#endif  // ICONWIDGET_H
