#ifndef GALLERYCONFIG_H
#define GALLERYCONFIG_H

#include <Common/Config.h>

class GalleryConfig : public Qfw::QConfig
{
    Q_OBJECT
public:
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

public:
    static GalleryConfig *cfg();

public:
    // folders
    Qfw::ConfigItem *musicFolders;
    Qfw::ConfigItem *downloadFolder;

    // mainwindow

private:
};

const int YEAR             = 2023;
const QString AUTHOR       = "zhiyiYo";
const QString VERSION      = "v0.7.0";
const QString HELP_URL     = "https://pyqt-fluent-widgets.readthedocs.io";
const QString REPO_URL     = "https://github.com/zhiyiYo/PyQt-Fluent-Widgets";
const QString EXAMPLE_URL  = "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/tree/master/examples";
const QString FEEDBACK_URL = "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/issues";
const QString RELEASE_URL  = "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/releases/latest";

#endif  // GALLERYCONFIG_H
