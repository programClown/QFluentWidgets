#ifndef LAYOUTINTERFACE_H
#define LAYOUTINTERFACE_H

#include "GalleryInterface.h"

class LayoutInterface : public GalleryInterface
{
public:
    LayoutInterface(QWidget *parent = nullptr);

    QWidget *createWidget(bool animation = false);
};

#endif  // LAYOUTINTERFACE_H
