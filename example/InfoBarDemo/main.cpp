#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

#include <Widgets/Button.h>
#include <Widgets/InfoBar.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
        PushButton *button1     = new PushButton("Information", this);
        PushButton *button2     = new PushButton("Success", this);
        PushButton *button3     = new PushButton("Warning", this);
        PushButton *button4     = new PushButton("Error", this);
        PushButton *button5     = new PushButton("Custom", this);

        connect(button1, &PushButton::clicked, this, &Demo::createInfoInfoBar);
        connect(button2, &PushButton::clicked, this, &Demo::createSuccessInfoBar);
        connect(button3, &PushButton::clicked, this, &Demo::createWarningInfoBar);
        connect(button4, &PushButton::clicked, this, &Demo::createErrorInfoBar);
        connect(button5, &PushButton::clicked, this, &Demo::createCustomInfoBar);

        hBoxLayout->addWidget(button1);
        hBoxLayout->addWidget(button2);
        hBoxLayout->addWidget(button3);
        hBoxLayout->addWidget(button4);
        hBoxLayout->addWidget(button5);
        hBoxLayout->setContentsMargins(30, 0, 30, 0);

        resize(700, 700);
    }

private slots:
    void createInfoInfoBar()
    {
        QString content =
                "My name is kira yoshikake, 33 years old. Living in the villa area northeast of duwangting, unmarried. "
                "I work in Guiyou chain store. Every day I have to work overtime until 8 p.m. to go home. I don't "
                "smoke. The wine is only for a taste. Sleep at 11 p.m. for 8 hours a day. Before I go to bed, I must "
                "drink a cup of warm milk, then do 20 minutes of soft exercise, get on the bed, and immediately fall "
                "asleep. Never leave fatigue and stress until the next day. Doctors say I'm normal.";
        InfoBar *w = new InfoBar(NEWFLICON(InfoBarIcon, INFORMATION), "Title", content, Qt::Vertical, true, 2000,
                                 InfoBarPosition::TOP_RIGHT, this);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->addWidget(new PushButton("Action"));
        w->show();
    }

    void createSuccessInfoBar()
    {
        InfoBar::success("Lesson 4", "With respect, let's advance towards a new stage of the spin.", Qt::Horizontal,
                         true, 2000, InfoBarPosition::TOP, this);
    }

    void createWarningInfoBar()
    {
        InfoBar::warning("Lesson 3", "Believe in the spin, just keep believing!", Qt::Horizontal,
                         false,  // disable close button
                         2000, InfoBarPosition::TOP_LEFT, this);
    }

    void createErrorInfoBar()
    {
        InfoBar::error("Lesson 5", "迂回路を行けば最短ルート。", Qt::Horizontal, true,
                       -1,  // won't disappear automatically
                       InfoBarPosition::BOTTOM_RIGHT, this);
    }

    void createCustomInfoBar()
    {
        auto w = InfoBar::creator(NEWFLICON(FluentIcon, GITHUB), "Zeppeli",
                                  "人間讃歌は「勇気」の讃歌ッ！！ 人間のすばらしさは勇気のすばらしさ！！",
                                  Qt::Horizontal, true, 2000, InfoBarPosition::BOTTOM, this);
        w->setCustomBackgroundColor("white", "#202020");
    }
};

int main(int argc, char *argv[])
{
// enable dpi scale
#if (QT_VERSION > QT_VERSION_CHECK(5, 14, 0))
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Q_INIT_RESOURCE(qfluentwidgets);

    QApplication a(argc, argv);
    Demo w;
    w.show();
    return a.exec();
}

#include "main.moc"
