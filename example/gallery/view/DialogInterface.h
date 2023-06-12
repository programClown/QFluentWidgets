#ifndef DIALOGINTERFACE_H
#define DIALOGINTERFACE_H

#include "GalleryInterface.h"

class DialogInterface : public GalleryInterface
{
public:
    DialogInterface(QWidget *parent = nullptr);

    void showDialog();

    void showMessageDialog();

    void showColorDialog();
};

#endif  // DIALOGINTERFACE_H
