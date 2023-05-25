#include <QApplication>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QWidget>

#include <Widgets/TreeView.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        auto hBoxLayout = new QHBoxLayout(this);
        setStyleSheet("Demo{background:rgb(255,255,255)}");

        auto view  = new TreeView(this);
        auto model = new QFileSystemModel();
        model->setRootPath(".");
        view->setModel(model);

        hBoxLayout->addWidget(view);
        hBoxLayout->setContentsMargins(30, 30, 30, 30);
        resize(700, 600);
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
