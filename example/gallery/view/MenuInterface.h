#ifndef MENUINTERFACE_H
#define MENUINTERFACE_H

#include "GalleryInterface.h"

class MenuInterface : public GalleryInterface
{
public:
    MenuInterface(QWidget *parent = nullptr);

    void createMenu(QPoint pos);
};

#endif  // MENUINTERFACE_H
