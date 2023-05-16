#ifndef SLIDER_H
#define SLIDER_H

#include <QMap>
#include <QProxyStyle>
#include <QSlider>

class Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(Qt::Orientation orientation, QWidget *parent = nullptr);

signals:
    void clicked(int);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

/// Hollow handle style
class HollowHandleStyle : public QProxyStyle
{
    Q_OBJECT
public:
    HollowHandleStyle(const QMap<QString, QVariant> &cfg = QMap<QString, QVariant>());

    // QStyle interface
public:
    void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p,
                            const QWidget *widget) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc,
                         const QWidget *widget) const override;

private:
    QMap<QString, QVariant> config;
};
#endif  // SLIDER_H
