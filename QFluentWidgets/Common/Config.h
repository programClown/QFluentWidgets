#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include "Theme.h"

#include <QDebug>
#include <iostream>
#include <QMetaEnum>
#include <type_traits>

namespace Qfw
{
    class ConfigValidator
    {
    public:
        virtual bool validate(int) { return true; }
        virtual int correct(int value) { return value; }
    };

    class RangeValidator : public ConfigValidator
    {
    public:
        RangeValidator(int min, int max) : m_min(min), m_max(max) { }

        bool validate(int value) override { return value >= m_min && value <= m_max; };

        int correct(int value) override { return std::min(std::max(m_min, value), m_max); }

    private:
        int m_min, m_max;
    };

    template<typename T>

    class OptionsValidator : public ConfigValidator
    {
    public:
        OptionsValidator(QList<T> options)
        {
            qDebug() << __FUNCTION__ << __LINE__;
            if (options.isEmpty()) {
                qDebug() << "The 'options' can't be empty" << endl;
            }

            qDebug() << __FUNCTION__ << __LINE__;

            auto value = std::is_same<int, typename std::decay<T>::type>::value;
            qDebug() << __FUNCTION__ << __LINE__;
            //            qDebug() << "2" << metaEnum.isValid() << metaEnum.name() << metaEnum.enumName() <<
            //            metaEnum.keyCount();
        }

    private:
        QList<T> m_options;
    };

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

    class RangeConfigItem : public ConfigItem
    {
    public:
        void range() { }
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
