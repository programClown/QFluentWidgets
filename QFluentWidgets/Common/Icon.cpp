#include "Icon.h"
#include "QFluentWidgets.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QDomDocument>
#include <QHash>
#include <QFile>
#include <QDebug>

static void drawSvgIcon(const QString &file, QPainter *painter, QRect rect)
{
    QSvgRenderer svgRender(file);
    svgRender.render(painter, QRectF(rect));
}

IconEngine::IconEngine(const QString &path) : QIconEngine(), m_iconPath(path) { }

void IconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode /*mode*/, QIcon::State /*state*/)
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (!m_iconPath.toLower().endsWith("svg")) {
        painter->drawPixmap(rect, QPixmap(m_iconPath));
    } else {
        drawSvgIcon(m_iconPath, painter, rect);
    }
}

QPixmap IconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    paint(&painter, QRect(QPoint(0, 0), size), mode, state);

    return pixmap;
}

QIconEngine *IconEngine::clone() const
{
    return new IconEngine(*this);
}

Icon::Icon(const QString &iconPath) : QIcon(new IconEngine(iconPath)), m_iconPath(iconPath) { }

MenuIconEngine::MenuIconEngine(const QIcon &icon) : m_icon(icon) { }

void MenuIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode /*mode*/, QIcon::State state)
{
    m_icon.paint(painter, rect, Qt::AlignHCenter, QIcon::Normal, state);
}

QIconEngine *MenuIconEngine::clone() const
{
    return new MenuIconEngine(*this);
}

QString getIconColor()
{
    if (QFWIns.isDarkTheme()) {
        return "white";
    } else {
        return "black";
    }
}

static QString writeSvg(const QString &iconPath, const QVector<int> &indexes, const QHash<QString, QString> &attributes)
{
    if (!iconPath.toLower().endsWith("svg")) {
        return "";
    }

    QFile f(iconPath);
    f.open(QIODevice::ReadOnly | QIODevice::Text);

    QDomDocument dom;
    dom.setContent(f.readAll());
    f.close();

    // change the color of each path
    QDomNodeList pathNodes = dom.elementsByTagName("path");

    QVector<int> _indexes = indexes;
    if (_indexes.isEmpty()) {
        for (int i = 0; i < pathNodes.length(); ++i) {
            _indexes.append(i);
        }
    }

    for (auto index : _indexes) {
        QDomElement element = pathNodes.at(index).toElement();

        QHashIterator<QString, QString> iter(attributes);
        while (iter.hasNext()) {
            iter.next();
            element.setAttribute(iter.key(), iter.value());
        }
    }

    return dom.toString();
}

void FluentIconBase::render(QPainter *painter, const QRect &rect, const QVector<int> &indexes,
                            const QHash<QString, QString> &attributes)
{
    QString filePath = path();
    //    qDebug() << filePath;
    bool isSvg = filePath.toLower().endsWith("svg");
    if (isSvg) {
        if (attributes.isEmpty()) {
            QSvgRenderer svgRender(filePath);
            svgRender.render(painter, QRectF(rect));
        } else {
            QString svg = writeSvg(filePath, indexes, attributes);
            QSvgRenderer svgRender(svg.toUtf8());
            svgRender.render(painter, QRectF(rect));
        }
    } else {
        QPixmap pixmap = icon().pixmap(rect.width(), rect.height());
        painter->drawPixmap(rect, pixmap);
    }
}

FluentIcon *FluentIcon::create(FluentIcon::IconType t)
{
    if (t >= ADD && t <= BACKGROUND_FILL) {
        return new FluentIcon(t);
    }

    // default
    return nullptr;
}

FluentIcon::FluentIcon(FluentIcon::IconType type, Qfw::Theme t) : m_type(type), m_theme(t), m_file(""), isIconType(true)
{
}

FluentIcon::FluentIcon(const QString &file, Qfw::Theme t)
    : m_type(EMPTY_ICON), m_theme(t), m_file(file), isIconType(false)
{
}

QString FluentIcon::iconName(FluentIcon::IconType type)
{
    switch (type) {
        case EMPTY_ICON:
            return "Empty";
        case ADD:
            return "Add";
        case CUT:
            return "Cut";
        case PIN:
            return "Pin";
        case TAG:
            return "Tag";
        case CHAT:
            return "Chat";
        case COPY:
            return "Copy";
        case CODE:
            return "Code";
        case EDIT:
            return "Edit";
        case FONT:
            return "Font";
        case HELP:
            return "Help";
        case HIDE:
            return "Hide";
        case HOME:
            return "Home";
        case INFO:
            return "Info";
        case LINK:
            return "Link";
        case MAIL:
            return "Mail";
        case MENU:
            return "Menu";
        case MORE:
            return "More";
        case SAVE:
            return "Save";
        case SEND:
            return "Send";
        case SYNC:
            return "Sync";
        case VIEW:
            return "View";
        case ZOOM:
            return "Zoom";
        case ALBUM:
            return "Album";
        case BRUSH:
            return "Brush";
        case CLOSE:
            return "Close";
        case EMBED:
            return "Embed";
        case GLOBE:
            return "Globe";
        case HEART:
            return "Heart";
        case MEDIA:
            return "Media";
        case MOVIE:
            return "Movie";
        case MUSIC:
            return "Music";
        case PASTE:
            return "Paste";
        case PHOTO:
            return "Photo";
        case PHONE:
            return "Phone";
        case PRINT:
            return "Print";
        case SHARE:
            return "Share";
        case UNPIN:
            return "Unpin";
        case VIDEO:
            return "Video";
        case ACCEPT:
            return "Accept";
        case CAMERA:
            return "Camera";
        case CANCEL:
            return "Cancel";
        case DELETE:
            return "Delete";
        case FOLDER:
            return "Folder";
        case SCROLL:
            return "Scroll";
        case LAYOUT:
            return "Layout";
        case GITHUB:
            return "GitHub";
        case UPDATE:
            return "Update";
        case RETURN:
            return "Return";
        case RINGER:
            return "Ringer";
        case SEARCH:
            return "Search";
        case SAVE_AS:
            return "SaveAs";
        case ZOOM_IN:
            return "ZoomIn";
        case HISTORY:
            return "History";
        case SETTING:
            return "Setting";
        case PALETTE:
            return "Palette";
        case MESSAGE:
            return "Message";
        case ZOOM_OUT:
            return "ZoomOut";
        case FEEDBACK:
            return "Feedback";
        case MINIMIZE:
            return "Minimize";
        case CHECKBOX:
            return "CheckBox";
        case DOCUMENT:
            return "Document";
        case LANGUAGE:
            return "Language";
        case DOWNLOAD:
            return "Download";
        case QUESTION:
            return "Question";
        case DATE_TIME:
            return "DateTime";
        case SEND_FILL:
            return "SendFill";
        case COMPLETED:
            return "Completed";
        case CONSTRACT:
            return "Constract";
        case ALIGNMENT:
            return "Alignment";
        case BOOK_SHELF:
            return "BookShelf";
        case HIGHTLIGHT:
            return "Highlight";
        case FOLDER_ADD:
            return "FolderAdd";
        case PENCIL_INK:
            return "PencilInk";
        case ZIP_FOLDER:
            return "ZipFolder";
        case MICROPHONE:
            return "Microphone";
        case ARROW_DOWN:
            return "ChevronDown";
        case TRANSPARENT:
            return "Transparent";
        case MUSIC_FOLDER:
            return "MusicFolder";
        case CHEVRON_RIGHT:
            return "ChevronRight";
        case BACKGROUND_FILL:
            return "BackgroundColor";
    }

    return "Unkown";
}

FluentIcon::IconType FluentIcon::type() const
{
    return m_type;
}

QString FluentIcon::typeName() const
{
    return iconName(m_type);
}

QString FluentIcon::path()
{
    if (m_type == EMPTY_ICON) {
        return "";
    }

    if (isIconType) {
        QString c;
        if (m_theme == Qfw::Theme::AUTO) {
            c = getIconColor();
        } else if (m_theme == Qfw::Theme::DARK) {
            c = "black";
        } else {
            c = "white";
        }

        return QString("../QFluentWidgets/images/icons/%1_%2.svg").arg(iconName(m_type)).arg(c);
    }

    return m_file;
}

QIcon FluentIcon::icon()
{
    if (m_type == EMPTY_ICON) {
        return QIcon();
    }
    return QIcon(path());
}

Qfw::Theme FluentIcon::theme() const
{
    return m_theme;
}

void FluentIcon::setTheme(const Qfw::Theme &theme)
{
    m_theme = theme;
}
