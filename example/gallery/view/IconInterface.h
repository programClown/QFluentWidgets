#ifndef ICONINTERFACE_H
#define ICONINTERFACE_H

#include "GalleryInterface.h"

#include <Widgets/LineEdit.h>

class FluentIcon;

class GalleryLineEdit : public SearchLineEdit
{
    Q_OBJECT
public:
    GalleryLineEdit(QWidget *parent = nullptr);
};

class IconCard : public QFrame
{
    Q_OBJECT

public:
    IconCard(FluentIconBase *icon, QWidget *parent = nullptr);

    void mouseReleaseEvent(QMouseEvent *event) override;

    void setSelected(bool isSelected, bool force = false);

signals:
    void clicked(FluentIconBase *icon);

private:
    QScopedPointer<FluentIconBase> m_icon;
    bool m_isSelected = false;

    IconWidget *m_iconWidget;
    QLabel *m_nameLabel;
    QVBoxLayout *m_vBoxLayout;
};

class IconInfoPanel : public QFrame
{
public:
    IconInfoPanel(FluentIconBase *icon, QWidget *parent = nullptr);

    void setIcon(FluentIconBase *icon);

private:
    QLabel *m_nameLabel;
    IconWidget *m_iconWidget;
    QLabel *m_iconNameTitleLabel;
    QLabel *m_iconNameLabel;
    QLabel *m_enumNameTitleLabel;
    QLabel *m_enumNameLabel;

    QVBoxLayout *m_vBoxLayout;
};

class Trie;
class FlowLayout;

class IconCardView : public QWidget
{
    Q_OBJECT
public:
    IconCardView(QWidget *parent = nullptr);

    void initWidget();

    void addIcon(FluentIconBase *icon);

    void setSelectedIcon(FluentIconBase *icon);

    void search(QString keyWord);

    void showAllIcons();

private:
    void setQss();

private:
    Trie *m_trie;
    QLabel *m_iconLibraryLabel;
    GalleryLineEdit *m_searchLineEdit;

    QFrame *m_view;
    SmoothScrollArea *m_scrollArea;
    QWidget *m_scrollWidget;
    IconInfoPanel *m_infoPannel;

    QVBoxLayout *m_vBoxLayout;
    QHBoxLayout *m_hBoxLayout;
    FlowLayout *m_flowLayout;

    QList<IconCard *> m_cards;
    QList<FluentIconBase *> m_icons;
    int m_currentIndex = -1;
};

class IconInterface : public GalleryInterface
{
public:
    IconInterface(QWidget *parent = nullptr);

private:
    IconCardView *m_iconView;
};

#endif  // ICONINTERFACE_H
