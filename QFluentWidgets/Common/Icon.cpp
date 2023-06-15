#include "Icon.h"
#include "QFluentWidgets.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QDomDocument>
#include <QHash>
#include <QFile>

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

IconEngine::IconEngine(const QString &path)
    : QIconEngine(), m_iconPath(path), m_indexes(QVector<int>()), m_attributes(QHash<QString, QString>())
{
    m_isSvg = m_iconPath.toLower().endsWith("svg");
}

void IconEngine::setIndexes(const QVector<int> &indexes)
{
    m_indexes = indexes;
}

void IconEngine::setAttributes(const QHash<QString, QString> &attributes)
{
    m_attributes = attributes;
}

void IconEngine::setIconPath(const QString &iconPath)
{
    m_iconPath = iconPath;
    m_isSvg    = m_iconPath.toLower().endsWith("svg");
}

QString IconEngine::iconPath() const
{
    return m_iconPath;
}

void IconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode /*mode*/, QIcon::State /*state*/)
{
    if (m_iconPath.isEmpty()) {
        return;
    }
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (m_isSvg) {
        if (m_attributes.isEmpty()) {
            QSvgRenderer svgRender(m_iconPath);
            svgRender.render(painter, QRectF(rect));
        } else {
            QString svg = writeSvg(m_iconPath, m_indexes, m_attributes);
            QSvgRenderer svgRender(svg.toUtf8());
            svgRender.render(painter, QRectF(rect));
        }
    } else {
        QPixmap pixmap = QIcon(m_iconPath).pixmap(rect.width(), rect.height());
        painter->drawPixmap(rect, pixmap);
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
    return new IconEngine(m_iconPath);
}

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

FluentIconBase::FluentIconBase(const QString &path) : iconEngine(new IconEngine(path)) { }

FluentIconBase::~FluentIconBase() { }

void FluentIconBase::render(QPainter *painter, const QRect &rect, const QVector<int> &indexes,
                            const QHash<QString, QString> &attributes)
{
    iconEngine->setIndexes(indexes);
    iconEngine->setAttributes(attributes);
    iconEngine->paint(painter, rect, QIcon::Normal, QIcon::On);
}

QString FluentIcon::iconName(FluentIcon::IconType type)
{
    switch (type) {
        case UNKNOWN:
            return "Unknown";
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

    return "Unknown";
}

FluentIcon::FluentIcon(const QString &customPath)
    : FluentIconBase(customPath), m_theme(Qfw::Theme::AUTO), m_type(UNKNOWN)
{
}

FluentIcon::FluentIcon(IconType type, Qfw::Theme t) : FluentIconBase(""), m_theme(t), m_type(type)
{
    iconEngine->setIconPath(iconPath());
}

FluentIcon::~FluentIcon() { }

QString FluentIcon::iconPath()
{
    if (m_type == UNKNOWN) {
        return "";
    }

    QString c;
    if (m_theme == Qfw::Theme::AUTO) {
        c = getIconColor();
    } else if (m_theme == Qfw::Theme::DARK) {
        c = "black";
    } else {
        c = "white";
    }

    return QString(":/qfluentwidgets/images/icons/%1_%2.svg").arg(iconName(m_type)).arg(c);
}

QIcon FluentIcon::icon()
{
    return QIcon(iconEngine->clone());
}

QString FluentIcon::typeName() const
{
    return iconName(m_type);
}

QString FluentIcon::enumName() const
{
    QMetaEnum metaEnum = QMetaEnum::fromType<IconType>();
    return metaEnum.valueToKey(m_type);
}

FluentIconBase *FluentIcon::clone()
{
    if (m_type == UNKNOWN) {
        return new FluentIcon(iconEngine->iconPath());
    }

    return new FluentIcon(m_type, m_theme);
}

Qfw::Theme FluentIcon::theme() const
{
    return m_theme;
}

void FluentIcon::setTheme(const Qfw::Theme &theme)
{
    m_theme = theme;
    iconEngine->setIconPath(iconPath());
}
