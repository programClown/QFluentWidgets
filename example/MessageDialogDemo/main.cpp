#include <QApplication>
#include <QWidget>
#include <QDebug>

#include <Widgets/Button.h>
#include <DialogBox/Dialog.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        resize(1000, 500);
        PrimaryPushButton *btn = new PrimaryPushButton("Click Me", this);
        btn->move(455, 25);
        connect(btn, &PrimaryPushButton::clicked, this, &Demo::showDialog);
        setStyleSheet("Demo{background:white}");
    }

private slots:
    void showDialog()
    {
        QString title   = "Are you sure you want to delete the folder?";
        QString content = "If you delete the \"Music\" folder from the list, the folder will no longer appear in the "
                          "list, but will not be deleted.";

        MessageBox w(title, content, this);
        if (w.exec() == QDialog::Accepted) {
            qDebug() << "Yes button is pressed";
        } else {
            qDebug() << "Cancel button is pressed";
        }
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
