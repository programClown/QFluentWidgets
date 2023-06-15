#include "IconInterface.h"

#include "common/Translator.h"
#include "common/Trie.h"
#include "common/GalleryConfig.h"
#include "common/GalleryStyleSheet.h"

#include <Widgets/IconWidget.h>
#include <Common/Theme.h>
#include <QFluentWidgets.h>
#include <Widgets/ScrollArea.h>
#include <Layout/FlowLayout.h>

#include <QLabel>
#include <QApplication>

GalleryLineEdit::GalleryLineEdit(QWidget *parent) : SearchLineEdit(parent)
{
    setPlaceholderText(tr("Search icons"));
    setFixedWidth(304);
    connect(this, &GalleryLineEdit::textChanged, this, &GalleryLineEdit::search);
}

IconCard::IconCard(FluentIconBase *icon, QWidget *parent) : QFrame(parent), m_icon(icon)
{
    m_iconWidget = new IconWidget(icon, this);
    m_nameLabel  = new QLabel(this);
    m_vBoxLayout = new QVBoxLayout(this);

    setFixedSize(96, 96);
    m_vBoxLayout->setSpacing(0);
    m_vBoxLayout->setContentsMargins(8, 28, 8, 0);
    m_vBoxLayout->setAlignment(Qt::AlignTop);
    m_iconWidget->setFixedSize(28, 28);
    m_vBoxLayout->addWidget(m_iconWidget, 0, Qt::AlignHCenter);
    m_vBoxLayout->addSpacing(14);
    m_vBoxLayout->addWidget(m_nameLabel, 0, Qt::AlignHCenter);

    QString typeName = icon->iconEngine->iconPath().split("/").last().split("_").first();
    QString text     = m_nameLabel->fontMetrics().elidedText(typeName, Qt::ElideRight, 78);
    m_nameLabel->setText(text);
}

void IconCard::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if (m_isSelected) {
        return;
    }

    emit clicked(m_icon.data());
}

void IconCard::setSelected(bool isSelected, bool force)
{
    if (isSelected == m_isSelected && !force) {
        return;
    }

    m_isSelected = isSelected;

    if (isSelected) {
        m_icon->setTheme(QFWIns.isDarkTheme() ? Qfw::Theme::LIGHT : Qfw::Theme::DARK);
    }

    m_iconWidget->setIcon(m_icon.data());

    setStyle(QApplication::style());
}

IconInfoPanel::IconInfoPanel(FluentIconBase *icon, QWidget *parent) : QFrame(parent)
{
    m_nameLabel          = new QLabel(icon->typeName(), this);
    m_iconWidget         = new IconWidget(icon, this);
    m_iconNameTitleLabel = new QLabel(tr("Icon name"));
    m_iconNameLabel      = new QLabel(icon->typeName(), this);
    m_enumNameTitleLabel = new QLabel(tr("Enum member"));
    m_enumNameLabel      = new QLabel("FluentIcon::" + icon->enumName());

    m_vBoxLayout = new QVBoxLayout(this);
    m_vBoxLayout->setContentsMargins(16, 20, 16, 20);
    m_vBoxLayout->setSpacing(0);
    m_vBoxLayout->setAlignment(Qt::AlignTop);

    m_vBoxLayout->addWidget(m_nameLabel);
    m_vBoxLayout->addSpacing(16);
    m_vBoxLayout->addWidget(m_iconWidget);
    m_vBoxLayout->addSpacing(45);
    m_vBoxLayout->addWidget(m_iconNameTitleLabel);
    m_vBoxLayout->addSpacing(5);
    m_vBoxLayout->addWidget(m_iconNameLabel);
    m_vBoxLayout->addSpacing(34);
    m_vBoxLayout->addWidget(m_enumNameTitleLabel);
    m_vBoxLayout->addSpacing(5);
    m_vBoxLayout->addWidget(m_enumNameLabel);

    m_iconWidget->setFixedSize(48, 48);
    setFixedWidth(216);

    m_nameLabel->setObjectName("nameLabel");
    m_iconNameTitleLabel->setObjectName("subTitleLabel");
    m_enumNameTitleLabel->setObjectName("subTitleLabel");
}

void IconInfoPanel::setIcon(FluentIconBase *icon)
{
    m_iconWidget->setIcon(icon);
    //    m_nameLabel->setText(icon->typeName());
    //    m_iconNameLabel->setText(icon->typeName());
    //    m_enumNameLabel->setText("FluentIcon::" + icon->enumName());
}

IconCardView::IconCardView(QWidget *parent) : QWidget(parent)
{
    m_trie             = new Trie();
    m_iconLibraryLabel = new QLabel(tr("Fluent Icons Library"), this);
    m_searchLineEdit   = new GalleryLineEdit(this);

    m_view         = new QFrame(this);
    m_scrollArea   = new SmoothScrollArea(m_view);
    m_scrollWidget = new QWidget(m_scrollArea);
    m_infoPannel   = new IconInfoPanel(NEWFLICON(FluentIcon, MENU), this);

    m_vBoxLayout = new QVBoxLayout(this);
    m_hBoxLayout = new QHBoxLayout(m_view);
    m_flowLayout = new FlowLayout(m_scrollWidget, false, true);

    initWidget();
}

void IconCardView::initWidget()
{
    m_scrollArea->setWidget(m_scrollWidget);
    m_scrollArea->setViewportMargins(0, 5, 0, 5);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxLayout->setSpacing(12);
    m_vBoxLayout->addWidget(m_iconLibraryLabel);
    m_vBoxLayout->addWidget(m_searchLineEdit);
    m_vBoxLayout->addWidget(m_view);

    m_hBoxLayout->setSpacing(0);
    m_hBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_hBoxLayout->addWidget(m_scrollArea);
    m_hBoxLayout->addWidget(m_infoPannel, 0, Qt::AlignRight);

    m_flowLayout->setVerticalSpacing(8);
    m_flowLayout->setHorizontalSpacing(8);
    m_flowLayout->setContentsMargins(8, 3, 8, 8);

    setQss();
    connect(GalleryConfig::cfg(), &GalleryConfig::themeChanged, this, &IconCardView::setQss);
    connect(m_searchLineEdit, &GalleryLineEdit::clearSignal, this, &IconCardView::showAllIcons);
    connect(m_searchLineEdit, &GalleryLineEdit::searchSignal, this, &IconCardView::search);

    QMetaEnum metaEnum = QMetaEnum::fromType<FluentIcon::IconType>();

    for (int i = 0; i <= metaEnum.keyCount(); i++) {
        FluentIcon::IconType type = (FluentIcon::IconType)metaEnum.value(i);
        FluentIcon *icon          = new FluentIcon(type);
        addIcon(icon);
    }
    if (m_icons.count() > 0) {
        setSelectedIcon(m_icons[0]);
    }
}

void IconCardView::addIcon(FluentIconBase *icon)
{
    IconCard *card = new IconCard(icon, this);
    connect(card, &IconCard::clicked, this, &IconCardView::setSelectedIcon);

    m_trie->insert(icon->typeName(), m_cards.length());
    m_cards.append(card);
    m_icons.append(icon);
    m_flowLayout->addWidget(card);
}

void IconCardView::setSelectedIcon(FluentIconBase *icon)
{
    int index = m_icons.indexOf(icon);

    if (m_currentIndex >= 0) {
        m_cards[m_currentIndex]->setSelected(false);
    }

    m_currentIndex = index;
    m_cards[index]->setSelected(true);
    m_infoPannel->setIcon(icon);
}

void IconCardView::search(QString keyWord)
{
    QMap<QString, int> items = m_trie->items(keyWord.toLower());
    QList<int> indexes       = items.values();

    for (int i = 0; i < m_cards.length(); i++) {
        m_cards[i]->setVisible(indexes.contains(i));
    }
}

void IconCardView::showAllIcons()
{
    for (IconCard *card : m_cards) {
        card->show();
    }
}

void IconCardView::setQss()
{
    m_view->setObjectName("iconView");
    m_scrollWidget->setObjectName("scrollWidget");
    m_iconLibraryLabel->setObjectName("iconLibraryLabel");

    GalleryStyleSheet::apply(GalleryStyleSheet::ICON_INTERFACE, this);

    if (m_currentIndex >= 0) {
        m_cards[m_currentIndex]->setSelected(true, true);
    }
}

IconInterface::IconInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->icons, "qfluentwidgets.common.icon", parent)
{
    m_iconView = new IconCardView(this);
    m_vBoxLayout->addWidget(m_iconView);
}
