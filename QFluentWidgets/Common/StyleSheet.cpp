#include "StyleSheet.h"
#include "QFluentWidgets.h"

#include <QFile>
#include <QMap>
#include <QTextCodec>
#include <QTextStream>
#include <QtMath>

StyleSheetManager::StyleSheetManager() { }

void StyleSheetManager::Register(const QString &file, QWidget *widget)
{
    if (!m_widgets.contains(widget)) {
        connect(widget, &QWidget::destroyed, [=](QObject *obj) {
            QWidget *w = static_cast<QWidget *>(obj);
            StyleSheetManager::instance().DeRegister(w);
        });
    }

    m_widgets.insert(widget, file);
}

void StyleSheetManager::DeRegister(QWidget *widget)
{
    if (!m_widgets.contains(widget)) {
        return;
    }

    m_widgets.remove(widget);
}

QHash<QWidget *, QString> StyleSheetManager::widgets() const
{
    return m_widgets;
}

QString StyleSheetBase::getStyleSheet(const QString &file, Qfw::Theme theme)
{
    QFile f(file);
    QString qss;
    if (f.open(QIODevice::ReadOnly)) {
        QTextCodec *codec = QTextCodec::codecForName("UTF8");
        qss               = codec->toUnicode(f.readAll());
        f.close();
    }

    return qss;
}

void StyleSheetBase::setStyleSheet(QWidget *widget, const QString &file, Qfw::Theme theme, bool reg)
{
    if (reg) {
        StyleSheetManager::instance().Register(file, widget);
    }

    widget->setStyleSheet(getStyleSheet(file, theme));
}

QString StyleSheetBase::applyThemeColor(const QString &qss)
{
    // 模板替换
    const static QList<ThemeColor> mappings = { ThemeColor(ThemeColor::PRIMARY), ThemeColor(ThemeColor::DARK_1),
                                                ThemeColor(ThemeColor::DARK_2),  ThemeColor(ThemeColor::DARK_3),
                                                ThemeColor(ThemeColor::LIGHT_1), ThemeColor(ThemeColor::LIGHT_2),
                                                ThemeColor(ThemeColor::LIGHT_3) };

    QStringList lines = qss.split("\r\n");
    QString result    = "";

    for (auto line : lines) {
        int start = line.indexOf("--");
        if (start >= 0) {
            int end = -1;
            for (int i = start + 2; i < line.count(); ++i) {
                if (!line.at(i).isLetterOrNumber()) {
                    break;
                }
                end = i;
            }
            if (end > start) {
                QString mask = line.mid(start + 2, end - start - 1);
                bool found   = false;
                for (auto m : mappings) {
                    if (m.type() == mask) {
                        result += line.left(start) + m.name() + line.mid(end + 1) + "\r\n";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    result += line + "\r\n";
                }
            } else {
                result += line + "\r\n";
            }
        } else {
            result += line + "\r\n";
        }
    }

    return result;
}

QString FluentStyleSheet::typeName(FluentStyleSheet::Type t)
{
    switch (t) {
        case MENU:
            return "menu";
        case BUTTON:
            return "button";
        case DIALOG:
            return "dialog";
        case SLIDER:
            return "slider";
        case INFO_BAR:
            return "info_bar";
        case SPIN_BOX:
            return "spin_box";
        case TOOL_TIP:
            return "tool_tip";
        case CHECK_BOX:
            return "check_box";
        case COMBO_BOX:
            return "combo_box";
        case LINE_EDIT:
            return "line_edit";
        case TREE_VIEW:
            return "tree_view";
        case TIME_PICKER:
            return "time_picker";
        case SETTING_CARD:
            return "setting_card";
        case COLOR_DIALOG:
            return "color_dialog";
        case SWITCH_BUTTON:
            return "switch_button";
        case MESSAGE_DIALOG:
            return "message_dialog";
        case STATE_TOOL_TIP:
            return "state_tool_tip";
        case FOLDER_LIST_DIALOG:
            return "folder_list_dialog";
        case SETTING_CARD_GROUP:
            return "setting_card_group";
        case EXPAND_SETTING_CARD:
            return "expand_setting_card";
        case NAVIGATION_INTERFACE:
            return "navigation_interface";
    }

    return "";
}

QString FluentStyleSheet::path(Qfw::Theme theme)
{
    return "";
}

QString FluentStyleSheet::content(const QString &name)
{
    QString fullName;
    if (QFWIns.isDarkTheme()) {
        fullName = QString(":/qfluentwidgets/qss/dark/%2.qss").arg(name.toLower());
    } else {
        fullName = QString(":/qfluentwidgets/qss/light/%2.qss").arg(name.toLower());
    }
    QFile f(fullName);
    QString qss = "";
    if (f.open(QIODevice::ReadOnly)) {
        // 用QTextStream读取样式文件不用区分文件编码 带bom也行
        QTextStream in(&f);
        in.setCodec("utf-8");
        while (!in.atEnd()) {
            QString line = in.readLine();
            qss += line + "\r\n";
        }
        f.close();
    }

    return qss;
}

QString FluentStyleSheet::operator[](FluentStyleSheet::Type t)
{
    return typeName(t);
}

void FluentStyleSheet::apply(FluentStyleSheet::Type t, QWidget *widget)
{
    QString name = typeName(t);
    if (!name.isEmpty()) {
        apply(name, widget);
    }
}

void FluentStyleSheet::apply(const QString &name, QWidget *widget)
{
    QString fullName;
    if (QFWIns.isDarkTheme()) {
        fullName = QString(":/qfluentwidgets/qss/dark/%2.qss").arg(name.toLower());
    } else {
        fullName = QString(":/qfluentwidgets/qss/light/%2.qss").arg(name.toLower());
    }
    QFile f(fullName);
    QString qss = "";
    if (f.open(QIODevice::ReadOnly)) {
        // 用QTextStream读取样式文件不用区分文件编码 带bom也行
        QTextStream in(&f);
        in.setCodec("utf-8");
        while (!in.atEnd()) {
            QString line = in.readLine();
            qss += line + "\r\n";
        }
        f.close();
    }

    widget->setStyleSheet(applyThemeColor(qss));
}

ThemeColor::ThemeColor(ThemeColor::Type t) : m_type(t) { }

QString ThemeColor::type()
{
    switch (m_type) {
        case PRIMARY:
            return "ThemeColorPrimary";
        case DARK_1:
            return "ThemeColorDark1";
        case DARK_2:
            return "ThemeColorDark2";
        case DARK_3:
            return "ThemeColorDark3";
        case LIGHT_1:
            return "ThemeColorLight1";
        case LIGHT_2:
            return "ThemeColorLight2";
        case LIGHT_3:
            return "ThemeColorLight3";
    }

    return "";
}

QString ThemeColor::operator[](ThemeColor::Type t)
{
    switch (t) {
        case PRIMARY:
            return "ThemeColorPrimary";
        case DARK_1:
            return "ThemeColorDark1";
        case DARK_2:
            return "ThemeColorDark2";
        case DARK_3:
            return "ThemeColorDark3";
        case LIGHT_1:
            return "ThemeColorLight1";
        case LIGHT_2:
            return "ThemeColorLight2";
        case LIGHT_3:
            return "ThemeColorLight3";
    }

    return "";
}

QString ThemeColor::name() const
{
    return color().name();
}

QColor ThemeColor::color() const
{
    QColor themeColor = QFWIns.themeColor();

    // transform color into hsv space
    qreal h, s, v;
    themeColor.getHsvF(&h, &s, &v);

    if (QFWIns.isDarkTheme()) {
        s *= 0.84;
        v = 1;
        if (m_type == DARK_1) {
            v *= 0.9;
        } else if (m_type == DARK_2) {
            s *= 0.977;
            v *= 0.82;
        } else if (m_type == DARK_3) {
            s *= 0.95;
            v *= 0.7;
        } else if (m_type == LIGHT_1) {
            s *= 0.92;
        } else if (m_type == LIGHT_2) {
            s *= 0.78;
        } else if (m_type == LIGHT_3) {
            s *= 0.65;
        }
    } else {
        if (m_type == DARK_1) {
            v *= 0.75;
        } else if (m_type == DARK_2) {
            s *= 1.05;
            v *= 0.5;
        } else if (m_type == DARK_3) {
            s *= 1.1;
            v *= 0.4;
        } else if (m_type == LIGHT_1) {
            v *= 1.05;
        } else if (m_type == LIGHT_2) {
            s *= 0.75;
            v *= 1.05;
        } else if (m_type == LIGHT_3) {
            s *= 0.65;
            v *= 1.05;
        }
    }

    return QColor::fromHsvF(h, qMin(s, 1.), qMin(v, 1.));
}

QColor themeColor()
{
    return ThemeColor(ThemeColor::PRIMARY).color();
}

void setThemeColor(const QColor &color, bool save) { }
