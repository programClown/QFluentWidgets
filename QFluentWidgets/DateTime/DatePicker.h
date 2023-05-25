#ifndef DATEPICKER_H
#define DATEPICKER_H

#include "PickerBase.h"

#include <QDate>

#if (QT_VERSION > QT_VERSION_CHECK(5, 14, 0))
    #include <QCalendar>
#endif

class MonthFormatter : public PickerColumnFormatter
{
    Q_OBJECT
public:
    MonthFormatter(QObject *parent = nullptr) : PickerColumnFormatter(parent)
    {
        m_months = QStringList{ tr("January"),   tr("February"), tr("March"),    tr("April"),
                                tr("May"),       tr("June"),     tr("July"),     tr("August"),
                                tr("September"), tr("October"),  tr("November"), tr("December") };
    }

private:
    QStringList m_months;

    // PickerColumnFormatter interface
public:
    QString encode(const QVariant &month) override { return m_months[month.toInt() - 1]; }

    QString decode(const QVariant &value) override { return QString::number(m_months.indexOf(value.toString()) + 1); }
};

class DatePickerBase : public PickerBase
{
    Q_OBJECT
public:
    explicit DatePickerBase(QWidget *parent = nullptr) : PickerBase(parent)
    {
        date = QDate();

#if (QT_VERSION > QT_VERSION_CHECK(5, 14, 0))
        calendar = QCalendar();
#endif

        m_yearFormatter  = nullptr;
        m_monthFormatter = nullptr;
        m_dayFormatter   = nullptr;
    }

    virtual void setDate(const QDate &date) = 0;

    void setYearFormatter(PickerColumnFormatter *formatter) { m_yearFormatter = formatter; }

    void setMonthFormatter(PickerColumnFormatter *formatter) { m_monthFormatter = formatter; }

    void setDayFormatter(PickerColumnFormatter *formatter) { m_dayFormatter = formatter; }

    PickerColumnFormatter *yearFormatter()
    {
        if (!m_yearFormatter) {
            return new DigitFormatter(this);
        }

        return m_yearFormatter;
    }

    PickerColumnFormatter *monthFormatter()
    {
        if (!m_monthFormatter) {
            return new MonthFormatter(this);
        }

        return m_monthFormatter;
    }

    PickerColumnFormatter *dayFormatter()
    {
        if (!m_dayFormatter) {
            return new DigitFormatter(this);
        }

        return m_dayFormatter;
    }

    QDate date;

#if (QT_VERSION > QT_VERSION_CHECK(5, 14, 0))
    QCalendar calendar;
#endif

signals:
    void dateChanged(const QDate &);

private:
    PickerColumnFormatter *m_yearFormatter;
    PickerColumnFormatter *m_monthFormatter;
    PickerColumnFormatter *m_dayFormatter;
};

class DatePicker : public DatePickerBase
{
    Q_OBJECT
public:
    enum Format
    {
        MM_DD_YYYY = 0,
        YYYY_MM_DD = 1
    };

    explicit DatePicker(QWidget *parent = nullptr, Format format = MM_DD_YYYY, bool monthTight = true);

    void setDateFormat(Format format);
    QStringList panelInitialValue();
    void setMonthTight(bool monthTight);

    void setDate(const QDate &date) override;

    QString YEAR;
    QString MONTH;
    QString DAY;

public slots:
    void onConfirmed(const QStringList &value) override;
    void onColumnValueChanged(PickerPanel *panel, int index, const QString &value) override;

private:
    int monthColumnWidth();

private:
    bool m_isMonthTight;
    Format m_dateFormat;

    int m_monthIndex;
    int m_dayIndex;
    int m_yearIndex;
};

/**
 * @brief Chinese date formatter
 */
class ZhFormatter : public PickerColumnFormatter
{
    Q_OBJECT
public:
    ZhFormatter(QObject *parent = nullptr) : PickerColumnFormatter(parent), suffix("") { }

    QString suffix;

    // PickerColumnFormatter interface
public:
    QString encode(const QVariant &value) override { return value.toString() + suffix; }

    QString decode(const QVariant &value) override
    {
        QString date = value.toString();
        if (date.isEmpty()) {
            return "0";
        } else {
            return date.left(date.count() - suffix.count());
        }
    }
};

class ZhYearFormatter : public ZhFormatter
{
    Q_OBJECT
public:
    ZhYearFormatter(QObject *parent = nullptr) : ZhFormatter(parent) { suffix = "年"; }
};

class ZhMonthFormatter : public ZhFormatter
{
    Q_OBJECT
public:
    ZhMonthFormatter(QObject *parent = nullptr) : ZhFormatter(parent) { suffix = "月"; }
};

class ZhDayFormatter : public ZhFormatter
{
    Q_OBJECT
public:
    ZhDayFormatter(QObject *parent = nullptr) : ZhFormatter(parent) { suffix = "日"; }
};

class ZhDatePicker : public DatePicker
{
    Q_OBJECT
public:
    explicit ZhDatePicker(QWidget *parent = nullptr) : DatePicker(parent)
    {
        MONTH = "月";
        YEAR  = "年";
        DAY   = "日";
        setDayFormatter(new ZhDayFormatter(this));
        setYearFormatter(new ZhYearFormatter(this));
        setMonthFormatter(new ZhMonthFormatter(this));
        setDateFormat(YYYY_MM_DD);
    }
};
#endif  // DATEPICKER_H
