#include "MainWindow.h"

#include <QApplication>
#include <QDebug>

#include <QFramelessWindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //    FWindow wf;  // 标题栏包含菜单、应用图标和标题居中,
    //    DWindow wd;  // 应用图标和标题
    //    AWindow wa;  // 毛玻璃和阴影效果

    QFramelessHelper *helper = new QFramelessHelper(&w);
    helper->setResizeEnabled(false);

    w.show();

    return a.exec();
}
