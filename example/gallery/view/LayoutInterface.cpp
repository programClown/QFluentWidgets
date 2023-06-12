#include "LayoutInterface.h"

#include "common/Translator.h"

#include <Layout/FlowLayout.h>
#include <Widgets/Button.h>

LayoutInterface::LayoutInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->layout, "qfluentwidgets.components.layout", parent)
{
    addExampleCard(tr("Flow layout without animation"), createWidget(),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/flow_layout/demo.py", 1);
    addExampleCard(tr("Flow layout with animation"), createWidget(true),
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/flow_layout/demo.py", 1);
}

QWidget *LayoutInterface::createWidget(bool animation)
{
    QList<QString> texts = {
        tr("Star Platinum"),   tr("Hierophant Green"),
        tr("Silver Chariot"),  tr("Crazy diamond"),
        tr("Heaven's Door"),   tr("Killer Queen"),
        tr("Gold Experience"), tr("Sticky Fingers"),
        tr("Sex Pistols"),     tr("Dirty Deeds Done Dirt Cheap"),
    };

    QWidget *widget    = new QWidget();
    FlowLayout *layout = new FlowLayout(widget, animation);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setVerticalSpacing(20);
    layout->setHorizontalSpacing(10);

    for (QString text : texts) {
        layout->addWidget(new PushButton(text));
    }

    return widget;
}
