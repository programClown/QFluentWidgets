#include "TimePicker.h"

TimePicker::TimePicker(QWidget *parent, bool showSeconds) : TimePickerBase(parent, showSeconds)
{
    // add hour column
    int w = showSeconds ? 80 : 120;

    QVariantList range;
    for (int i = 0; i < 24; ++i) {
        range << i;
    }
    addColumn(tr("hour"), range, w, Qt::AlignLeft, new DigitFormatter(this));

    range.clear();
    for (int i = 0; i < 60; ++i) {
        range << i;
    }
    // add minute column;
    addColumn(tr("minute"), range, w, Qt::AlignLeft, new MiniuteFormatter(this));

    // add seconds column;
    addColumn(tr("second"), range, w, Qt::AlignLeft, new MiniuteFormatter(this));
    setColumnVisible(2, showSeconds);
}

void TimePicker::setTime(const QTime &time)
{
    if (time.isNull() || !time.isValid()) {
        return;
    }

    this->time = time;

    setColumnValue(0, QString::number(time.hour()));
    setColumnValue(1, QString::number(time.minute()));
    setColumnValue(2, QString::number(time.second()));
}

void TimePicker::setSecondVisible(bool visible)
{
    setColumnVisible(2, visible);
    int w = visible ? 80 : 120;
    for (auto button : columns) {
        button->setFixedWidth(w);
    }
}

QStringList TimePicker::panelInitialValue()
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

    QTime time = QTime::currentTime();
    QString h  = this->encodeValue(0, time.hour()).toString();
    QString m  = this->encodeValue(1, time.minute()).toString();
    QString s  = this->encodeValue(2, time.second()).toString();

    if (showSeconds) {
        return QStringList{ h, m, s };
    } else {
        return QStringList{ h, m };
    }
}

void TimePicker::onConfirmed(const QStringList &value)
{
    TimePickerBase::onConfirmed(value);

    int h = this->decodeValue(0, value[0]).toInt();
    int m = this->decodeValue(1, value[1]).toInt();
    int s = 0;
    if (value.count() > 2) {
        s = this->decodeValue(2, value[2]).toInt();
    }

    QTime time(h, m, s);
    QTime ot = this->time;
    setTime(time);

    if (ot != time) {
        emit timeChanged(time);
    }
}

AMTimePicker::AMTimePicker(QWidget *parent, bool showSeconds) : TimePickerBase(parent, showSeconds)
{
    AM = tr("AM");
    PM = tr("PM");

    QVariantList range;
    for (int i = 1; i < 13; ++i) {
        range << i;
    }

    int w = 80;
    addColumn(tr("hour"), range, w, Qt::AlignLeft, new AMHourFormatter(this));

    range.clear();
    for (int i = 0; i < 60; ++i) {
        range << i;
    }
    // add minute column;
    addColumn(tr("minute"), range, w, Qt::AlignLeft, new MiniuteFormatter(this));

    // add seconds column;
    addColumn(tr("second"), range, w, Qt::AlignLeft, new MiniuteFormatter(this));
    setColumnVisible(2, showSeconds);

    // add AM/PM column
    range.clear();
    range << AM << PM;
    addColumn(AM, range, w, Qt::AlignLeft, new AMPMFormatter(this));
}

void AMTimePicker::setTime(const QTime &time)
{
    if (time.isNull() || !time.isValid()) {
        return;
    }

    this->time = time;

    setColumnValue(0, QString::number(time.hour()));
    setColumnValue(1, QString::number(time.minute()));
    setColumnValue(2, QString::number(time.second()));
    setColumnValue(3, QString::number(time.hour()));
}

void AMTimePicker::setSecondVisible(bool visible)
{
    setColumnVisible(2, visible);
}

QStringList AMTimePicker::panelInitialValue()
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

    QTime time = QTime::currentTime();
    QString h  = this->encodeValue(0, time.hour()).toString();
    QString m  = this->encodeValue(1, time.minute()).toString();
    QString s  = this->encodeValue(2, time.second()).toString();
    QString p  = this->encodeValue(3, time.hour()).toString();

    if (showSeconds) {
        return QStringList{ h, m, s, p };
    } else {
        return QStringList{ h, m, p };
    }
}

void AMTimePicker::onConfirmed(const QStringList &value)
{
    TimePickerBase::onConfirmed(value);

    int h, m, s;
    QString p;
    h = this->decodeValue(0, value[0]).toInt();
    m = this->decodeValue(1, value[1]).toInt();

    if (value.count() == 3) {
        p = this->decodeValue(3, value[2]).toString();
        s = 0;
    } else {
        s = this->decodeValue(2, value[2]).toInt();
        p = this->decodeValue(3, value[3]).toString();
    }

    if (p == AM) {
        h = (h == 12) ? 0 : h;
    } else {
        h = (h == 12) ? h : h + 12;
    }

    QTime time(h, m, s);
    QTime ot = this->time;
    setTime(time);

    if (ot != time) {
        emit timeChanged(time);
    }
}
