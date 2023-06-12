#ifndef MATERIALINTERFACE_H
#define MATERIALINTERFACE_H

#include "GalleryInterface.h"

class AcrylicLabel;

class MaterialInterface : public GalleryInterface
{
public:
    MaterialInterface(QWidget *parent = nullptr);

    void onBlurRadiusChanged(int radius);

private:
    AcrylicLabel *m_label;
};

#endif  // MATERIALINTERFACE_H
