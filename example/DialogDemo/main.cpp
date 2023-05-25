#include <QApplication>
#include <QWidget>

#include <Widgets/Button.h>
#include <DialogBox/Dialog.h>

#include <QDebug>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        resize(950, 500);
        PrimaryPushButton *btn = new PrimaryPushButton("Click Me", this);
        btn->move(425, 225);
        connect(btn, &PrimaryPushButton::clicked, [=]() {
            QString title   = "Are you sure you want to delete the folder?";
            QString content = "If you delete the \" Music \" folder from the "
                              "list, the folder will no longer appear in "
                              "the list, but will not be deleted.";

            Dialog w(title, content, this);
            w.setTitleBarVisible(false);
            if (w.exec()) {
                qDebug() << "Yes button is pressed";
            } else {
                qDebug() << "Cancel button is pressed";
            }
        });
        setStyleSheet("Demo{background:white}");
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
