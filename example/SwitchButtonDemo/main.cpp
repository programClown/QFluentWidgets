#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

#include <Widgets/SwitchButton.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        resize(160, 80);

        m_switchButton = new SwitchButton("Off", this);
        m_switchButton->move(48, 24);
        connect(m_switchButton, &SwitchButton::checkedChanged, this, &Demo::onCheckedChanged);
    }

private slots:
    void onCheckedChanged(bool checked)
    {
        QString text = checked ? "On" : "Off";
        m_switchButton->setText(text);
    }

private:
    SwitchButton *m_switchButton;
};

int main(int argc, char *argv[])
{
// enable dpi scale
#if (QT_VERSION > QT_VERSION_CHECK(5, 14, 0))
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Q_INIT_RESOURCE(qfluentwidgets);

    QApplication a(argc, argv);
    Demo w;
    w.show();
    return a.exec();
}

#include "main.moc"
