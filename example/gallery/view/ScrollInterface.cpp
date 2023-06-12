#include "ScrollInterface.h"

#include "common/Translator.h"

#include <Widgets/Label.h>
#include <Widgets/ToolTip.h>

ScrollInterface::ScrollInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->scroll, "qfluentwidgets.components.widgets", parent)
{
    ScrollArea *w      = new ScrollArea();
    PixmapLabel *label = new PixmapLabel(this);
    label->setPixmap(QPixmap(":/resource/images/chidanta2.jpg")
                             .scaled(775, 1229, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    w->horizontalScrollBar()->setValue(0);
    w->setWidget(label);
    w->setFixedSize(780, 430);
    w->setObjectName("imageViewer");

    ExampleCard *card =
            addExampleCard(tr("Smooth scroll area"), w,
                           "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/scroll_area/demo.py");
    card->card()->installEventFilter(new ToolTipFilter(card->card(), 500));
    card->card()->setToolTip(tr("Chitanda Eru is too hot🥵"));
    card->card()->setToolTipDuration(2000);

    SmoothScrollArea *w1 = new SmoothScrollArea();
    PixmapLabel *label1  = new PixmapLabel(this);
    label1->setPixmap(QPixmap(":/resource/images/chidanta3.jpg"));

    w1->setWidget(label1);
    w1->setFixedSize(659, 540);
    w1->setObjectName("imageViewer");

    ExampleCard *card1 =
            addExampleCard(tr("Smooth scroll area implemented by animation"), w1,
                           "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/scroll_area/demo.py");

    card1->card()->installEventFilter(new ToolTipFilter(card1->card(), 500));
    card1->card()->setToolTip(tr("Chitanda Eru is so hot 🥵🥵"));
    card1->card()->setToolTipDuration(2000);
}
