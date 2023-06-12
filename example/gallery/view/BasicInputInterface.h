#ifndef BASICINPUTINTERFACE_H
#define BASICINPUTINTERFACE_H

#include "GalleryInterface.h"

class SwitchButton;

class BasicInputInterface : public GalleryInterface
{
public:
    BasicInputInterface(QWidget *parent = nullptr);

    void onSwitchCheckedChanged(bool isChecked);

private:
    SwitchButton *m_switchButton;
};

#endif  // BASICINPUTINTERFACE_H
