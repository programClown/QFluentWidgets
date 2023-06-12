#ifndef GALLERYSTYLESHEET_H
#define GALLERYSTYLESHEET_H

#include <Common/StyleSheet.h>

class GalleryStyleSheet : public StyleSheetBase
{
public:
    enum Type
    {
        LINK_CARD,          //"link_card"
        MAIN_WINDOW,        // "main_window"
        SAMPLE_CARD,        // "sample_card"
        HOME_INTERFACE,     // "home_interface"
        ICON_INTERFACE,     // "icon_interface"
        VIEW_INTERFACE,     // "view_interface"
        SETTING_INTERFACE,  // "setting_interface"
        GALLERY_INTERFACE,  // "gallery_interface"
    };

    static QString typeName(Type t);

    QString path(Qfw::Theme theme) override;
    static QString content(const QString &name);
    QString operator[](Type t);

    static void apply(Type t, QWidget *widget);

    static void apply(const QString &name, QWidget *widget);
};

#endif  // GALLERYSTYLESHEET_H
