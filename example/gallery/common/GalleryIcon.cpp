#include "GalleryIcon.h"

#include <QFluentWidgets.h>

QString GalleryIcon::iconName(GalleryIcon::IconType type)
{
    switch (type) {
        case GRID:
            return "Grid";
        case MENU:
            return "Menu";
        case TEXT:
            return "Text";
        case EMOJI_TAB_SYMBOLS:
            return "EmojiTabSymbols";
    }

    return "Unknown";
}

GalleryIcon::GalleryIcon(GalleryIcon::IconType type, Qfw::Theme t) : FluentIconBase(""), m_theme(t), m_type(type)
{
    iconEngine->setIconPath(iconPath());
}

GalleryIcon::~GalleryIcon() { }

QString GalleryIcon::iconPath()
{
    QString colorName;
    if (m_theme == Qfw::Theme::AUTO) {
        colorName = QFWIns.isDarkTheme() ? "white" : "black";
    } else {
        colorName = Qfw::ThemeString(m_theme).toLower();
    }

    return QString(":/resource/images/icons/%1_%2.svg").arg(iconName(m_type)).arg(colorName);
}

QIcon GalleryIcon::icon()
{
    return QIcon(iconEngine->clone());
}

QString GalleryIcon::typeName() const
{
    return iconName(m_type);
}

QString GalleryIcon::enumName() const
{
    QMetaEnum metaEnum = QMetaEnum::fromType<IconType>();
    return metaEnum.valueToKey(m_type);
}

FluentIconBase *GalleryIcon::clone()
{
    return new GalleryIcon(m_type, m_theme);
}

Qfw::Theme GalleryIcon::theme() const
{
    return m_theme;
}

void GalleryIcon::setTheme(const Qfw::Theme &theme)
{
    m_theme = theme;
    iconEngine->setIconPath(iconPath());
}
