#include "HomeInterface.h"

BannerWidget::BannerWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(336);
}

void BannerWidget::paintEvent(QPaintEvent *event) { }

HomeInterface::HomeInterface(QWidget *parent) : ScrollArea(parent) { }

void HomeInterface::initWidget() { }

void HomeInterface::loadSamples() { }
