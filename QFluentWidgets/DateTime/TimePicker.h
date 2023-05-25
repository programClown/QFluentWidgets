#ifndef TIMEPICKER_H
#define TIMEPICKER_H

#include "PickerBase.h"

#include <QTime>

class TimePickerBase : public PickerBase
{
    Q_OBJECT
public:
    explicit TimePickerBase(QWidget *parent = nullptr, bool showSeconds = false) : PickerBase(parent)
    {
        this->showSeconds = showSeconds;
    }

    bool showSeconds;
    QTime time;

    virtual void setTime(const QTime &time)     = 0;
    virtual void setSecondVisible(bool visible) = 0;

signals:
    void timeChanged(const QTime &);
};

class MiniuteFormatter : public DigitFormatter
{
    Q_OBJECT
public:
    explicit MiniuteFormatter(QObject *parent = nullptr) : DigitFormatter(parent) { }

    QString encode(const QVariant &minute) override
    {
        QString min = minute.toString();
        min         = min.rightJustified(2, '0');
        return min;
    }
};

class AMHourFormatter : public DigitFormatter
{
    Q_OBJECT
public:
    explicit AMHourFormatter(QObject *parent = nullptr) : DigitFormatter(parent) { }

    QString encode(const QVariant &hour) override
    {
        int h = hour.toInt();

        if (h == 0 || h == 12) {
            return QString::number(12);
        }

        return QString::number(h % 12);
    }
};

class AMPMFormatter : public PickerColumnFormatter
{
    Q_OBJECT
public:
    explicit AMPMFormatter(QObject *parent = nullptr) : PickerColumnFormatter(parent)
    {
        AM = tr("AM");
        PM = tr("PM");
    }

    QString encode(const QVariant &hour) override
    {
        bool ok;
        int h = hour.toInt(&ok);
        if (!ok) {
            return hour.toString();
        }

        if (h < 12) {
            return AM;
        }

        return PM;
    }

    QString AM;
    QString PM;
};

/**
 * @brief 24 hours time picker
 */
class TimePicker : public TimePickerBase
{
    Q_OBJECT
public:
    explicit TimePicker(QWidget *parent = nullptr, bool showSeconds = false);

    void setTime(const QTime &time) override;
    void setSecondVisible(bool visible) override;
    QStringList panelInitialValue();

private slots:
    void onConfirmed(const QStringList &value) override;
};

/**
 * @brief AM/PM time picker
 */
class AMTimePicker : public TimePickerBase
{
    Q_OBJECT
public:
    explicit AMTimePicker(QWidget *parent = nullptr, bool showSeconds = false);

    void setTime(const QTime &time) override;
    void setSecondVisible(bool visible) override;
    QStringList panelInitialValue();

    QString AM;
    QString PM;

private slots:
    void onConfirmed(const QStringList &value) override;
};

#endif  // TIMEPICKER_H
