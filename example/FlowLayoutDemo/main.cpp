#include <QApplication>
#include <QPushButton>
#include <QWidget>

#include <Layout/FlowLayout.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {

        FlowLayout *layout = new FlowLayout(this, true, false);

        layout->setAnimation(250, QEasingCurve::OutQuad);

        layout->setContentsMargins(30, 30, 30, 30);
        layout->setVerticalSpacing(20);
        layout->setHorizontalSpacing(10);

        layout->addWidget(new QPushButton("aiko"));
        layout->addWidget(new QPushButton("刘静爱"));
        layout->addWidget(new QPushButton("柳井爱子"));
        layout->addWidget(new QPushButton("aiko 赛高"));
        layout->addWidget(new QPushButton("aiko 太爱啦"));
        resize(250, 300);
        setStyleSheet("Demo{background: white} QPushButton{padding: 5px 10px; font:15px \"Microsoft YaHei\"}");
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Demo w;
    w.show();
    return a.exec();
}

#include "main.moc"
