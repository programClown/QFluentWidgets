#include "MenuInterface.h"

#include "common/Translator.h"

#include <Widgets/Button.h>
#include <Widgets/Menu.h>
#include <Common/Icon.h>

MenuInterface::MenuInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->menus, "qfluentwidgets.components.widgets", parent)
{
    PushButton *button = new PushButton(tr("Show menu"));
    connect(button, &PushButton::clicked,
            [this, button]() { createMenu(button->mapToGlobal(QPoint()) + QPoint(button->width() + 5, -100)); });

    addExampleCard(tr("Rounded corners menu"), button,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/menu/demo.py");
}

void MenuInterface::createMenu(QPoint pos)
{
    RoundMenu *menu = new RoundMenu(tr("Add to"), this);

    // add actions
    menu->addAction(new QAction(FluentIcon(FluentIcon::COPY).icon(), tr("Copy")));
    menu->addAction(new QAction(FluentIcon(FluentIcon::CUT).icon(), tr("Cut")));

    // add sub menu
    RoundMenu *subMenu = new RoundMenu(tr("Add to"), this);
    subMenu->setIcon(FluentIcon(FluentIcon::ADD).icon());
    subMenu->addActions({ new QAction(FluentIcon(FluentIcon::VIDEO).icon(), tr("Video")),
                          new QAction(FluentIcon(FluentIcon::MUSIC).icon(), tr("Music")) });
    menu->addMenu(subMenu);

    menu->addActions({ new QAction(FluentIcon(FluentIcon::PASTE).icon(), tr("Paste")),
                       new QAction(FluentIcon(FluentIcon::CANCEL).icon(), tr("Undo")) });

    // add separator
    menu->addSeparator();

    // add actions
    menu->addAction(new QAction(FluentIcon(FluentIcon::SETTING).icon(), tr("Settings")));
    menu->addActions({ new QAction(FluentIcon(FluentIcon::HELP).icon(), tr("Help")),
                       new QAction(FluentIcon(FluentIcon::FEEDBACK).icon(), tr("Feedbackc")) });
    menu->addAction(new QAction(tr("Select all")));

    // show menu
    menu->exec(pos, true);
}
