#ifndef HOMEINTERFACE_H
#define HOMEINTERFACE_H

#include <Widgets/ScrollArea.h>

class QVBoxLayout;
class QLabel;
class LinkCardView;

class BannerWidget : public QWidget
{

public:
    BannerWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

private:
    QVBoxLayout *m_vBoxLayout;
    QLabel *m_galleryLabel;
    QPixmap m_banner;
    LinkCardView *m_linkCardView;
};

class HomeInterface : public ScrollArea
{
public:
    HomeInterface(QWidget *parent = nullptr);

private:
    void initWidget();

    void loadSamples();

private:
    BannerWidget *m_banner;
    QWidget *m_view;
    QVBoxLayout *m_vBoxLayout;
};

#endif  // HOMEINTERFACE_H
