#include "QFluentWidgets.h"

#include <QApplication>
#include <QTextCodec>

QFluentWidgets::QFluentWidgets()
{
    m_settings.reset(new QSettings(QCoreApplication::applicationDirPath() + "/qfluentwidgets/qfluentwidgets.ini",
                                   QSettings::IniFormat));
    m_settings->setIniCodec(QTextCodec::codecForName("utf-8"));

    m_theme      = Qfw::AUTO;
    m_themeColor = QColor("#009faa");
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
