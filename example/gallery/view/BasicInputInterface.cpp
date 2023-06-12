#include "BasicInputInterface.h"

#include "common/Translator.h"

#include <Widgets/Button.h>
#include <Common/Icon.h>
#include <Widgets/CheckBox.h>
#include <Widgets/ComboBox.h>
#include <Widgets/Slider.h>
#include <Widgets/SwitchButton.h>

#include <QBoxLayout>
#include <QButtonGroup>

BasicInputInterface::BasicInputInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->basicInput, "qfluentwidgets.components.widgets", parent)
{
    // simple push button
    addExampleCard(tr("A simple button with text content"), new PushButton(tr("Standard push button")),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/button/demo.py");

    // tool button
    ToolButton *button = new ToolButton(new FluentIcon(":/resource/images/kunkun.png"));
    button->setIconSize(QSize(40, 40));
    button->resize(70, 70);
    addExampleCard(tr("A button with graphical content"), button,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/button/demo.py");

    // primary color button
    addExampleCard(tr("Accent style applied to button"), new PrimaryPushButton(tr("Accent style button")),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/button/demo.py");

    // hyperlink button
    addExampleCard(tr("A hyperlink button that navigates to a URI"),
                   new HyperlinkButton("http://github.com", tr("GitHub home page")),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/button/demo.py");

    // 2-state check box
    addExampleCard(tr("A 2-state CheckBox"), new CheckBox(tr("Two-state CheckBox")),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/check_box/demo.py");

    // 3-state check box
    CheckBox *checkBox = new CheckBox(tr("Three-state CheckBox"));
    checkBox->setTristate(true);
    addExampleCard(tr("A 3-state CheckBox"), checkBox,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/check_box/demo.py");

    // combo box
    ComboBox *comboBox = new ComboBox();
    comboBox->addItems({ "shoko 🥰", "西宫硝子 😊", "一级棒卡哇伊的硝子酱 😘" });
    comboBox->setCurrentIndex(0);
    comboBox->setMinimumWidth(210);
    addExampleCard(tr("A ComboBox with items"), comboBox,
                   "ht   tps://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/combo_box/demo.py");

    // editable combo box
    // No such class "EditableComboBox"
    // radio button
    QWidget *radioWidget     = new QWidget();
    QVBoxLayout *radioLayout = new QVBoxLayout(radioWidget);
    radioLayout->setContentsMargins(2, 0, 0, 0);
    radioLayout->setSpacing(15);

    RadioButton *radioButton1 = new RadioButton(tr("Star Platinum"), radioWidget);
    RadioButton *radioButton2 = new RadioButton(tr("Crazy Diamond"), radioWidget);
    RadioButton *radioButton3 = new RadioButton(tr("Soft and Wet"), radioWidget);

    QButtonGroup *buttonGroup = new QButtonGroup(radioWidget);
    buttonGroup->addButton(radioButton1);
    buttonGroup->addButton(radioButton2);
    buttonGroup->addButton(radioButton3);
    radioLayout->addWidget(radioButton1);
    radioLayout->addWidget(radioButton2);
    radioLayout->addWidget(radioButton3);
    radioButton1->click();
    QString groupButtonStr = "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/radio_button/demo.py";
    addExampleCard(tr("A group of RadioButton controls in a button group"), radioWidget, groupButtonStr);

    // horizontal slider
    Slider *slider = new Slider(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(30);
    slider->setMinimumWidth(200);
    addExampleCard(tr("A simple horizontal slider"), slider,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/slider/demo.py");

    // switch button
    m_switchButton = new SwitchButton(tr("Off"));
    connect(m_switchButton, &SwitchButton::checkedChanged, this, &BasicInputInterface::onSwitchCheckedChanged);
    addExampleCard(tr("A simple switch button"), m_switchButton,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/switch_button/demo.py");
}

void BasicInputInterface::onSwitchCheckedChanged(bool isChecked)
{
    if (isChecked) {
        m_switchButton->setText(tr("On"));
    } else {
        m_switchButton->setText(tr("Off"));
    }
}
