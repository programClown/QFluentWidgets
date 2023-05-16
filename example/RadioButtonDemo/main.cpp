#include <QApplication>
#include <QBoxLayout>
#include <QWidget>

#include <Widgets/Button.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        auto vBoxLayout = new QVBoxLayout(this);
        auto button1    = new RadioButton("Option 1", this);
        auto button2    = new RadioButton("Option 2", this);
        auto button3    = new RadioButton("Option 3", this);

        vBoxLayout->addWidget(button1, 0, Qt::AlignCenter);
        vBoxLayout->addWidget(button2, 0, Qt::AlignCenter);
        vBoxLayout->addWidget(button3, 0, Qt::AlignCenter);
        resize(300, 150);
        setStyleSheet("Demo{background:white}");
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
