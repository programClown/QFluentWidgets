#ifndef AVATARWIDGET_H
#define AVATARWIDGET_H

#include <Navigation/NavigationWidget.h>

class AvatarWidget : public NavigationWidget
{
public:
    AvatarWidget(const QString imagePath, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

private:
    QImage m_avatar;
};

#endif  // AVATARWIDGET_H
