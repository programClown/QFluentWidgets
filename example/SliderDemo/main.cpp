#include <QApplication>
#include <QBoxLayout>
#include <QSlider>
#include <QWidget>

#include <Widgets/Slider.h>

class Demo1 : public QWidget
{
    Q_OBJECT
public:
    Demo1(QWidget *parent = nullptr) : QWidget(parent)
    {
        resize(300, 150);
        setStyleSheet("Demo1{background: rgb(184, 106, 106)}");
        // customize style

        QMap<QString, QVariant> style;
        style.insert("sub-page.color", QColor(70, 23, 180));

        auto slider = new QSlider(Qt::Horizontal, this);
        slider->setStyle(new HollowHandleStyle(style));
        // need adjust height

        slider->resize(200, 28);
        slider->move(50, 61);
    }
};

class Demo2 : public QWidget
{
    Q_OBJECT
public:
    Demo2(QWidget *parent = nullptr) : QWidget(parent)
    {
        resize(300, 300);

        QMap<QString, QVariant> style;
        style.insert("sub-page.color", QColor(70, 23, 180));

        auto slider1 = new Slider(Qt::Horizontal, this);
        slider1->setFixedWidth(200);
        slider1->move(50, 30);

        auto slider2 = new Slider(Qt::Vertical, this);
        slider2->setFixedHeight(200);
        slider2->move(140, 80);

        setStyleSheet("Demo2{background: white}");
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
    Demo1 w1;
    w1.show();

    Demo2 w2;
    w2.show();
    return a.exec();
}

#include "main.moc"
