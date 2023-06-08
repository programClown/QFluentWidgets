#include "Icon.h"
#include "QFluentWidgets.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QDomDocument>
#include <QHash>
#include <QFile>
#include <QDebug>

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

void FluentIconBase::render(QPainter *painter, const QRect &rect, const QString &themeColor, bool useTheme)
{
    painter->save();

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
#endif
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPaintDevice *device = painter->device();
    QWidget *widget      = static_cast<QWidget *>(device);
    QString iconPath     = path();
    QImage img(iconPath);
    QColor fillColor;
    bool colorChange = false;
    if (useTheme) {
        if (!themeColor.isEmpty()) {
            colorChange = true;
            fillColor   = QColor(themeColor);
        } else if (widget && (iconPath.toLower().endsWith("svg") || iconPath.toLower().endsWith("png"))) {
            auto palette = widget->palette();
            fillColor    = palette.color(QPalette::Foreground);
            colorChange  = true;
        }

        if (colorChange) {
            QPainter p(&img);
            p.setCompositionMode(QPainter::CompositionMode_SourceIn);
            p.fillRect(img.rect(), fillColor);
        }
    }

    painter->drawImage(rect, img);
    painter->restore();
}

FluentIcon *FluentIcon::create(FluentIcon::IconType t)
{
    if (t >= ADD && t <= INFO_BAR_ERROR) {
        return new FluentIcon(t);
    }

    // default
    return nullptr;
}

FluentIcon *FluentIcon::create(const QString &file)
{
    return new FluentIcon(file);
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
            return "icons/Empty";
        case ADD:
            return "icons/Add";
        case CUT:
            return "icons/Cut";
        case PIN:
            return "icons/Pin";
        case TAG:
            return "icons/Tag";
        case CHAT:
            return "icons/Chat";
        case COPY:
            return "icons/Copy";
        case CODE:
            return "icons/Code";
        case EDIT:
            return "icons/Edit";
        case FONT:
            return "icons/Font";
        case HELP:
            return "icons/Help";
        case HIDE:
            return "icons/Hide";
        case HOME:
            return "icons/Home";
        case INFO:
            return "icons/Info";
        case LINK:
            return "icons/Link";
        case MAIL:
            return "icons/Mail";
        case MENU:
            return "Menu";
        case MORE:
            return "icons/More";
        case SAVE:
            return "icons/Save";
        case SEND:
            return "icons/Send";
        case SYNC:
            return "icons/Sync";
        case VIEW:
            return "icons/View";
        case ZOOM:
            return "icons/Zoom";
        case ALBUM:
            return "icons/Album";
        case BRUSH:
            return "icons/Brush";
        case CLOSE:
            return "icons/Close";
        case EMBED:
            return "icons/Embed";
        case GLOBE:
            return "icons/Globe";
        case HEART:
            return "icons/Heart";
        case MEDIA:
            return "icons/Media";
        case MOVIE:
            return "icons/Movie";
        case MUSIC:
            return "icons/Music";
        case PASTE:
            return "icons/Paste";
        case PHOTO:
            return "icons/Photo";
        case PHONE:
            return "icons/Phone";
        case PRINT:
            return "icons/Print";
        case SHARE:
            return "icons/Share";
        case UNPIN:
            return "icons/Unpin";
        case VIDEO:
            return "icons/Video";
        case ACCEPT:
            return "icons/Accept";
        case CAMERA:
            return "icons/Camera";
        case CANCEL:
            return "icons/Cancel";
        case DELETE:
            return "icons/Delete";
        case FOLDER:
            return "icons/Folder";
        case SCROLL:
            return "icons/Scroll";
        case LAYOUT:
            return "icons/Layout";
        case GITHUB:
            return "icons/GitHub";
        case UPDATE:
            return "icons/Update";
        case RETURN:
            return "icons/Return";
        case RINGER:
            return "icons/Ringer";
        case SEARCH:
            return "icons/Search";
        case SAVE_AS:
            return "icons/SaveAs";
        case ZOOM_IN:
            return "icons/ZoomIn";
        case HISTORY:
            return "icons/History";
        case SETTING:
            return "icons/Setting";
        case PALETTE:
            return "icons/Palette";
        case MESSAGE:
            return "icons/Message";
        case ZOOM_OUT:
            return "icons/ZoomOut";
        case FEEDBACK:
            return "icons/Feedback";
        case MINIMIZE:
            return "icons/Minimize";
        case CHECKBOX:
            return "icons/CheckBox";
        case DOCUMENT:
            return "icons/Document";
        case LANGUAGE:
            return "icons/Language";
        case DOWNLOAD:
            return "icons/Download";
        case QUESTION:
            return "icons/Question";
        case DATE_TIME:
            return "icons/DateTime";
        case SEND_FILL:
            return "icons/SendFill";
        case COMPLETED:
            return "icons/Completed";
        case CONSTRACT:
            return "icons/Constract";
        case ALIGNMENT:
            return "icons/Alignment";
        case BOOK_SHELF:
            return "icons/BookShelf";
        case HIGHTLIGHT:
            return "icons/Highlight";
        case FOLDER_ADD:
            return "icons/FolderAdd";
        case PENCIL_INK:
            return "icons/PencilInk";
        case ZIP_FOLDER:
            return "icons/ZipFolder";
        case MICROPHONE:
            return "icons/Microphone";
        case ARROW_DOWN:
            return "icons/ChevronDown";
        case TRANSPARENT:
            return "icons/Transparent";
        case MUSIC_FOLDER:
            return "icons/MusicFolder";
        case CHEVRON_RIGHT:
            return "icons/ChevronRight";
        case BACKGROUND_FILL:
            return "icons/BackgroundColor";

        case SPIN_BOX_UP:
            return "spin_box/Up";
        case SPIN_BOX_DOWN:
            return "spin_box/Down";

        case TIME_PICKER_UP:
            return "time_picker/Up";
        case TIME_PICKER_DOWN:
            return "time_picker/Down";

        case INFO_BAR_WARN:
            return "info_bar/Warning";
        case INFO_BAR_SUCCESS:
            return "info_bar/Success";
        case INFO_BAR_INFORMATION:
            return "info_bar/Info";
        case INFO_BAR_ERROR:
            return "info_bar/Error";
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
    if (isIconType && m_type == EMPTY_ICON) {
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
        if (m_type >= INFO_BAR_WARN && m_type <= INFO_BAR_ERROR) {
            QString colorName;
            if (m_theme == Qfw::Theme::AUTO) {
                colorName = QFWIns.isDarkTheme() ? "dark" : "light";
            } else {
                colorName = Qfw::ThemeString(m_theme).toLower();
            }

            return QString(":/qfluentwidgets/images/%1_%2.svg").arg(iconName(m_type)).arg(colorName);
        }

        return QString(":/qfluentwidgets/images/%1_%2.svg").arg(iconName(m_type)).arg(c);
    }

    return m_file;
}

QIcon FluentIcon::icon()
{
    if (m_type == EMPTY_ICON) {
        if (m_file.isEmpty()) {
            return QIcon();
        }

        return QIcon(m_file);
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
