#include <QApplication>
#include <QWidget>
#include <QDebug>

#include <Widgets/Button.h>
#include <DialogBox/FolderListDialog.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        resize(800, 720);
        PrimaryPushButton *btn = new PrimaryPushButton(tr("Click Me"), this);
        btn->move(352, 300);
        connect(btn, &PrimaryPushButton::clicked, this, &Demo::showDialog);
        setStyleSheet("Demo{background:white}");
    }

private slots:
    void showDialog()
    {
        QStringList folder_paths = { "D:/出差", "C:\\Users\\Austin\\Desktop" };
        QString title            = tr("Build your collection from your local music files");
        QString content          = tr("Right now, we're watching these folders:");
        FolderListDialog *fdlg   = new FolderListDialog(folder_paths, title, content, this);
        connect(fdlg, &FolderListDialog::folderChanged, [=](const QStringList &list) { qDebug() << list; });
        fdlg->exec();
        fdlg->deleteLater();
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
