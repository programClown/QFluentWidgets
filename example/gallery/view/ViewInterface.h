#ifndef VIEWINTERFACE_H
#define VIEWINTERFACE_H

#include "GalleryInterface.h"

class AFrame : public QFrame
{
public:
    AFrame(QWidget *parent = nullptr);

    void addWidget(QWidget *widget);

private:
    QHBoxLayout *m_hBoxLayout;
};

class TreeWidget;

class TreeFrame : public AFrame
{
public:
    TreeFrame(QWidget *parent = nullptr, bool enableClick = false);

private:
    TreeWidget *m_tree;
};

class ViewInterface : public GalleryInterface
{
public:
    ViewInterface(QWidget *parent = nullptr);
};

#endif  // VIEWINTERFACE_H
