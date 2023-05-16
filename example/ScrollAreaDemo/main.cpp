#include "Dialog.h"

#include <QApplication>
#include <QBoxLayout>
#include <QFile>
#include <QTextCodec>
#include <QDebug>

#include <Widgets/Label.h>
#include <Widgets/ScrollArea.h>

class Demo : public SmoothScrollArea
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : SmoothScrollArea(parent)
    {
        auto label = new PixmapLabel(this);
        label->setPixmap(QPixmap(":/resource/shoko.jpg"));

        // customize scroll animation;
        setScrollAnimation(Qt::Vertical, 400, QEasingCurve::OutQuint);
        setScrollAnimation(Qt::Horizontal, 400, QEasingCurve::OutQuint);

        horizontalScrollBar()->setValue(1900);
        setWidget(label);
        resize(1200, 800);

        QFile f(QString(":/resource/demo.qss"));
        QString qss;
        if (f.open(QIODevice::ReadOnly)) {
            QTextCodec *codec = QTextCodec::codecForName("UTF8");
            qss               = codec->toUnicode(f.readAll());
            f.close();
        }

        //当前版本，QScrollBar不知道为什么，格式未起效
        setStyleSheet(qss);
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
    //    Dialog w;
    w.show();

    return a.exec();
}

#include "main.moc"
