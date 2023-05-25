#include "SettingCardGroup.h"
#include "Layout/ExpandLayout.h"
#include "Common/StyleSheet.h"

SettingCardGroup::SettingCardGroup(const QString &title, QWidget *parent) : QWidget(parent)
{
    m_titleLabel = new QLabel(title, this);
    vBoxLayout   = new QVBoxLayout(this);
    cardLayout   = new ExpandLayout();

    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setAlignment(Qt::AlignTop);
    vBoxLayout->setSpacing(0);
    cardLayout->setContentsMargins(0, 0, 0, 0);
    cardLayout->setSpacing(2);

    vBoxLayout->addWidget(m_titleLabel);
    vBoxLayout->addSpacing(12);
    vBoxLayout->addLayout(cardLayout, 1);

    FluentStyleSheet::apply("SETTING_CARD_GROUP", this);
    m_titleLabel->adjustSize();
}

void SettingCardGroup::addSettingCard(QWidget *card)
{
    card->setParent(this);
    cardLayout->addWidget(card);
    adjustSize();
}

void SettingCardGroup::addSettingCards(const QList<QWidget *> &cards)
{
    for (auto c : cards) {
        addSettingCard(c);
    }
}

void SettingCardGroup::adjustSize()
{
    int h = cardLayout->heightForWidth(this->width()) + 46;
    this->resize(this->width(), h);
}
