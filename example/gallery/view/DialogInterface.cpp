#include "DialogInterface.h"

#include "common/Translator.h"

#include <Widgets/Button.h>
#include <DialogBox/Dialog.h>
#include <DialogBox/ColorDialog.h>

#include <QDebug>

DialogInterface::DialogInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->dialogs, "qfluentwidgets.components.dialog_box", parent)
{
    PushButton *button = new PushButton(tr("Show Dialog"));
    connect(button, &PushButton::clicked, this, &DialogInterface::showDialog);
    addExampleCard(tr("A frameless message box"), button,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/dialog/demo.py");

    button = new PushButton(tr("Show Dialog"));
    connect(button, &PushButton::clicked, this, &DialogInterface::showMessageDialog);
    addExampleCard(tr("A message box with mask"), button,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/message_dialog/demo.py");

    button = new PushButton(tr("Show Dialog"));
    connect(button, &PushButton::clicked, this, &DialogInterface::showColorDialog);
    addExampleCard(tr("A color dialog"), button,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/color_dialog/demo.py");
}

void DialogInterface::showDialog()
{
    QString title   = tr("This is a frameless message dialog");
    QString content = tr("If the content of the message box is veeeeeeeeeeeeeeeeeeeeeeeeeery long, it will "
                         "automatically wrap like this.");
    Dialog *w       = new Dialog(title, content, window());
    if (w->exec()) {
        qDebug() << __FUNCTION__ << "Yes button is clicked";
    } else {
        qDebug() << __FUNCTION__ << "Cancel button is pressed";
    }
}

void DialogInterface::showMessageDialog()
{
    QString title   = tr("");
    QString content = tr("");
    MessageBox *w   = new MessageBox(title, content, window());
    if (w->exec()) {
        qDebug() << __FUNCTION__ << "Yes button is pressed";
    } else {
        qDebug() << __FUNCTION__ << "Cancel button is pressed";
    }
}

void DialogInterface::showColorDialog()
{
    ColorDialog *w = new ColorDialog(Qt::cyan, tr("Choose color"), window());
    w->updateStyle();
    connect(w, &ColorDialog::colorChanged, [](QColor c) { qDebug() << __FUNCTION__ << c.name(); });
}
