#include "LinkCard.h"

#include "common/GalleryStyleSheet.h"

#include "Common/Icon.h"
#include "Widgets/IconWidget.h"
#include "Common/StyleSheet.h"

#include <QDesktopServices>
#include <QLabel>
#include <QBoxLayout>
#include <QBrush>
#include <QPalette>

#include <QDebug>

LinkCard::LinkCard(FluentIconBase *icon, QString title, QString content, QString const url, QWidget *parent)
    : QFrame(parent)
{
    m_url = QUrl(url);
    setFixedSize(198, 220);
    m_iconWidget   = new IconWidget(icon, this);
    m_titleLabel   = new QLabel(title, this);
    m_contentLabel = new QLabel(content, this);
    m_contentLabel->setWordWrap(true);
    m_urlWidget = new IconWidget(NEWFLICON(FluentIcon, LINK), this);

    //    setAutoFillBackground(true);

    initWidget();
}

void LinkCard::initWidget()
{
    setCursor(Qt::PointingHandCursor);

    m_iconWidget->setFixedSize(54, 54);
    m_urlWidget->setFixedSize(16, 16);

    m_vBoxLayout = new QVBoxLayout(this);
    m_vBoxLayout->setSpacing(0);
    m_vBoxLayout->setContentsMargins(24, 24, 0, 13);

    m_vBoxLayout->addWidget(m_iconWidget);
    m_vBoxLayout->addSpacing(16);
    m_vBoxLayout->addWidget(m_titleLabel);
    m_vBoxLayout->addSpacing(8);
    m_vBoxLayout->addWidget(m_contentLabel);
    m_vBoxLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_urlWidget->move(170, 192);
    m_titleLabel->setObjectName("titleLabel");
    m_contentLabel->setObjectName("contentLabel");
}

void LinkCard::mouseReleaseEvent(QMouseEvent *event)
{
    QFrame::mouseReleaseEvent(event);

    QDesktopServices::openUrl(m_url);
}

LinkCardView::LinkCardView(QWidget *parent) : ScrollArea(parent, Qt::Horizontal)
{
    m_view       = new QWidget(this);
    m_hBoxLayout = new QHBoxLayout(m_view);

    m_hBoxLayout->setContentsMargins(36, 0, 0, 0);
    m_hBoxLayout->setSpacing(12);
    m_hBoxLayout->setAlignment(Qt::AlignLeft);

    setWidget(m_view);
    setWidgetResizable(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_view->setObjectName("view");
    //    GalleryStyleSheet::apply(GalleryStyleSheet::LINK_CARD, this);

    //    setStyleSheet("#view { background-color: yellow }");
    //    setStyleSheet("#titleLabel { font: 18px }");

    setStyleSheet("#view { background-color: yellow }");

    qDebug() << __FUNCTION__ << __LINE__ << palette().background().color();
}

void LinkCardView::addCard(FluentIconBase *icon, QString title, QString content, QString const url)
{
    LinkCard *card = new LinkCard(icon, title, content, url, m_view);
    m_hBoxLayout->addWidget(card, 0, Qt::AlignLeft);
}
