#ifndef STATUSINFOINTERFACE_H
#define STATUSINFOINTERFACE_H

#include "GalleryInterface.h"

class StateToolTip;

class StatusInfoInterface : public GalleryInterface
{
    Q_OBJECT
public:
    StatusInfoInterface(QWidget *parent = nullptr);

private slots:

    void onStateButtonClicked();

    void createTopRightInfoBar();

    void createTopInfoBar();

    void createTopLeftInfoBar();

    void createBottomRightInfoBar();

    void createBottomInfoBar();

    void createBottomLeftInfoBar();

private:
    StateToolTip *m_stateTooltip = nullptr;
};

#endif  // STATUSINFOINTERFACE_H
