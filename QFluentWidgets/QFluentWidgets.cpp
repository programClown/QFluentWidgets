#include "QFluentWidgets.h"

#include <QApplication>
#include <QTextCodec>

QFluentWidgets::QFluentWidgets()
{
    m_config.reset(new Qfw::QConfig(this));
    m_theme      = m_config->theme();
    m_themeColor = m_config->themeColor();
}

void QFluentWidgets::setConfigPath(const QString &path)
{
    m_config.reset(new Qfw::QConfig(path, this));
    m_theme      = m_config->theme();
    m_themeColor = m_config->themeColor();
}

Qfw::Theme QFluentWidgets::theme() const
{
    return m_theme;
}

QString QFluentWidgets::themeName() const
{
    return Qfw::ThemeString(m_theme);
}

void QFluentWidgets::setTheme(const Qfw::Theme &theme)
{
    m_theme = theme;
}

QColor QFluentWidgets::themeColor() const
{
    return m_themeColor;
}

void QFluentWidgets::setThemeColor(const QColor &themeColor)
{
    m_themeColor = themeColor;
}

bool QFluentWidgets::isDarkTheme()
{
    return theme() == Qfw::DARK;
}
