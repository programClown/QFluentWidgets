#ifndef LINKCARD_H
#define LINKCARD_H

#include <Widgets/ScrollArea.h>

#include <QFrame>
#include <QUrl>

class FluentIcon;

class LinkCard : public QFrame
{
public:
    LinkCard(FluentIcon icon, QString title, QString content, QUrl url, QWidget *parent = nullptr);

private:
    void initWidget();

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QUrl m_url;
};

class LinkCardView : ScrollArea
{
public:
    LinkCardView(QWidget *parent = nullptr);

    void addCard(FluentIcon icon, QString title, QString content, QUrl url);
};

#endif  // LINKCARD_H
