#include "CheckBox.h"
#include "Common/StyleSheet.h"

CheckBox::CheckBox(const QString &text, QWidget *parent) : QCheckBox(text, parent)
{
    FluentStyleSheet::apply("CHECK_BOX", this);
}
