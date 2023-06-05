#include "LinkCard.h"

#include "Common/Icon.h"

#include <QDesktopServices>

LinkCard::LinkCard(FluentIcon icon, QString title, QString content, QUrl url, QWidget *parent) : QFrame(parent) { }

void LinkCard::initWidget() { }

void LinkCard::mouseReleaseEvent(QMouseEvent *event)
{
    QFrame::mouseReleaseEvent(event);

    QDesktopServices::openUrl(m_url);
}

LinkCardView::LinkCardView(QWidget *parent) : ScrollArea(parent) { }

void LinkCardView::addCard(FluentIcon icon, QString title, QString content, QUrl url) { }
