#include <QApplication>
#include <Widgets/Button.h>
#include <QStyle>
#include <QVBoxLayout>
#include <Common/Icon.h>
#include <QFluentWidgets.h>

class ButtonDemo : public QWidget
{
    Q_OBJECT
public:
    ButtonDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        ToolButton *toolButton  = new ToolButton(NEWFLICON(FluentIcon, SETTING), this);
        PushButton *pushButton1 = new PushButton("Standard push button", nullptr, this);
        PushButton *pushButton2 = new PushButton("Standard push button with icon", NEWFLICON(FluentIcon, FOLDER), this);

        PrimaryPushButton *primaryButton1 = new PrimaryPushButton("Accent style button", nullptr, this);
        PrimaryPushButton *primaryButton2 =
                new PrimaryPushButton("Accent style button with icon", NEWFLICON(FluentIcon, UPDATE), this);

        HyperlinkButton *hyperlinkButton =
                new HyperlinkButton("https://github.com/zhiyiYo/PyQt-Fluent-Widgets", "Hyper link button", this);

        QVBoxLayout *vBoxLayout = new QVBoxLayout(this);

        vBoxLayout->addWidget(toolButton, 0, Qt::AlignCenter);
        vBoxLayout->addWidget(pushButton1, 0, Qt::AlignCenter);
        vBoxLayout->addWidget(pushButton2, 0, Qt::AlignCenter);
        vBoxLayout->addWidget(primaryButton1, 0, Qt::AlignCenter);
        vBoxLayout->addWidget(primaryButton2, 0, Qt::AlignCenter);
        vBoxLayout->addWidget(hyperlinkButton, 0, Qt::AlignCenter);
        vBoxLayout->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);

        setLayout(vBoxLayout);
        resize(400, 400);
        setStyleSheet("ButtonDemo{background:white}");
    }
};

int main(int argc, char *argv[])
{
    // enable dpi scale
    //    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Q_INIT_RESOURCE(qfluentwidgets);

    QApplication a(argc, argv);
    QFluentWidgets::ins();
    ButtonDemo w;
    w.show();
    return a.exec();
}

#include "main.moc"
