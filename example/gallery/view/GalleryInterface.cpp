#include "GalleryInterface.h"

#include "common/GalleryConfig.h"
#include "common/GalleryStyleSheet.h"

#include <Widgets/Button.h>
#include <Common/Icon.h>
#include <Widgets/ToolTip.h>
#include <Widgets/IconWidget.h>

#include <QLabel>
#include <QBoxLayout>
#include <QDesktopServices>
#include <QEvent>

ToolBar::ToolBar(QString title, QString subtitle, QWidget *parent) : QWidget(parent)
{
    m_titleLabel    = new QLabel(title, this);
    m_subtitleLabel = new QLabel(subtitle, this);

    m_documentButton = new PushButton(tr("Documentation"), NEWFLICON(FluentIcon, DOCUMENT), this);
    m_sourceButton   = new PushButton(tr("Source"), NEWFLICON(FluentIcon, GITHUB), this);
    m_themeButton    = new ToolButton(NEWFLICON(FluentIcon, CONSTRACT), this);
    m_feedbackButton = new ToolButton(NEWFLICON(FluentIcon, FEEDBACK), this);

    m_vBoxLayout   = new QVBoxLayout(this);
    m_buttonLayout = new QHBoxLayout();

    initWidget();
}

void ToolBar::initWidget()
{
    setFixedHeight(138);
    m_vBoxLayout->setSpacing(0);
    m_vBoxLayout->setContentsMargins(36, 22, 36, 12);
    m_vBoxLayout->addWidget(m_titleLabel);
    m_vBoxLayout->addSpacing(4);
    m_vBoxLayout->addWidget(m_subtitleLabel);
    m_vBoxLayout->addSpacing(4);
    m_vBoxLayout->addLayout(m_buttonLayout, 1);
    m_vBoxLayout->setAlignment(Qt::AlignTop);

    m_buttonLayout->setSpacing(4);
    m_buttonLayout->setContentsMargins(0, 0, 0, 0);
    m_buttonLayout->addWidget(m_documentButton, 1, Qt::AlignLeft);
    m_buttonLayout->addWidget(m_sourceButton, 1, Qt::AlignLeft);
    m_buttonLayout->addStretch(1);
    m_buttonLayout->addWidget(m_themeButton, 1, Qt::AlignRight);
    m_buttonLayout->addWidget(m_feedbackButton, 1, Qt::AlignRight);
    m_buttonLayout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    m_themeButton->installEventFilter(new ToolTipFilter(m_themeButton));
    m_feedbackButton->installEventFilter(new ToolTipFilter(m_feedbackButton));
    m_themeButton->setToolTip(tr("Toggle theme"));
    m_feedbackButton->setToolTip(tr("Send feedback"));

    m_titleLabel->setObjectName("titleLabel");
    m_subtitleLabel->setObjectName("subtitleLabel");

    connect(m_themeButton, &ToolButton::clicked, this, &ToolBar::toggleTheme);
    connect(m_documentButton, &PushButton::clicked, []() { QDesktopServices::openUrl(QUrl(HELP_URL)); });
    connect(m_sourceButton, &PushButton::clicked, []() { QDesktopServices::openUrl(QUrl(EXAMPLE_URL)); });
    connect(m_feedbackButton, &ToolButton::clicked, []() { QDesktopServices::openUrl(QUrl(FEEDBACK_URL)); });
}

void ToolBar::toggleTheme() { }

ExampleCard::ExampleCard(const QString title, QWidget *widget, QString sourcePath, int stretch, QWidget *parent)
    : QWidget(parent)
{
    m_widget  = widget;
    m_stretch = stretch;

    m_titleLabel = new QLabel(title, this);
    m_card       = new QFrame(this);

    m_sourceWidget    = new QFrame(m_card);
    m_sourcePath      = sourcePath;
    m_sourcePathLabel = new QLabel(tr("Source code"), m_sourceWidget);
    m_linkIcon        = new IconWidget(NEWFLICON(FluentIcon, LINK), m_sourceWidget);

    m_vBoxLayout   = new QVBoxLayout(this);
    m_cardLayout   = new QVBoxLayout(m_card);
    m_topLayout    = new QHBoxLayout();
    m_bottomLayout = new QHBoxLayout(m_sourceWidget);

    initWidget();
}

void ExampleCard::initWidget()
{
    setFixedSize(16, 16);
    initLayout();

    m_sourceWidget->setCursor(Qt::PointingHandCursor);
    m_sourceWidget->installEventFilter(this);

    m_titleLabel->setObjectName("titleLabel");
    m_card->setObjectName("card");
    m_sourcePathLabel->setObjectName("sourcePathLabel");
    m_sourceWidget->setObjectName("sourceWidget");
}

void ExampleCard::initLayout()
{
    m_vBoxLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);
    m_cardLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);
    m_topLayout->setSizeConstraint(QHBoxLayout::SetMinimumSize);

    m_vBoxLayout->setSpacing(12);
    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_topLayout->setContentsMargins(12, 12, 12, 12);
    m_bottomLayout->setContentsMargins(18, 18, 18, 18);
    m_cardLayout->setContentsMargins(0, 0, 0, 0);

    m_vBoxLayout->addWidget(m_titleLabel, 0, Qt::AlignTop);
    m_vBoxLayout->addWidget(m_card, 0, Qt::AlignTop);
    m_vBoxLayout->setAlignment(Qt::AlignTop);

    m_cardLayout->setSpacing(0);
    m_cardLayout->setAlignment(Qt::AlignTop);
    m_cardLayout->addLayout(m_topLayout, 0);
    m_cardLayout->addWidget(m_sourceWidget, 0, Qt::AlignBottom);

    m_widget->setParent(m_card);
    m_topLayout->addWidget(m_widget);
    if (m_stretch == 0) {
        m_topLayout->addStretch(1);
    }
    m_widget->show();

    m_bottomLayout->addWidget(m_sourcePathLabel, 0, Qt::AlignLeft);
    m_bottomLayout->addStretch(1);
    m_bottomLayout->addWidget(m_linkIcon, 0, Qt::AlignLeft);
    m_bottomLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

bool ExampleCard::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == m_sourceWidget) {
        if (e->type() == QEvent::MouseButtonRelease) {
            QDesktopServices::openUrl(QUrl(m_sourcePath));
        }
    }

    return QWidget::eventFilter(obj, e);
}

QFrame *ExampleCard::card() const
{
    return m_card;
}

GalleryInterface::GalleryInterface(QString title, QString subtitle, QWidget *parent) : ScrollArea(parent)
{
    m_view       = new QWidget(this);
    m_toolBar    = new ToolBar(title, subtitle, this);
    m_vBoxLayout = new QVBoxLayout(m_view);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewportMargins(0, m_toolBar->height(), 0, 0);
    setWidget(m_view);
    setWidgetResizable(true);

    m_vBoxLayout->setSpacing(0);
    m_vBoxLayout->setAlignment(Qt::AlignTop);
    m_vBoxLayout->setContentsMargins(36, 20, 36, 36);

    m_view->setObjectName("view");
    GalleryStyleSheet::apply(GalleryStyleSheet::GALLERY_INTERFACE, this);
}

ExampleCard *GalleryInterface::addExampleCard(QString title, QWidget *widget, QString sourcePath, int stretch)
{
    ExampleCard *card = new ExampleCard(title, widget, sourcePath, stretch, m_view);
    m_vBoxLayout->addWidget(card, 0, Qt::AlignTop);
    return card;
}

void GalleryInterface::scrollToCard(int index)
{
    QWidget *w = m_vBoxLayout->itemAt(index)->widget();
    verticalScrollBar()->setValue(w->y());
}

void GalleryInterface::resizeEvent(QResizeEvent *e)
{
    ScrollArea::resizeEvent(e);
    m_toolBar->resize(width(), m_toolBar->height());
}
