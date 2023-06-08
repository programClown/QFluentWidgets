#ifndef ICON_H
#define ICON_H

#include <QIconEngine>
#include "StyleSheet.h"

QString getIconColor();

class IconEngine : public QIconEngine
{
public:
    IconEngine(const QString &path);

private:
    QString m_iconPath;

    // QIconEngine interface
public:
    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    QIconEngine *clone() const override;
};

class Icon : public QIcon
{
public:
    Icon(const QString &iconPath);

    QString m_iconPath;
};

class MenuIconEngine : public QIconEngine
{
public:
    MenuIconEngine(const QIcon &icon);

    QIcon m_icon;

    // QIconEngine interface
public:
    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
    QIconEngine *clone() const override;
};

class FluentIconBase
{
public:
    virtual QString path() = 0;

    virtual QIcon icon() = 0;

    virtual void setTheme(const Qfw::Theme &theme) = 0;

    virtual void render(QPainter *painter, const QRect &rect, const QVector<int> &indexes = QVector<int>(),
                        const QHash<QString, QString> &attributes = QHash<QString, QString>());
};

class FluentIcon : public FluentIconBase
{
public:
    enum IconType
    {
        EMPTY_ICON = -1,
        ADD        = 0,
        CUT,
        PIN,
        TAG,
        CHAT,
        COPY,
        CODE,
        EDIT,
        FONT,
        HELP,
        HIDE,
        HOME,
        INFO,
        LINK,
        MAIL,
        MENU,
        MORE,
        SAVE,
        SEND,
        SYNC,
        VIEW,
        ZOOM,
        ALBUM,
        BRUSH,
        CLOSE,
        EMBED,
        GLOBE,
        HEART,
        MEDIA,
        MOVIE,
        MUSIC,
        PASTE,
        PHOTO,
        PHONE,
        PRINT,
        SHARE,
        UNPIN,
        VIDEO,
        ACCEPT,
        CAMERA,
        CANCEL,
        DELETE,
        FOLDER,
        SCROLL,
        LAYOUT,
        GITHUB,
        UPDATE,
        RETURN,
        RINGER,
        SEARCH,
        SAVE_AS,
        ZOOM_IN,
        HISTORY,
        SETTING,
        PALETTE,
        MESSAGE,
        ZOOM_OUT,
        FEEDBACK,
        MINIMIZE,
        CHECKBOX,
        DOCUMENT,
        LANGUAGE,
        DOWNLOAD,
        QUESTION,
        DATE_TIME,
        SEND_FILL,
        COMPLETED,
        CONSTRACT,
        ALIGNMENT,
        BOOK_SHELF,
        HIGHTLIGHT,
        FOLDER_ADD,
        PENCIL_INK,
        ZIP_FOLDER,
        MICROPHONE,
        ARROW_DOWN,
        TRANSPARENT,
        MUSIC_FOLDER,
        CHEVRON_RIGHT,
        BACKGROUND_FILL
    };

    static FluentIcon *create(IconType t);

    FluentIcon(IconType type, Qfw::Theme t = Qfw::AUTO);
    FluentIcon(const QString &file, Qfw::Theme t = Qfw::AUTO);

    static QString iconName(IconType type);
    IconType type() const;
    QString typeName() const;

    // FluentIconBase interface
    QString path() override;
    QIcon icon() override;

    // FluentIconBase interface
    Qfw::Theme theme() const;
    void setTheme(const Qfw::Theme &theme) override;

private:
    IconType m_type;
    Qfw::Theme m_theme;
    QString m_file;
    bool isIconType;
};

typedef QSharedPointer<FluentIconBase> FluentIconBaseSPtr;
typedef QSharedPointer<FluentIcon> FluentIconSPtr;

#define NEWFLICON(icon) QSharedPointer<FluentIcon>(FluentIcon::create(icon))

#endif  // ICON_H
