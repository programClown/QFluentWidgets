#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QVariant>
#include <QDebug>
#include <QFileInfo>
#include <QColor>
#include <QDir>
#include <QSettings>

namespace Qfw
{
    /// Config validator
    class ConfigValidator
    {
    public:
        /// Verify whether the value is legal
        virtual bool validate(const QVariant & /*value*/) { return true; }

        /// correct illegal value
        virtual QVariant correct(const QVariant &value) { return value; }
    };

    class RangeValidator : public ConfigValidator
    {
    public:
        struct Range
        {
            double min;
            double max;
        };

        RangeValidator(double min, double max) : m_min(min), m_max(max)
        {

            m_range.min = min;
            m_range.max = max;
        }

        // ConfigValidator interface
    public:
        bool validate(const QVariant &value) override
        {
            double v = value.toDouble();
            // min <= v <= max
            return (v >= m_min) && (v <= m_max);
        }

        QVariant correct(const QVariant &value) override
        {
            double v   = value.toDouble();
            double ret = qMin(qMax(m_min, v), m_max);
            return QVariant(ret);
        }

        double m_min;
        double m_max;
        Range m_range;
    };

    class OptionsValidator : public ConfigValidator
    {
    public:
        OptionsValidator(const QVariantList &options) : m_options(options)
        {
            if (options.count() == 0) {
                qCritical() << "The `options` can't be empty.";
            }
        }

        // ConfigValidator interface
    public:
        bool validate(const QVariant &value) override { return m_options.contains(value); }

        QVariant correct(const QVariant &value) override
        {
            if (validate(value)) {
                return value;
            }
            if (m_options.count() == 0) {
                return QVariant();
            }
            return m_options.at(0);
        }

        QVariantList m_options;
    };

    class BoolValidator : public OptionsValidator
    {
    public:
        BoolValidator() : OptionsValidator(QVariantList{ true, false }) { }
    };

    class FolderValidator : public ConfigValidator
    {

        // ConfigValidator interface
    public:
        bool validate(const QVariant &value) override
        {
            QString file = value.toString();
            if (!QFileInfo(file).exists()) {
                return false;
            }
            return true;
        }

        QVariant correct(const QVariant &value) override
        {
            QString file = value.toString();
            if (!QFileInfo(file).exists()) {
                QDir().mkdir(file);
            }

            return QFileInfo(file).absolutePath().replace("\\", "/");
        }
    };

    class FolderListValidator : public ConfigValidator
    {
        // ConfigValidator interface
    public:
        bool validate(const QVariant &value) override
        {
            QStringList files = value.toStringList();
            for (auto p : files) {
                if (!QFileInfo(p).exists()) {
                    return false;
                }
            }

            return true;
        }

        QVariant correct(const QVariant &value) override
        {
            QStringList files = value.toStringList();
            QStringList folders;
            for (auto p : files) {
                if (QFileInfo(p).exists()) {
                    folders.append(QFileInfo(p).absolutePath().replace("\\", "/"));
                }
            }

            return folders;
        }
    };

    class ColorValidator : public ConfigValidator
    {
    public:
        ColorValidator(const QColor &c) : m_color(c) { }

        // ConfigValidator interface
    public:
        bool validate(const QVariant &value) override
        {
            if (value.canConvert<QColor>()) {
                return true;
            }
            return false;
        }

        QVariant correct(const QVariant &value) override
        {
            if (validate(value)) {
                return value;
            }

            return QVariant(m_color);
        }

        QColor m_color;
    };

    class ConfigItem : public QObject
    {
        Q_OBJECT
    public:
        ConfigItem(const QString &group, const QString &name, const QVariant &defaultValue,
                   ConfigValidator *validator = nullptr, bool restart = false, QObject *parent = nullptr);

        void setValue(const QVariant &value);
        QVariant value() const;
        QString key();

    signals:
        void valueChanged(const QVariant &);

    public:
        QString group;
        QString name;
        QScopedPointer<ConfigValidator> validator;
        QVariant serializeValue;
        QVariant defaultValue;
        bool restart;

    private:
        QVariant m_value;
    };

    class RangeConfigItem : public ConfigItem
    {
        Q_OBJECT
    public:
        explicit RangeConfigItem(RangeValidator *validator, QObject *parent = nullptr);

        RangeValidator::Range range() const;
    };

    class QConfig : public QObject
    {
        Q_OBJECT
    public:
        explicit QConfig(QObject *parent = nullptr);
    };

}  // namespace Qfw

#endif  // CONFIG_H
