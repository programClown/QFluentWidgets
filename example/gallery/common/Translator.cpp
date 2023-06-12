#include "Translator.h"

Translator::Translator(QObject *parent) : QObject(parent)
{
    text       = tr("Text");
    view       = tr("View");
    menus      = tr("Menus");
    icons      = tr("Icons");
    layout     = tr("Layout");
    dialogs    = tr("Dialogs");
    scroll     = tr("Scroll");
    material   = tr("Material");
    dateTime   = tr("Date & Time");
    basicInput = tr("Basic Input");
    statusInfo = tr("Status & Info");
}

Translator *Translator::ins()
{
    static Translator *single = new Translator();
    return single;
}
