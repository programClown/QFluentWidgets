#include <QApplication>

#include <Widgets/AcrylicLabel.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qfluentwidgets);

    QApplication a(argc, argv);
    AcrylicLabel w(20, QColor(105, 114, 168, 102));
    w.setImage("D:\\vscode\\PyQt-Fluent-Widgets\\examples\\acrylic_label\\resource\\埃罗芒阿老师.jpg");
    w.show();
    return a.exec();
}
