#include "GalleryStyleSheet.h"

#include <QFluentWidgets.h>

#include <QFile>
#include <QTextStream>

QString GalleryStyleSheet::typeName(GalleryStyleSheet::Type t)
{
    switch (t) {
        case LINK_CARD:
            return "link_card";
        case MAIN_WINDOW:
            return "main_window";
        case SAMPLE_CARD:
            return "sample_card";
        case HOME_INTERFACE:
            return "home_interface";
        case ICON_INTERFACE:
            return "icon_interface";
        case VIEW_INTERFACE:
            return "view_interface";
        case SETTING_INTERFACE:
            return "setting_interface";
        case GALLERY_INTERFACE:
            return "gallery_interface";
        default:
            return "";
    }
}

QString GalleryStyleSheet::path(Qfw::Theme theme)
{
    return "";
}

QString GalleryStyleSheet::content(const QString &name)
{
    QString fullName;
    if (QFWIns.isDarkTheme()) {
        fullName = QString(":/resource/qss/dark/%2.qss").arg(name.toLower());
    } else {
        fullName = QString(":/resource/qss/light/%2.qss").arg(name.toLower());
    }
    QFile f(fullName);
    QString qss = "";
    if (f.open(QIODevice::ReadOnly)) {
        // 用QTextStream读取样式文件不用区分文件编码 带bom也行
        QTextStream in(&f);
        in.setCodec("utf-8");
        while (!in.atEnd()) {
            QString line = in.readLine();
            qss += line + "\r\n";
        }
        f.close();
    }

    return qss;
}

QString GalleryStyleSheet::operator[](GalleryStyleSheet::Type t)
{
    return typeName(t);
}

void GalleryStyleSheet::apply(GalleryStyleSheet::Type t, QWidget *widget)
{
    QString name = typeName(t);
    if (!name.isEmpty()) {
        apply(name, widget);
    }
}

void GalleryStyleSheet::apply(const QString &name, QWidget *widget)
{
    QString qss = content(name);
    widget->setStyleSheet(applyThemeColor(qss));
}
