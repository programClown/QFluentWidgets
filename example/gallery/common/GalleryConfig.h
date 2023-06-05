#ifndef GALLERYCONFIG_H
#define GALLERYCONFIG_H

#include <Common/Config.h>

namespace Gallery
{
    enum Language
    {
        CHINESE_SIMPLIFIED,
        CHINESE_TRADITIONAL,
        ENGLISH,
        AUTO
    };

    static inline QString LanguageString(Language lang)
    {
        switch (lang) {
            case CHINESE_SIMPLIFIED:
                return "zh";
            case CHINESE_TRADITIONAL:
                return "hk";
            case ENGLISH:
                return "en";
            case AUTO:
                return "Auto";
        }
    }
}  // namespace Gallery

class GalleryConfig : public Qfw::QConfig
{
    Q_OBJECT
public:
    GalleryConfig();

private:
};

#endif  // GALLERYCONFIG_H
