#ifndef ICON_H
#define ICON_H

#include <QIconEngine>
#include "StyleSheet.h"

QString getIconColor();

class IconEngine : public QIconEngine
{
public:
    IconEngine(const QString &path);

    void setIndexes(const QVector<int> &indexes);
    void setAttributes(const QHash<QString, QString> &attributes);
    void setIconPath(const QString &iconPath);
    QString iconPath() const;

    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    QIconEngine *clone() const override;

private:
    QString m_iconPath;
    QVector<int> m_indexes;
    QHash<QString, QString> m_attributes;
    bool m_isSvg;
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

class FluentIconBase : public QObject
{
    Q_OBJECT
public:
    explicit FluentIconBase(const QString &path);
    virtual ~FluentIconBase();

    virtual QIcon icon()             = 0;
    virtual QString typeName() const = 0;
    virtual QString enumName() const = 0;
    virtual FluentIconBase *clone()  = 0;

    virtual void setTheme(const Qfw::Theme &theme) = 0;

    virtual void render(QPainter *painter, const QRect &rect, const QVector<int> &indexes = QVector<int>(),
                        const QHash<QString, QString> &attributes = QHash<QString, QString>());

    QScopedPointer<IconEngine> iconEngine;
};

class FluentIcon : public FluentIconBase
{
    Q_OBJECT
public:
    enum IconType
    {
        UNKNOWN = -1,
        ADD     = 0,
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

    Q_ENUM(IconType)

    static QString iconName(IconType type);

    FluentIcon(const QString &customPath);
    FluentIcon(IconType type, Qfw::Theme t = Qfw::Theme::AUTO);
    ~FluentIcon();

    QString iconPath();

    // FluentIconBase interface
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

#define NEWFLICON(cls, type) (new cls(cls::type))

#endif  // ICON_H
