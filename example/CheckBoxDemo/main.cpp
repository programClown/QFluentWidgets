#include <QApplication>
#include <Widgets/CheckBox.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        CheckBox *checkBox = new CheckBox("This is a check box", this);
        checkBox->setTristate(true);
        checkBox->move(120, 80);
        setStyleSheet("Demo{background:white}");
        resize(400, 400);
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
    Demo w;
    w.show();
    return a.exec();
}

#include "main.moc"
