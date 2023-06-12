#include "DateTimeInterface.h"

#include "common/Translator.h"

#include <DateTime/DatePicker.h>
#include <DateTime/TimePicker.h>

DateTimeInterface::DateTimeInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->dateTime, "qfluentwidgets.components.date_time", parent)
{
    // date picker
    addExampleCard(tr("A simple DatePicker"), new DatePicker(this),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/date_time/demo.py");

    // date picker
    addExampleCard(tr("A DatePicker in another format"), new ZhDatePicker(this),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/date_time/demo.py");

    // AM/PM time picker
    addExampleCard(tr("A simple TimePicker"), new AMTimePicker(this),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/date_time/demo.py");

    // 24 hours time picker
    addExampleCard(tr("A TimePicker using a 24-hour clock"), new TimePicker(this),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/date_time/demo.py");

    addExampleCard(tr("A TimePicker with seconds column"), new TimePicker(this, true),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/date_time/demo.py");
}
