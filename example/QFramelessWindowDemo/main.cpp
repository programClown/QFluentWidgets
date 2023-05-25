#include "MainWindow.h"

#include "QFluentWidgets.h"

#include "FWindow.h"
#include "DWindow.h"
#include "AWindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    FWindow wf;  // 标题栏包含菜单、应用图标和标题居中,
    DWindow wd;  // 应用图标和标题
    AWindow wa;  // 毛玻璃和阴影效果

    wf.show();

    qDebug() << __FUNCTION__ << __LINE__ << QFluentWidgets::ins().themeName();

    return a.exec();
}
