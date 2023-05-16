#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QHash>
#include <QWidget>
#include <QVariant>
#include "Theme.h"

/**
 * @brief 样式单例
 */
class StyleSheetManager : public QObject
{
    Q_OBJECT

    Q_DISABLE_COPY(StyleSheetManager);
    // Q_DISABLE_MOVE
    StyleSheetManager(StyleSheetManager &&)            = delete;
    StyleSheetManager &operator=(StyleSheetManager &&) = delete;

public:
    static StyleSheetManager &instance()
    {
        static StyleSheetManager ins;
        return ins;
    }

    void Register(const QString &file, QWidget *widget);
    void DeRegister(QWidget *widget);

    QHash<QWidget *, QString> widgets() const;

private:
    StyleSheetManager();

    QHash<QWidget *, QString> m_widgets;
};

/// Style sheet base class
class StyleSheetBase
{
public:
    virtual QString path(Qfw::Theme theme = Qfw::Theme::AUTO) = 0;
    virtual QString content(Qfw::Theme theme = Qfw::Theme::AUTO) { return getStyleSheet(path(theme), theme); }

    static QString getStyleSheet(const QString &file, Qfw::Theme theme = Qfw::Theme::AUTO);
    static void setStyleSheet(QWidget *widget, const QString &file, Qfw::Theme theme = Qfw::Theme::AUTO,
                              bool reg = true);

    static QString applyThemeColor(const QString &qss);
};

class FluentStyleSheet : public StyleSheetBase
{
public:
    enum Type
    {
        MENU = 0,
        BUTTON,
        DIALOG,
        SLIDER,
        INFO_BAR,
        SPIN_BOX,
        TOOL_TIP,
        CHECK_BOX,
        COMBO_BOX,
        LINE_EDIT,
        TREE_VIEW,
        TIME_PICKER,
        SETTING_CARD,
        COLOR_DIALOG,
        SWITCH_BUTTON,
        MESSAGE_DIALOG,
        STATE_TOOL_TIP,
        FOLDER_LIST_DIALOG,
        SETTING_CARD_GROUP,
        EXPAND_SETTING_CARD,
        NAVIGATION_INTERFACE,
    };

    static QString typeName(Type t);

    QString path(Qfw::Theme theme) override;
    static QString content(const QString &name);
    QString operator[](Type t);

    static void apply(Type t, QWidget *widget);

    static void apply(const QString &name, QWidget *widget);
};

class ThemeColor
{
public:
    enum Type
    {
        PRIMARY = 0,
        DARK_1,
        DARK_2,
        DARK_3,
        LIGHT_1,
        LIGHT_2,
        LIGHT_3
    };
    ThemeColor(Type t);
    QString type();
    QString operator[](Type t);

    QString name() const;
    QColor color() const;

private:
    Type m_type;
};

QColor themeColor();

void setThemeColor(const QColor &color, bool save = false);

#endif  // STYLESHEET_H
