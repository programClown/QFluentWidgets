#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

#include <Widgets/Button.h>
#include <Widgets/ToolTip.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        auto hBox    = new QHBoxLayout(this);
        auto button1 = new PushButton("キラキラ", this);
        auto button2 = new PushButton("食べた愛", this);
        auto button3 = new PushButton("シアワセ", this);
        m_toolTip    = new ToolTip("", this);

        button1->setToolTip("aiko - キラキラ ");
        button2->setToolTip("aiko - 食べた愛 ");
        button3->setToolTip("aiko - シアワセ ");

        button1->setToolTipDuration(1000);
        button2->setToolTipDuration(5000);
        button1->installEventFilter(this);
        button2->installEventFilter(this);
        button3->installEventFilter(new ToolTipFilter(button3));

        hBox->setContentsMargins(24, 24, 24, 24);
        hBox->setSpacing(16);
        hBox->addWidget(button1);
        hBox->addWidget(button2);
        hBox->addWidget(button3);

        resize(480, 240);
        m_toolTip->hide();

        setStyleSheet("Demo{background:white}");
    }

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == this) {
            return QWidget::eventFilter(watched, event);
        }

        QWidget *obj = static_cast<QWidget *>(watched);
        if (event->type() == QEvent::Enter) {
            m_toolTip->setText(obj->toolTip());
            m_toolTip->setDuration(obj->toolTipDuration());
            m_toolTip->adjustPos(obj->mapToGlobal(QPoint()), obj->size());
            m_toolTip->show();
        } else if (event->type() == QEvent::Leave) {
            m_toolTip->hide();
        } else if (event->type() == QEvent::ToolTip) {
            return true;
        }
        return QWidget::eventFilter(watched, event);
    }

private:
    ToolTip *m_toolTip;
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
