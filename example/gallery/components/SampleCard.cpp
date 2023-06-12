#include "SampleCard.h"

#include "common/SignalBus.h"
#include "common/GalleryStyleSheet.h"

#include <Widgets/IconWidget.h>
#include <Common/AutoWrap.h>
#include <Layout/FlowLayout.h>

#include <QLabel>
#include <QBoxLayout>

SampleCard::SampleCard(FluentIconBase *icon, const QString title, const QString content, const QString routeKey,
                       int index, QWidget *parent)
    : QFrame(parent)
{
    m_index    = index;
    m_routeKey = routeKey;

    m_iconWidget   = new IconWidget(icon, this);
    m_titleLabel   = new QLabel(title, this);
    m_contentLabel = new QLabel(TextWrap::wrap(content, 45, false).first, this);

    m_hBoxLayout = new QHBoxLayout(this);
    m_vBoxLayout = new QVBoxLayout();

    setFixedSize(360, 90);
    m_iconWidget->setFixedSize(48, 48);

    m_hBoxLayout->setSpacing(28);
    m_hBoxLayout->setContentsMargins(20, 0, 0, 0);

    m_vBoxLayout->setSpacing(2);
    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxLayout->setAlignment(Qt::AlignVCenter);

    m_hBoxLayout->setAlignment(Qt::AlignVCenter);
    m_hBoxLayout->addWidget(m_iconWidget);
    m_hBoxLayout->addLayout(m_vBoxLayout);
    m_vBoxLayout->addStretch(1);
    m_vBoxLayout->addWidget(m_titleLabel);
    m_vBoxLayout->addWidget(m_contentLabel);
    m_vBoxLayout->addStretch(1);

    m_titleLabel->setObjectName("titleLabel");
    m_contentLabel->setObjectName("contentLabel");
}

void SampleCard::mouseReleaseEvent(QMouseEvent *event)
{
    QFrame::mouseReleaseEvent(event);
    emit SignalBus::signalBus()->switchToSampleCard(m_routeKey, m_index);
}

SampleCardView::SampleCardView(const QString title, QWidget *parent) : QWidget(parent)
{
    m_titleLabel = new QLabel(title, this);
    m_vBoxLayout = new QVBoxLayout(this);
    m_flowLayout = new FlowLayout();

    m_vBoxLayout->setContentsMargins(36, 0, 36, 0);
    m_vBoxLayout->setSpacing(0);
    m_flowLayout->setContentsMargins(0, 0, 0, 0);
    m_flowLayout->setHorizontalSpacing(12);
    m_flowLayout->setVerticalSpacing(12);

    m_vBoxLayout->addWidget(m_titleLabel);
    m_vBoxLayout->addLayout(m_flowLayout, 1);

    m_titleLabel->setObjectName("viewTitleLabel");
    GalleryStyleSheet::apply(GalleryStyleSheet::SAMPLE_CARD, this);
}

void SampleCardView::addSampleCard(FluentIconBase *icon, const QString title, const QString content,
                                   const QString routeKey, int index)
{
    SampleCard *card = new SampleCard(icon, title, content, routeKey, index, this);
    m_flowLayout->addWidget(card);
}
