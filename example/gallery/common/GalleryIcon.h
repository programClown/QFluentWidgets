#ifndef GALLERYICON_H
#define GALLERYICON_H

#include <Common/Icon.h>

class GalleryIcon : public FluentIconBase
{
    Q_OBJECT
public:
    enum IconType
    {
        GRID,
        MENU,
        TEXT,
        EMOJI_TAB_SYMBOLS,
    };

    Q_ENUM(IconType)

    static QString iconName(IconType type);

    GalleryIcon(IconType type, Qfw::Theme t = Qfw::AUTO);
    ~GalleryIcon();

    QString iconPath();

    QIcon icon() override;
    QString typeName() const override;
    QString enumName() const override;
    FluentIconBase *clone() override;

    Qfw::Theme theme() const;
    void setTheme(const Qfw::Theme &theme) override;

private:
    Qfw::Theme m_theme;
    IconType m_type;
};

#endif  // GALLERYICON_H
