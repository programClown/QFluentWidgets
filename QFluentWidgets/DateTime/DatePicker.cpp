#include "DatePicker.h"
#include "Widgets/CycleListWidget.h"

/**
 * @brief DatePicker::DatePicker
 * @param panel
 * @param parent
 * @param format: the format of date, could be `DatePicker.MM_DD_YYYY` or `DatePicker.YYYY_MM_DD`
 * @param monthTight: is the month column tight
 */
DatePicker::DatePicker(QWidget *parent, DatePicker::Format format, bool monthTight)
    : DatePickerBase(parent), m_isMonthTight(monthTight)
{
    MONTH = tr("month");
    YEAR  = tr("year");
    DAY   = tr("day");
    setDateFormat(format);
}

void DatePicker::setDateFormat(DatePicker::Format format)
{
    this->clearColumns();
    int y = QDate::currentDate().year();

    m_dateFormat = format;

    QVariantList range;
    int w = 80;

    if (format == MM_DD_YYYY) {
        m_monthIndex = 0;
        m_dayIndex   = 1;
        m_yearIndex  = 2;

        range.clear();
        for (int i = 1; i < 13; ++i) {
            range << i;
        }
        addColumn(MONTH, range, w, Qt::AlignLeft, monthFormatter());

        range.clear();
        for (int i = 1; i < 32; ++i) {
            range << i;
        }
        addColumn(DAY, range, w, Qt::AlignLeft, dayFormatter());

        range.clear();
        for (int i = y - 100; i < y + 101; ++i) {
            range << i;
        }
        addColumn(YEAR, range, w, Qt::AlignLeft, yearFormatter());
    } else if (format == YYYY_MM_DD) {
        m_monthIndex = 1;
        m_dayIndex   = 2;
        m_yearIndex  = 0;

        range.clear();
        for (int i = y - 100; i < y + 101; ++i) {
            range << i;
        }
        addColumn(YEAR, range, w, Qt::AlignLeft, yearFormatter());

        range.clear();
        for (int i = 1; i < 13; ++i) {
            range << i;
        }
        addColumn(MONTH, range, w, Qt::AlignLeft, monthFormatter());

        range.clear();
        for (int i = 1; i < 32; ++i) {
            range << i;
        }
        addColumn(DAY, range, w, Qt::AlignLeft, dayFormatter());
    }

    setColumnWidth(m_monthIndex, monthColumnWidth());
}

QStringList DatePicker::panelInitialValue()
{
    bool any = false;
    for (auto v : this->value()) {
        if (!v.isEmpty()) {
            any = true;
        }
    }

    if (any) {
        return this->value();
    }

    QDate date = QDate::currentDate();
    QString y  = this->encodeValue(0, date.year()).toString();
    QString m  = this->encodeValue(1, date.month()).toString();
    QString d  = this->encodeValue(2, date.day()).toString();

    if (m_dateFormat == YYYY_MM_DD) {
        return QStringList{ y, m, d };
    } else {
        return QStringList{ m, d, y };
    }
}

void DatePicker::setMonthTight(bool monthTight)
{
    if (m_isMonthTight == monthTight) {
        return;
    }

    m_isMonthTight = monthTight;
    setColumnWidth(m_monthIndex, monthColumnWidth());
}

void DatePicker::setDate(const QDate &date)
{
    if (!date.isValid() || date.isNull()) {
        return;
    }

    this->date = date;

    setColumnValue(m_monthIndex, QString::number(date.month()));
    setColumnValue(m_dayIndex, QString::number(date.day()));
    setColumnValue(m_yearIndex, QString::number(date.year()));

    QVariantList range;
    for (int i = 1; i < date.daysInMonth() + 1; ++i) {
        range << i;
    }
    setColumnItems(m_dayIndex, range);
}

void DatePicker::onConfirmed(const QStringList &value)
{
    int year  = this->decodeValue(m_yearIndex, value[m_yearIndex]).toInt();
    int month = this->decodeValue(m_monthIndex, value[m_monthIndex]).toInt();
    int day   = this->decodeValue(m_dayIndex, value[m_dayIndex]).toInt();

    QDate date(year, month, day);
    QDate od = this->date;

    setDate(date);
    if (od != date) {
        emit dateChanged(date);
    }
}

void DatePicker::onColumnValueChanged(PickerPanel *panel, int index, const QString & /*value*/)
{
    if (index == m_dayIndex) {
        return;
    }

    // get days number in month
    QVariant m = this->decodeValue(m_monthIndex, panel->columnValue(m_monthIndex));
    int month  = this->decodeValue(m_monthIndex, panel->columnValue(m_monthIndex)).toInt();
    int year   = this->decodeValue(m_yearIndex, panel->columnValue(m_yearIndex)).toInt();
    QDate tmp(year, month, 1);
    int days = tmp.daysInMonth();

    // update days
    CycleListWidget *c = panel->column(m_dayIndex);
    QString day        = c->currentItem()->text();

    QVariantList range;
    for (int i = 1; i < days + 1; ++i) {
        range << i;
    }

    setColumnItems(m_dayIndex, range);

    c->setItems(this->columns[m_dayIndex]->items());
    c->setSelectedItem(day);
}

int DatePicker::monthColumnWidth()
{
    QFontMetrics fm = this->fontMetrics();
    int wm          = -1;
    for (auto i : columns[m_monthIndex]->items()) {
        if (wm < fm.width(i.toString())) {
            wm = fm.width(i.toString());
        }
    }
    wm += 20;

    // don't use tight layout for english
    if (MONTH == "month") {
        return wm + 49;
    }

    if (m_isMonthTight) {
        return qMax(80, wm);
    } else {
        return wm + 49;
    }
}
