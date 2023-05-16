#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

#include <Widgets/Button.h>
#include <Widgets/StateToolTip.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        resize(800, 300);
        auto btn = new PushButton("Click Me", this);
        btn->move(360, 225);
        connect(btn, &PushButton::clicked, this, &Demo::onButtonClicked);
        m_stateToolTip = nullptr;

        setStyleSheet("Demo{background:white}");
    }

private slots:
    void onButtonClicked()
    {
        if (m_stateToolTip) {
            m_stateToolTip->setContent("模型训练完成啦");
            m_stateToolTip->setState(true);
            m_stateToolTip->deleteLater();
            m_stateToolTip = nullptr;
        } else {
            m_stateToolTip = new StateToolTip("正在训练模型", "客官请耐心等待哦~~", this);
            m_stateToolTip->move(510, 30);
            m_stateToolTip->show();
        }
    }

private:
    StateToolTip *m_stateToolTip;
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
