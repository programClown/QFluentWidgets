#ifndef LINKCARD_H
#define LINKCARD_H

#include <Widgets/ScrollArea.h>

#include <QFrame>
#include <QUrl>

class FluentIconBase;
class IconWidget;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;

class LinkCard : public QFrame
{
public:
    LinkCard(FluentIconBase *icon, QString title, QString content, QString const url, QWidget *parent = nullptr);

private:
    void initWidget();

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QUrl m_url;
    IconWidget *m_iconWidget;
    QLabel *m_titleLabel;
    QLabel *m_contentLabel;
    IconWidget *m_urlWidget;

    QVBoxLayout *m_vBoxLayout;
};

class LinkCardView : public ScrollArea
{
public:
    LinkCardView(QWidget *parent = nullptr);

    void addCard(FluentIconBase *icon, QString title, QString content, QString const url);

private:
    QWidget *m_view;
    QHBoxLayout *m_hBoxLayout;
};

#endif  // LINKCARD_H
