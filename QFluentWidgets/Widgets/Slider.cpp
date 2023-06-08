#include "Slider.h"
#include "Common/StyleSheet.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionComplex>

Slider::Slider(Qt::Orientation orientation, QWidget *parent) : QSlider(orientation, parent)
{
    FluentStyleSheet::apply("SLIDER", this);
}

void Slider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);

    int value = 0;
    if (orientation() == Qt::Horizontal) {
        value = event->pos().x() * maximum() / width();
    } else {
        value = (height() - event->pos().y()) * maximum() / height();
    }

    setValue(value);
    emit clicked(value);
}

HollowHandleStyle::HollowHandleStyle(const QMap<QString, QVariant> &cfg)
{
    config.insert("groove.height", 3);
    config.insert("sub-page.color", QColor(255, 255, 255));
    config.insert("add-page.color", QColor(255, 255, 255, 64));
    config.insert("handle.color", QColor(255, 255, 255));
    config.insert("handle.ring-width", 4);
    config.insert("handle.hollow-radius", 6);
    config.insert("handle.margin", 4);

    QMapIterator<QString, QVariant> i(cfg);
    while (i.hasNext()) {
        i.next();
        config.insert(i.key(), i.value());
    }

    // get handle size
    int w = config["handle.margin"].toInt() + config["handle.ring-width"].toInt()
            + config["handle.hollow-radius"].toInt();
    config.insert("handle.size", QSize(2 * w, 2 * w));
}

void HollowHandleStyle::drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p,
                                           const QWidget *widget) const
{
    // draw sub control
    const QStyleOptionSlider *opts = static_cast<const QStyleOptionSlider *>(opt);
    if (!opts) {
        return QProxyStyle::drawComplexControl(cc, opt, p, widget);
    }

    if (cc != this->CC_Slider || (opts->orientation != Qt::Horizontal)) {
        return QProxyStyle::drawComplexControl(cc, opt, p, widget);
    }

    QRect grooveRect = subControlRect(cc, opt, SC_SliderGroove, widget);
    QRect handleRect = subControlRect(cc, opt, SC_SliderHandle, widget);
    p->setRenderHints(QPainter::Antialiasing);
    p->setPen(Qt::NoPen);

    // paint groove
    p->save();
    p->translate(grooveRect.topLeft());

    // paint the crossed part
    int w = handleRect.x() - grooveRect.x();
    int h = config["groove.height"].toInt();
    p->setBrush(config["sub-page.color"].value<QColor>());
    p->drawRect(0, 0, w, h);

    // paint the uncrossed part
    int x = w + config["handle.size"].toSize().width();
    p->setBrush(config["add-page.color"].value<QColor>());
    p->drawRect(x, 0, grooveRect.width() - w, h);
    p->restore();

    // paint handle
    int ringWidth    = config["handle.ring-width"].toInt();
    int hollowRadius = config["handle.hollow-radius"].toInt();
    int radius       = ringWidth + hollowRadius;

    QPainterPath path;
    path.moveTo(0, 0);
    QPoint center = handleRect.center() + QPoint(1, 1);
    path.addEllipse(center, radius, radius);
    path.addEllipse(center, hollowRadius, hollowRadius);

    QColor handleColor = config["handle.color"].value<QColor>();
    if (opt->activeSubControls != SC_SliderHandle) {
        handleColor.setAlpha(255);
    } else {
        handleColor.setAlpha(153);
    }

    p->setBrush(handleColor);
    p->drawPath(path);

    // press handle
    const QAbstractSlider *wslider = qobject_cast<const QAbstractSlider *>(widget);
    if (wslider && wslider->isSliderDown()) {
        handleColor.setAlpha(255);
        p->setBrush(handleColor);
        p->drawEllipse(handleRect);
    }
}

QRect HollowHandleStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt,
                                        QStyle::SubControl sc, const QWidget *widget) const
{
    // get the rectangular area occupied by the sub control
    const QStyleOptionSlider *opts = static_cast<const QStyleOptionSlider *>(opt);
    if (!opts) {
        return QProxyStyle::subControlRect(cc, opt, sc, widget);
    }

    if (cc != this->CC_Slider || (opts->orientation != Qt::Horizontal) || sc == SC_SliderTickmarks) {
        return QProxyStyle::subControlRect(cc, opt, sc, widget);
    }

    QRect rect = opt->rect;

    if (sc == SC_SliderGroove) {
        int h = config["groove.height"].toInt();
        QRectF grooveRect(0, (rect.height() - h) / 2.0, rect.width(), h);
        return grooveRect.toRect();
    } else if (sc == SC_SliderHandle) {
        QSize size = config["handle.size"].toSize();
        double x   = sliderPositionFromValue(opts->minimum, opts->maximum, opts->sliderPosition, rect.width());

        // solve the situation that the handle runs out of slider
        x *= (rect.width() - size.width()) * 1.0 / rect.width();
        QRectF sliderRect(x, 0, size.width(), size.height());
        return sliderRect.toRect();
    }

    // 正常不会到这里
    return QProxyStyle::subControlRect(cc, opt, sc, widget);
}
