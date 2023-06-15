#include "StatusInfoInterface.h"

#include "common/Translator.h"

#include <Widgets/Button.h>
#include <Widgets/ToolTip.h>
#include <Widgets/Label.h>
#include <Widgets/InfoBar.h>
#include <Widgets/StateToolTip.h>

StatusInfoInterface::StatusInfoInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->statusInfo, "qfluentwidgets.components.widgets", parent)
{
    PushButton *button = new PushButton(tr("Show StateToolTip"));
    connect(button, &PushButton::clicked, this, &StatusInfoInterface::onStateButtonClicked);
    addExampleCard(tr("State tool tip"), button,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/status_tool_tip/demo.py");

    button = new PushButton(tr("Button with a simple ToolTip"));
    button->installEventFilter(new ToolTipFilter(button));
    button->setToolTip(tr("Simple ToolTip"));
    addExampleCard(tr("State tool tip"), button,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/tool_tip/demo.py");

    PixmapLabel *label = new PixmapLabel();
    label->setPixmap(
            QPixmap(":/resource/images/kunkun.png").scaled(160, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    label->installEventFilter(new ToolTipFilter(label, 500));
    label->setToolTip("Label with a ToolTip");
    label->setToolTipDuration(1000);
    label->setFixedSize(160, 160);
    addExampleCard("A label with a ToolTip", label,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/tool_tip/demo.py");

    // short info bar
    InfoBar *infoBar =
            new InfoBar(NEWFLICON(InfoBarIcon, SUCCESS), "Success", "The Anthem of man is the Anthem of courage.",
                        Qt::Horizontal, true, -1, InfoBarPosition::NONE, this);
    addExampleCard("A closable InfoBar", infoBar,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/info_bar/demo.py");

    // long info bar
    QString content =
            "My name is kira yoshikake, 33 years old. Living in the villa area northeast of duwangting, unmarried. I "
            "work in Guiyou chain store. Every day I have to work overtime until 8 p.m. to go home. I don't smoke. The "
            "wine is only for a taste. Sleep at 11 p.m. for 8 hours a day. Before I go to bed, I must drink a cup of "
            "warm milk, then do 20 minutes of soft exercise, get on the bed, and immediately fall asleep. Never leave "
            "fatigue and stress until the next day. Doctors say I'm normal.";
    infoBar = new InfoBar(NEWFLICON(InfoBarIcon, WARNING), "Warning", content, Qt::Vertical, true, 1,
                          InfoBarPosition::NONE, this);
    addExampleCard("A closable InfoBar with long message", infoBar,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/info_bar/demo.py");

    infoBar = new InfoBar(NEWFLICON(FluentIcon, GITHUB), "Github",
                          "When you look long into an abyss, the abyss looks into you.", Qt::Horizontal, true, -1,
                          InfoBarPosition::NONE, this);
    infoBar->addWidget(new PushButton("Action"));
    infoBar->setCustomBackgroundColor("white", "#2a2a2a");
    addExampleCard("An InfoBar with custom icon, background color and widget.", infoBar,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/info_bar/demo.py");

    // different type info bar
    QWidget *w              = new QWidget(this);
    QHBoxLayout *hBoxLayout = new QHBoxLayout(w);
    PushButton *button1     = new PushButton("Top right", w);
    PushButton *button2     = new PushButton("Top", w);
    PushButton *button3     = new PushButton("Top left", w);
    PushButton *button4     = new PushButton("Bottom right", w);
    PushButton *button5     = new PushButton("Bottom", w);
    PushButton *button6     = new PushButton("Bottom left", w);

    connect(button1, &PushButton::clicked, this, &StatusInfoInterface::createTopRightInfoBar);
    connect(button2, &PushButton::clicked, this, &StatusInfoInterface::createTopInfoBar);
    connect(button3, &PushButton::clicked, this, &StatusInfoInterface::createTopLeftInfoBar);
    connect(button4, &PushButton::clicked, this, &StatusInfoInterface::createBottomRightInfoBar);
    connect(button5, &PushButton::clicked, this, &StatusInfoInterface::createBottomInfoBar);
    connect(button6, &PushButton::clicked, this, &StatusInfoInterface::createBottomLeftInfoBar);

    hBoxLayout->addWidget(button1);
    hBoxLayout->addWidget(button2);
    hBoxLayout->addWidget(button3);
    hBoxLayout->addWidget(button4);
    hBoxLayout->addWidget(button5);
    hBoxLayout->addWidget(button6);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(15);
    addExampleCard("InfoBar with different pop-up locations", w,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/info_bar/demo.py");
}

void StatusInfoInterface::onStateButtonClicked()
{
    PushButton *button = (PushButton *)sender();
    if (m_stateTooltip) {
        m_stateTooltip->setContent("The model training is complete! 😆 ");
        button->setText("Show StateToolTip");
        m_stateTooltip->setState(true);
        m_stateTooltip = nullptr;
    } else {
        m_stateTooltip = new StateToolTip("Training model", "Please wait patiently", window());
        button->setText("Hide StateToolTip");
        m_stateTooltip->move(m_stateTooltip->getSuitablePos());
        m_stateTooltip->show();
    }
}

void StatusInfoInterface::createTopRightInfoBar()
{
    InfoBar::info("Lesson 3", "Believe in the spin, just keep believing!", Qt::Horizontal, true, 2000,
                  InfoBarPosition::TOP_RIGHT, this);
}

void StatusInfoInterface::createTopInfoBar()
{
    InfoBar::success("Lesson 4", "With respect, let's advance towards a new stage of the spin.", Qt::Horizontal, true,
                     2000, InfoBarPosition::TOP, this);
}

void StatusInfoInterface::createTopLeftInfoBar()
{
    InfoBar::warning("Lesson 5", "迂回路を行けば最短ルート。", Qt::Horizontal, true, 2000, InfoBarPosition::TOP_LEFT,
                     this);
}

void StatusInfoInterface::createBottomRightInfoBar()
{
    InfoBar::error("No Internet", "An error message which won't disappear automatically.", Qt::Horizontal, true, 2000,
                   InfoBarPosition::BOTTOM_RIGHT, this);
}

void StatusInfoInterface::createBottomInfoBar()
{
    InfoBar::success("Lesson 1", "Don't have any strange expectations of me.", Qt::Horizontal, true, 2000,
                     InfoBarPosition::BOTTOM, this);
}

void StatusInfoInterface::createBottomLeftInfoBar()
{
    InfoBar::warning("Lesson 2", "Don't let your muscles notice.", Qt::Horizontal, true, 2000,
                     InfoBarPosition::BOTTOM_LEFT, this);
}
