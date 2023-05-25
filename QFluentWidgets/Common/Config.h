#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include "Theme.h"

namespace Qfw
{
    class ConfigItem : public QObject
    {
        Q_OBJECT
    public:
        ConfigItem(const QString &group, const QString &name, const QVariant &defaultValue, QObject *parent = nullptr);

        void setValue(const QVariant &value);
        QVariant value() const;
        QString key();

    signals:
        void valueChanged(const QVariant &);

    public:
        QString group;
        QString name;

    private:
        QVariant m_value;
    };

    class QConfig : public QObject
    {
        Q_OBJECT
    public:
        explicit QConfig(QObject *parent = nullptr);
        explicit QConfig(const QString &customPath, QObject *parent = nullptr);

        QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;

        void set(const QString &key, const QVariant &val, bool save = true);

        void setTheme(Theme t);
        Theme theme() const;
        void setThemeColor(const QColor &c);
        QColor themeColor() const;

    signals:
        void appRestartSig();
        void themeChanged(Theme t);
        void themeColorChanged(const QColor &);

    private:
        void init();

    private:
        QString m_filePath;
        QScopedPointer<QSettings> m_settings;
    };

}  // namespace Qfw

#endif  // CONFIG_H
