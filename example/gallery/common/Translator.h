#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>

class Translator : public QObject
{
    Q_OBJECT
public:
    explicit Translator(QObject *parent = nullptr);

    static Translator *ins();

    QString text;
    QString view;
    QString menus;
    QString icons;
    QString layout;
    QString dialogs;
    QString scroll;
    QString material;
    QString dateTime;
    QString basicInput;
    QString statusInfo;

signals:
};

#endif  // TRANSLATOR_H
