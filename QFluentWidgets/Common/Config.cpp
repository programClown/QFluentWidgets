#include "Config.h"

namespace Qfw
{
    ConfigItem::ConfigItem(const QString &_group, const QString &_name, const QVariant &_defaultValue,
                           ConfigValidator *_validator, bool _restart, QObject *parent)
        : QObject(parent),
          group(_group),
          name(_name),
          validator(_validator),
          serializeValue(_defaultValue),
          restart(_restart),
          m_value(_defaultValue)
    {
        if (!validator) {
            validator.reset(new ConfigValidator());
        }

        defaultValue = validator->correct(_defaultValue);
    }

    void ConfigItem::setValue(const QVariant &value)
    {
        QVariant v  = validator->correct(value);
        QVariant ov = m_value;
        m_value     = v;
        if (ov != v) {
            emit valueChanged(v);
        }
    }

    QVariant ConfigItem::value() const { return m_value; }

    /// get the config key separated by `.`
    QString ConfigItem::key()
    {
        if (name.isEmpty()) {
            return group;
        } else {
            return group + "." + name;
        }
    }

    QConfig::QConfig(QObject *parent) : QObject(parent) { }

    RangeConfigItem::RangeConfigItem(RangeValidator *validator, QObject *parent)
        : ConfigItem("_group", "_name", "", validator, false, parent)
    {
    }

    RangeValidator::Range RangeConfigItem::range() const
    {
        return static_cast<RangeValidator *>(validator.data())->m_range;
    }

}  // namespace Qfw
