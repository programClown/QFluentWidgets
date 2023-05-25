#ifndef QFLUENTWIDGETS_H
#define QFLUENTWIDGETS_H

#include <QObject>
#include "Common/Theme.h"
#include "Common/StyleSheet.h"
#include "Common/Config.h"

/**
 * @brief 库style，theme管理，单例
 */
class QFluentWidgets : public QObject
{
    Q_OBJECT

    Q_DISABLE_COPY(QFluentWidgets)
    QFluentWidgets(QFluentWidgets &&) = delete;
    QFluentWidgets &operator=(QFluentWidgets &&) = delete;

public:
    static QFluentWidgets &ins()
    {
        static QFluentWidgets single;
        return single;
    }

    void setConfigPath(const QString &path);

    /************************** theme start **************************/

    Qfw::Theme theme() const;
    QString themeName() const;
    void setTheme(const Qfw::Theme &theme);
    QColor themeColor() const;
    void setThemeColor(const QColor &themeColor);
    bool isDarkTheme();
    /************************** theme end **************************/

signals:
    void appRestartSig();
    void themeChanged(Qfw::Theme t);
    void themeColorChanged(const QColor &);

private:
    QFluentWidgets();

    Qfw::Theme m_theme;
    QColor m_themeColor;
    QScopedPointer<Qfw::QConfig> m_config;
};

#define QFWIns (QFluentWidgets::ins())

#endif  // QFLUENTWIDGETS_H
