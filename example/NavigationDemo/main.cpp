#include <QApplication>
#include <QScreen>
#include <QBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QMainWindow>
#include <QPainter>
#include <QStackedWidget>
#include <QFile>

#include <QFluentWidgets.h>
#include <Navigation/NavigationInterface.h>
#include <DialogBox/Dialog.h>

class Widget : public QFrame
{
    Q_OBJECT
public:
    QLabel *label;
    QHBoxLayout *hBoxLayout;

    Widget(const QString &text, QWidget *parent = nullptr) : QFrame(parent)
    {
        label = new QLabel(text, this);
        label->setAlignment(Qt::AlignCenter);
        hBoxLayout = new QHBoxLayout(this);
        hBoxLayout->addWidget(label, 1, Qt::AlignCenter);
        QString txt = text;
        setObjectName(txt.replace(" ", "-"));
    }
};

class AvatarWidget : public NavigationWidget
{
    Q_OBJECT
public:
    QImage avatar;

    explicit AvatarWidget(QWidget *parent = nullptr) : NavigationWidget(false, parent)
    {
        avatar = QImage(":/resource/shoko.png").scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // QWidget interface
protected:
    void paintEvent(QPaintEvent * /*event*/) override
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);

        if (isPressed) {
            painter.setOpacity(0.7);
        }

        // draw background
        if (isEnter) {
            int c = QFWIns.isDarkTheme() ? 255 : 0;
            painter.setBrush(QColor(c, c, c, 10));
            painter.drawRoundedRect(rect(), 5, 5);
        }

        // draw avatar
        painter.setBrush(QBrush(avatar));
        painter.translate(8, 6);
        painter.drawEllipse(0, 0, 24, 24);
        painter.translate(-8, -6);

        if (!isCompacted) {
            if (QFWIns.isDarkTheme()) {
                painter.setPen(Qt::white);
            } else {
                painter.setPen(Qt::black);
            }

            QFont font("Segoe UI");
            font.setPixelSize(14);
            painter.setFont(font);
            painter.drawText(QRect(44, 0, 255, 36), Qt::AlignVCenter, "zhiyiYo");
        }
    }
};

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr) : QWidget(parent)
    {
        hBoxLayout          = new QHBoxLayout();
        navigationInterface = new NavigationInterface(true, true);
        stackWidget         = new QStackedWidget();
        // create sub interface;
        searchInterface  = new Widget("Search Interface");
        musicInterface   = new Widget("Music Interface");
        videoInterface   = new Widget("Video Interface");
        folderInterface  = new Widget("Folder Interface");
        settingInterface = new Widget("Setting Interface");

        stackWidget->addWidget(searchInterface);
        stackWidget->addWidget(musicInterface);
        stackWidget->addWidget(videoInterface);
        stackWidget->addWidget(folderInterface);
        stackWidget->addWidget(settingInterface);

        initLayout();

        initNavigation();

        initWindow();
    }

private:
    void initLayout()
    {
        hBoxLayout->setSpacing(0);
        hBoxLayout->setContentsMargins(0, 39, 0, 0);
        hBoxLayout->addWidget(navigationInterface);
        hBoxLayout->addWidget(stackWidget);
        hBoxLayout->setStretchFactor(stackWidget, 1);
        setLayout(hBoxLayout);
    }

    void initNavigation()
    {
        navigationInterface->addItem(searchInterface->objectName(), NEWFLICON(FluentIcon, SEARCH), "Search", this,
                                     SLOT(searchInterfaceClick()));
        navigationInterface->addItem(musicInterface->objectName(), NEWFLICON(FluentIcon, MUSIC), "Music library", this,
                                     SLOT(musicInterfaceClick()));
        navigationInterface->addItem(videoInterface->objectName(), NEWFLICON(FluentIcon, VIDEO), "Video library", this,
                                     SLOT(videoInterfaceClick()));

        navigationInterface->addSeparator();
        // add navigation items to scroll area
        navigationInterface->addItem(folderInterface->objectName(), NEWFLICON(FluentIcon, FOLDER), "Folder library",
                                     this, SLOT(folderInterfaceClick()), true, NavigationItemPosition::SCROLL);

        for (int i = 1; i <= 20; i++) {

            QString name = QString("Folder %1").arg(i);

            navigationInterface->addItem(name, NEWFLICON(FluentIcon, FOLDER), name, this, SLOT(folderInterfaceClick()),
                                         true, NavigationItemPosition::SCROLL);
        }

        // add custom widget to bottom
        navigationInterface->addWidget("avatar", new AvatarWidget(this), this, SLOT(showMessageBox()),
                                       NavigationItemPosition::BOTTOM);

        navigationInterface->addItem(settingInterface->objectName(), NEWFLICON(FluentIcon, SETTING), "Settings", this,
                                     SLOT(settingInterfaceClick()), true, NavigationItemPosition::BOTTOM);

        // IMPORTANT: don't forget to set the default route key if you enable the return button
        navigationInterface->setDefaultRouteKey(musicInterface->objectName());

        // set the maximum width
        // navigationInterface->setExpandWidth(300);
        connect(stackWidget, &QStackedWidget::currentChanged, this, &Window::onCurrentInterfaceChanged);
        stackWidget->setCurrentIndex(1);
    }

    void initWindow()
    {
        resize(900, 700);
        setWindowIcon(QIcon(":/resource/logo.png"));
        setWindowTitle("PyQt-Fluent-Widgets");

        QRect desktop = QApplication::screens().at(0)->availableGeometry();
        int w         = desktop.width();
        int h         = desktop.height();
        move(w / 2 - width() / 2, h / 2 - height() / 2);
        setQss();
    }

    void setQss()
    {
        QString color = QFWIns.isDarkTheme() ? "dark" : "light";
        QFile f(QString(":/resource/%1/demo.qss").arg(color));
        f.open(QIODevice::ReadOnly);
        setStyleSheet(f.readAll());
        f.close();
    }

    void switchTo(QWidget *widget) { stackWidget->setCurrentWidget(widget); }

    void onCurrentInterfaceChanged(int index)
    {
        QWidget *widget = stackWidget->widget(index);
        navigationInterface->setCurrentItem(widget->objectName());
    }

private slots:
    void searchInterfaceClick() { switchTo(searchInterface); }
    void musicInterfaceClick() { switchTo(musicInterface); }
    void videoInterfaceClick() { switchTo(videoInterface); }
    void folderInterfaceClick() { switchTo(folderInterface); }
    void settingInterfaceClick() { switchTo(settingInterface); }
    void showMessageBox()
    {
        MessageBox w("This is a help message",
                     "You clicked a customized navigation widget. You can add more custom widgets by calling "
                     "`NavigationInterface.addWidget()` ",
                     this);
        w.exec();
    }

private:
    QHBoxLayout *hBoxLayout;
    NavigationInterface *navigationInterface;
    QStackedWidget *stackWidget;

    Widget *searchInterface;
    Widget *musicInterface;
    Widget *videoInterface;
    Widget *folderInterface;
    Widget *settingInterface;
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
    Window w;
    w.show();
    return a.exec();
}

#include "main.moc"
