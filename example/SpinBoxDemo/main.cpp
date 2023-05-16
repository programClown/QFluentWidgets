#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

#include <Widgets/SpinBox.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        auto vBoxLayout = new QVBoxLayout(this);

        auto spinBox       = new SpinBox(this);
        auto timeEdit      = new TimeEdit(this);
        auto dateEdit      = new DateEdit(this);
        auto dateTimeEdit  = new DateTimeEdit(this);
        auto doubleSpinBox = new DoubleSpinBox(this);

        resize(500, 500);
        setStyleSheet("Demo{background: white}");

        vBoxLayout->setContentsMargins(100, 50, 100, 50);
        vBoxLayout->addWidget(spinBox);
        vBoxLayout->addWidget(doubleSpinBox);
        vBoxLayout->addWidget(timeEdit);
        vBoxLayout->addWidget(dateEdit);
        vBoxLayout->addWidget(dateTimeEdit);
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
