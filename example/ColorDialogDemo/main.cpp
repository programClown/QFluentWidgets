#include <QApplication>
#include <QWidget>

#include <Settings/SettingCard.h>
#include <DialogBox/ColorDialog.h>
#include <DialogBox/HuePanel.h>
#include <DialogBox/MaskDialogBase.h>
#include <QHBoxLayout>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        ColorPickerButton *button = new ColorPickerButton(QColor("#12aaa2"), tr("Background Color"), this);
        resize(800, 720);
        button->move(352, 312);
        setStyleSheet("Demo{background:white}");

        //        QHBoxLayout *hlayout = new QHBoxLayout(this);
        //        HuePanel *mb         = new HuePanel(QColor(255, 0, 0), this);
        //        MaskDialogBase *mb = new MaskDialogBase(this);
        //        mb->setMaskColor(QColor("red"));
        //        ColorCard *mb = new ColorCard(QColor("red"), this);
        //        BrightnessSlider *mb = new BrightnessSlider(QColor("red"), this);
        //        HexColorLineEdit *mb = new HexColorLineEdit(QColor("red"), this);
        //        ColorLineEdit *mb = new ColorLineEdit("red", this);
        //        resize(800, 700);
        //        hlayout->addWidget(mb);
    }
};

int main(int argc, char *argv[])
{
    // enable dpi scale
    //    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Q_INIT_RESOURCE(qfluentwidgets);

    QApplication a(argc, argv);
    Demo w;
    w.show();
    return a.exec();
}

#include "main.moc"
