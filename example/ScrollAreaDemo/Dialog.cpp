#include "Dialog.h"
#include "ui_Dialog.h"

#include <Widgets/Label.h>

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);

    auto label = new PixmapLabel(this);
    label->setPixmap(QPixmap(":/resource/shoko.jpg"));
    ui->scrollArea->setWidget(label);
}

Dialog::~Dialog()
{
    delete ui;
}
