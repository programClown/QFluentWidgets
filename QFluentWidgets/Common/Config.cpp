#include "Config.h"

#include <QColor>
#include <QCoreApplication>
#include <QTextCodec>

using namespace Qfw;

ConfigItem::ConfigItem(const QString &_group, const QString &_name, const QVariant &_defaultValue, QObject *parent)
    : QObject(parent), group(_group), name(_name), m_value(_defaultValue)
{
}

void ConfigItem::setValue(const QVariant &value)
{
    m_value = value;
}

QVariant ConfigItem::value() const
{
    return m_value;
}

/// get the config key separated by `.`
QString ConfigItem::key()
{
    if (name.isEmpty()) {
        return group;
    } else {
        return group + "." + name;
    }
}

QConfig::QConfig(QObject *parent)
    : QObject(parent), m_filePath(QCoreApplication::applicationDirPath() + "/config/config.ini")
{
    init();
}

QConfig::QConfig(const QString &customPath, QObject *parent) : QObject(parent), m_filePath(customPath)
{
    init();
}

void QConfig::init()
{
    m_settings.reset(new QSettings(m_filePath, QSettings::IniFormat));
    m_settings->setIniCodec(QTextCodec::codecForName("utf-8"));
}

QVariant QConfig::get(const QString &key, const QVariant &defaultValue) const
{
    return m_settings->value(key, defaultValue);
}

void QConfig::set(const QString &key, const QVariant &val, bool save)
{
    if (m_settings->value(key) == val) {
        return;
    }

    if (save) {
        m_settings->setValue(key, val);
    }

    if (key.toLower() == "qfluentwidgets/restart") {
        emit appRestartSig();
    }

    if (key.toLower() == "qfluentwidgets/thememode") {
        emit themeChanged((Theme)val.toInt());
    }

    if (key.toLower() == "qfluentwidgets/themecolor") {
        emit themeColorChanged(val.value<QColor>());
    }
}

void QConfig::setTheme(Theme t)
{
    set("QFluentWidgets/ThemeMode", int(t));
}

Theme QConfig::theme() const
{
    return (Theme)get("QFluentWidgets/ThemeMode", 0).toInt();
}

void QConfig::setThemeColor(const QColor &c)
{
    set("QFluentWidgets/ThemeColor", c);
}

QColor QConfig::themeColor() const
{
    return get("QFluentWidgets/ThemeColor", QColor("#009faa")).value<QColor>();
}
