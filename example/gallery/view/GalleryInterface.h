#ifndef GALLERYINTERFACE_H
#define GALLERYINTERFACE_H

#include <Widgets/ScrollArea.h>

class QLabel;
class PushButton;
class ToolButton;
class QVBoxLayout;
class QHBoxLayout;
class IconWidget;

class ToolBar : public QWidget
{
public:
    ToolBar(QString title, QString subtitle, QWidget *parent = nullptr);

    void initWidget();

    void toggleTheme();

private:
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;

    PushButton *m_documentButton;
    PushButton *m_sourceButton;
    ToolButton *m_themeButton;
    ToolButton *m_feedbackButton;

    QVBoxLayout *m_vBoxLayout;
    QHBoxLayout *m_buttonLayout;
};

class ExampleCard : public QWidget
{
public:
    ExampleCard(QString const title, QWidget *widget, QString sourcePath, int stretch, QWidget *parent = nullptr);

    void initWidget();

    void initLayout();

    bool eventFilter(QObject *obj, QEvent *e) override;

    QFrame *card() const;

private:
    QWidget *m_widget;
    int m_stretch;

    QLabel *m_titleLabel;
    QFrame *m_card;

    QFrame *m_sourceWidget;
    QString m_sourcePath;
    QLabel *m_sourcePathLabel;
    IconWidget *m_linkIcon;

    QVBoxLayout *m_vBoxLayout;
    QVBoxLayout *m_cardLayout;
    QHBoxLayout *m_topLayout;
    QHBoxLayout *m_bottomLayout;
};

class GalleryInterface : public ScrollArea
{
public:
    GalleryInterface(QString title, QString subtitle, QWidget *parent = nullptr);

    ExampleCard *addExampleCard(QString title, QWidget *widget, QString sourcePath, int stretch = 0);

    void scrollToCard(int index);

    void resizeEvent(QResizeEvent *) override;

protected:
    QVBoxLayout *m_vBoxLayout;

private:
    QWidget *m_view;
    ToolBar *m_toolBar;
};

#endif  // GALLERYINTERFACE_H
