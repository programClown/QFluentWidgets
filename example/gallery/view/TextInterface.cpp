#include "TextInterface.h"

#include "common/Translator.h"

#include <Widgets/LineEdit.h>
#include <Widgets/SpinBox.h>

TextInterface::TextInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->text, "qfluentwidgets.components.widgets", parent)
{
    // line edit
    LineEdit *lineEdit = new PasswordLineEdit(this);
    lineEdit->setText("ko no dio da！");
    addExampleCard("A LineEdit with a clear button", lineEdit,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/line_edit/demo.py");

    // spin box
    addExampleCard("A SpinBox with a spin button", new SpinBox(this),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/spin_box/demo.py");

    // double spin box
    addExampleCard("A DoubleSpinBox with a spin button", new DoubleSpinBox(this),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/spin_box/demo.py");

    // date edit
    addExampleCard("A DateEdit with a spin button", new DateEdit(this),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/spin_box/demo.py");

    // time edit
    addExampleCard("A TimeEdit with a spin button", new TimeEdit(this),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/spin_box/demo.py");

    // date time edit
    addExampleCard("A DateTimeEdit with a spin button", new DateTimeEdit(this),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/spin_box/demo.py");

    // text edit
    // !!!不支持 markdown
    TextEdit *textEdit = new TextEdit(this);
    textEdit->setText("## Steel Ball Run \n * Johnny Joestar 🦄 \n * Gyro Zeppeli 🐴 ");
    textEdit->setFixedHeight(150);
    addExampleCard("A simple TextEdit", textEdit,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/line_edit/demo.py", 1);
}
