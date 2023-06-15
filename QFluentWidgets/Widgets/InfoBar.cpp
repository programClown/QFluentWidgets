#include "InfoBar.h"
#include "Common/StyleSheet.h"
#include "QFluentWidgets.h"
#include "Common/AutoWrap.h"

#include <QPainter>
#include <QResizeEvent>
#include <QTimer>

InfoBarCloseButton::InfoBarCloseButton(QWidget *parent) : QToolButton(parent)
{
    setFixedSize(36, 36);
    setIconSize(QSize(12, 12));
    setCursor(Qt::PointingHandCursor);
    setObjectName("infoBarCloseButton");
    FluentStyleSheet::apply("INFO_BAR", this);
    m_ficon.reset(NEWFLICON(FluentIcon, CLOSE));
}

void InfoBarCloseButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_ficon->render(&painter, QRect(12, 12, 12, 12));
}

QString InfoBarIcon::iconName(InfoBarIcon::IconType type)
{
    switch (type) {
        case INFORMATION:
            return "Info";
        case SUCCESS:
            return "Success";
        case WARNING:
            return "Warning";
        case ERROR:
            return "Error";
    }

    return "Unknown";
}

InfoBarIcon::InfoBarIcon(IconType type, Qfw::Theme t) : FluentIconBase(""), m_theme(t), m_type(type)
{
    iconEngine->setIconPath(iconPath());
}

InfoBarIcon::~InfoBarIcon() { }

QString InfoBarIcon::iconPath()
{
    QString colorName;
    if (m_theme == Qfw::Theme::AUTO) {
        colorName = QFWIns.isDarkTheme() ? "dark" : "light";
    } else {
        colorName = Qfw::ThemeString(m_theme).toLower();
    }

    return QString(":/qfluentwidgets/images/info_bar/%1_%2.svg").arg(iconName(m_type)).arg(colorName);
}

QIcon InfoBarIcon::icon()
{
    return QIcon(iconEngine->clone());
}

QString InfoBarIcon::typeName() const
{
    return iconName(m_type);
}

QString InfoBarIcon::enumName() const
{
    QMetaEnum metaEnum = QMetaEnum::fromType<IconType>();
    return metaEnum.valueToKey(m_type);
}

FluentIconBase *InfoBarIcon::clone()
{
    return new InfoBarIcon(m_type, m_theme);
}

Qfw::Theme InfoBarIcon::theme() const
{
    return m_theme;
}

void InfoBarIcon::setTheme(const Qfw::Theme &theme)
{
    m_theme = theme;
    iconEngine->setIconPath(iconPath());
}

InfoIconWidget::InfoIconWidget(FluentIconBase *icon, QWidget *parent) : QWidget(parent), m_icon(icon)
{
    setFixedSize(15, 15);
}

void InfoIconWidget::paintEvent(QPaintEvent * /*event*/)
{
    if (!m_icon) {
        return;
    }
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    InfoBarIcon *barIcon = dynamic_cast<InfoBarIcon *>(m_icon.data());
    bool isInformation   = false;
    if (barIcon) {
        isInformation = barIcon->iconEngine->iconPath().split("/").last().contains("Info");
    }
    if (isInformation) {
        m_icon->render(&painter, rect());
    } else {
        QHash<QString, QString> attributes;
        attributes.insert("indexes", "0");
        attributes.insert("fill", themeColor().name());
        m_icon->render(&painter, rect(), QVector<int>(), attributes);
    }
}

InfoBar::InfoBar(FluentIconBase *ficon, const QString &title, const QString &content, Qt::Orientation orient,
                 bool isClosable, int duration, InfoBarPosition position, QWidget *parent)
    : QFrame(parent), m_ficon(ficon)
{
    m_title      = title;
    m_content    = content;
    m_orient     = orient;
    m_duration   = duration;
    m_isClosable = isClosable;
    m_position   = position;

    m_titleLabel   = new QLabel(title, this);
    m_contentLabel = new QLabel(this);
    m_closeButton  = new InfoBarCloseButton(this);
    m_iconWidget   = new InfoIconWidget(m_ficon->clone());

    m_vBoxLayout = new QVBoxLayout(this);
    m_hBoxLayout = new QHBoxLayout();
    if (m_orient == Qt::Horizontal) {
        m_contentLayout = new QHBoxLayout();
    } else {
        m_contentLayout = new QVBoxLayout();
    }
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityAni    = new QPropertyAnimation(m_opacityEffect, "opacity", this);

    m_lightBackgroundColor = QColor();
    m_darkBackgroundColor  = QColor();

    initWidget();
}

void InfoBar::adjustSize()
{
    QFrame::adjustSize();
    resize(width(), qMax(height(), 50));
}

/// add widget to info bar
void InfoBar::addWidget(QWidget *widget, int stretch)
{
    m_contentLayout->addWidget(widget, stretch, Qt::AlignLeft);
}

void InfoBar::setCustomBackgroundColor(const QColor &light, const QColor &dark)
{
    m_darkBackgroundColor  = dark;
    m_lightBackgroundColor = light;
    update();
}

InfoBar *InfoBar::creator(FluentIconBase *ficon, const QString &title, const QString &content, Qt::Orientation orient,
                          bool isClosable, int duration, InfoBarPosition position, QWidget *parent)
{
    InfoBar *cls = new InfoBar(ficon, title, content, orient, isClosable, duration, position, parent);
    cls->setAttribute(Qt::WA_DeleteOnClose);
    cls->show();
    return cls;
}

InfoBar *InfoBar::info(const QString &title, const QString &content, Qt::Orientation orient, bool isClosable,
                       int duration, InfoBarPosition position, QWidget *parent)
{
    return creator(NEWFLICON(InfoBarIcon, INFORMATION), title, content, orient, isClosable, duration, position, parent);
}

InfoBar *InfoBar::success(const QString &title, const QString &content, Qt::Orientation orient, bool isClosable,
                          int duration, InfoBarPosition position, QWidget *parent)
{
    return creator(NEWFLICON(InfoBarIcon, SUCCESS), title, content, orient, isClosable, duration, position, parent);
}

InfoBar *InfoBar::warning(const QString &title, const QString &content, Qt::Orientation orient, bool isClosable,
                          int duration, InfoBarPosition position, QWidget *parent)
{
    return creator(NEWFLICON(InfoBarIcon, WARNING), title, content, orient, isClosable, duration, position, parent);
}

InfoBar *InfoBar::error(const QString &title, const QString &content, Qt::Orientation orient, bool isClosable,
                        int duration, InfoBarPosition position, QWidget *parent)
{
    return creator(NEWFLICON(InfoBarIcon, ERROR), title, content, orient, isClosable, duration, position, parent);
}

void InfoBar::initWidget()
{
    m_titleLabel->setMinimumHeight(36);
    m_opacityEffect->setOpacity(1);
    setGraphicsEffect(m_opacityEffect);
    m_closeButton->setVisible(m_isClosable);

    initLayout();
    setQss();

    connect(m_closeButton, &InfoBarCloseButton::clicked, this, &InfoBar::close);
}

void InfoBar::setQss()
{
    m_titleLabel->setObjectName("titleLabel");
    m_contentLabel->setObjectName("contentLabel");

    InfoBarIcon *barIcon = dynamic_cast<InfoBarIcon *>(m_ficon.data());
    if (barIcon) {
        QString typeName = barIcon->iconEngine->iconPath().split("/").last().split("_").first();
        setProperty("type", typeName);
    } else {
        FluentIcon *fluentIcon = dynamic_cast<FluentIcon *>(m_ficon.data());
        if (fluentIcon) {
            QString typeName = fluentIcon->iconEngine->iconPath().split("/").last().split("_").first();
            setProperty("type", typeName);
        }
    }

    FluentStyleSheet::apply("INFO_BAR", this);
}

void InfoBar::initLayout()
{
    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);
    m_contentLayout->setSizeConstraint(QHBoxLayout::SetMinimumSize);

    m_vBoxLayout->setSpacing(0);
    m_hBoxLayout->setSpacing(0);

    m_vBoxLayout->addLayout(m_hBoxLayout);
    m_hBoxLayout->addWidget(m_iconWidget);

    if (!m_title.isEmpty()) {
        m_hBoxLayout->addSpacing(15);
        m_hBoxLayout->addWidget(m_titleLabel);
    } else {
        m_titleLabel->hide();
    }

    // add content label to layout
    if (m_orient == Qt::Horizontal) {
        m_vBoxLayout->setAlignment(Qt::AlignVCenter);
        m_hBoxLayout->addSpacing(12);
        m_contentLayout->setContentsMargins(0, 0, 20 * (m_isClosable ? 1 : 0), 0);
        m_contentLayout->setSpacing(12);
        m_hBoxLayout->addLayout(m_contentLayout);
    } else {
        m_vBoxLayout->setAlignment(Qt::AlignTop);
        m_contentLayout->setContentsMargins(47, 0, 40, 18);
        m_contentLayout->setSpacing(14);
        m_vBoxLayout->addLayout(m_contentLayout);
        m_contentLayout->setAlignment(Qt::AlignTop);
    }

    adjustText();
    m_contentLayout->addWidget(m_contentLabel);

    // add close button to layout
    if (m_isClosable) {
        m_hBoxLayout->addWidget(m_closeButton);
        int mb = (m_orient == Qt::Horizontal) ? 6 : 0;
        m_hBoxLayout->setContentsMargins(16, 6, 7, mb);
    } else {
        m_hBoxLayout->setContentsMargins(16, 6, 16, 6);
    }
}

void InfoBar::adjustText()
{
    int w = 0;
    if (!this->parent()) {
        w = 900;
    } else {
        w = static_cast<QWidget *>(this->parent())->width() - 50;
    }

    int chars = qMax(qMin(w / 9, 120), 30);
    m_contentLabel->setText(TextWrap::wrap(m_content, chars, false).first);

    this->adjustSize();
}

bool InfoBar::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->parent()) {
        if (event->type() == QEvent::Resize || event->type() == QEvent::WindowStateChange) {
            adjustText();
        }
    }

    return QFrame::eventFilter(watched, event);
}

void InfoBar::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    if (!m_lightBackgroundColor.isValid()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    if (QFWIns.isDarkTheme()) {
        painter.setBrush(m_darkBackgroundColor);
    } else {
        painter.setBrush(m_lightBackgroundColor);
    }

    QRect rect = this->rect().adjusted(1, 1, -1, -1);
    painter.drawRoundedRect(rect, 6, 6);
}

void InfoBar::closeEvent(QCloseEvent * /*event*/)
{
    emit closedSignal();
    this->deleteLater();
}

void InfoBar::showEvent(QShowEvent *event)
{
    adjustText();
    QFrame::showEvent(event);

    if (m_duration >= 0) {
        QTimer::singleShot(m_duration, this, &InfoBar::fadeOut);
    }

    if (m_position != InfoBarPosition::NONE) {
        InfoBarManagerFactory::factory(m_position).add(this);
    }

    if (this->parent()) {
        this->parent()->installEventFilter(this);
    }
}

void InfoBar::fadeOut()
{
    m_opacityAni->setDuration(200);
    m_opacityAni->setStartValue(1);
    m_opacityAni->setEndValue(0);
    connect(m_opacityAni, &QPropertyAnimation::finished, this, &InfoBar::close);
    m_opacityAni->start();
}

InfoBarManagerBase::InfoBarManagerBase(QObject *parent) : QObject(parent)
{
    spacing = 16;
    margin  = 24;
}

void InfoBarManagerBase::add(InfoBar *infoBar)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return;
    }

    if (!infoBars.keys().contains(p)) {
        p->installEventFilter(this);
        infoBars.insert(p, {});
        aniGroups.insert(p, new QParallelAnimationGroup(this));
    }

    if (infoBars[p].contains(infoBar)) {
        return;
    }

    // add drop animation
    if (!infoBars[p].isEmpty()) {
        QPropertyAnimation *dropAni = new QPropertyAnimation(infoBar, "pos");
        dropAni->setDuration(200);

        aniGroups[p]->addAnimation(dropAni);
        dropAnis.append(dropAni);

        infoBar->setProperty("dropAni", QVariant::fromValue<QPropertyAnimation *>(dropAni));
    }

    // add slide animation
    infoBars[p].append(infoBar);
    QPropertyAnimation *slideAni = createSlideAni(infoBar);
    slideAnis.append(slideAni);

    infoBar->setProperty("slideAni", QVariant::fromValue<QPropertyAnimation *>(slideAni));
    connect(infoBar, &InfoBar::closedSignal, [=]() { remove(infoBar); });

    slideAni->start();
}

void InfoBarManagerBase::remove(InfoBar *infoBar)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return;
    }

    if (!infoBars.keys().contains(p)) {
        return;
    }

    infoBars[p].removeAll(infoBar);

    // remove drop animation
    QPropertyAnimation *dropAni = infoBar->property("dropAni").value<QPropertyAnimation *>();
    if (dropAni) {
        aniGroups[p]->removeAnimation(dropAni);
        dropAnis.removeAll(dropAni);
        dropAni->deleteLater();
    }

    // remove slider animation
    QPropertyAnimation *slideAni = infoBar->property("slideAni").value<QPropertyAnimation *>();
    if (slideAni) {
        slideAnis.removeAll(slideAni);
        slideAni->deleteLater();
    }

    infoBar->deleteLater();
    // adjust the position of the remaining info bars
    updateDropAni(p);
    aniGroups[p]->start();
}

QPropertyAnimation *InfoBarManagerBase::createSlideAni(InfoBar *infoBar)
{
    QPropertyAnimation *slideAni = new QPropertyAnimation(infoBar, "pos");
    slideAni->setEasingCurve(QEasingCurve::OutQuad);
    slideAni->setDuration(200);

    slideAni->setStartValue(this->slideStartPos(infoBar));
    slideAni->setEndValue(this->pos(infoBar));

    return slideAni;
}

void InfoBarManagerBase::updateDropAni(QWidget *parent)
{
    for (auto bar : infoBars[parent]) {
        QPropertyAnimation *ani = bar->property("dropAni").value<QPropertyAnimation *>();
        if (!ani) {
            continue;
        }
        ani->setStartValue(bar->pos());
        ani->setEndValue(this->pos(bar));
    }
}

bool InfoBarManagerBase::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *obj = static_cast<QWidget *>(watched);
    if (!obj || !infoBars.keys().contains(obj)) {
        return false;
    }

    if (event->type() == QEvent::Resize || event->type() == QEvent::WindowStateChange) {
        QSize size = (event->type() == QEvent::Resize) ? static_cast<QResizeEvent *>(event)->size() : QSize();

        for (auto bar : infoBars[obj]) {
            bar->move(this->pos(bar, size));
        }
    }

    return QObject::eventFilter(watched, event);
}

TopInfoBarManager::TopInfoBarManager(QObject *parent) : InfoBarManagerBase(parent) { }

QPoint TopInfoBarManager::pos(InfoBar *infoBar, const QSize & /*parentSize*/)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return QPoint();
    }

    //    QSize size = (parentSize.isValid()) ? parentSize : p->size();
    int x     = (p->width() - infoBar->width()) / 2;
    int y     = this->margin;
    int index = this->infoBars[p].indexOf(infoBar);

    for (int i = 0; i < index; ++i) {
        y += this->infoBars[p].at(i)->height() + this->spacing;
    }

    return QPoint(x, y);
}

QPoint TopInfoBarManager::slideStartPos(InfoBar *infoBar)
{
    QPoint pos = this->pos(infoBar);
    return QPoint(pos.x(), pos.y() - 16);
}

TopRightInfoBarManager::TopRightInfoBarManager(QObject *parent) : InfoBarManagerBase(parent) { }

QPoint TopRightInfoBarManager::pos(InfoBar *infoBar, const QSize &parentSize)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return QPoint();
    }

    QSize size = (parentSize.isValid()) ? parentSize : p->size();

    int x     = size.width() - infoBar->width() - this->margin;
    int y     = this->margin;
    int index = this->infoBars[p].indexOf(infoBar);

    for (int i = 0; i < index; ++i) {
        y += this->infoBars[p].at(i)->height() + this->spacing;
    }

    return QPoint(x, y);
}

QPoint TopRightInfoBarManager::slideStartPos(InfoBar *infoBar)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return QPoint();
    }

    return QPoint(p->width(), this->pos(infoBar).y());
}

BottomRightInfoBarManager::BottomRightInfoBarManager(QObject *parent) : InfoBarManagerBase(parent) { }

QPoint BottomRightInfoBarManager::pos(InfoBar *infoBar, const QSize &parentSize)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return QPoint();
    }

    QSize size = (parentSize.isValid()) ? parentSize : p->size();

    int x     = size.width() - infoBar->width() - this->margin;
    int y     = size.height() - infoBar->height() - this->margin;
    int index = this->infoBars[p].indexOf(infoBar);

    for (int i = 0; i < index; ++i) {
        y -= this->infoBars[p].at(i)->height() + this->spacing;
    }

    return QPoint(x, y);
}

QPoint BottomRightInfoBarManager::slideStartPos(InfoBar *infoBar)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return QPoint();
    }

    return QPoint(p->width(), this->pos(infoBar).y());
}

TopLeftInfoBarManager::TopLeftInfoBarManager(QObject *parent) : InfoBarManagerBase(parent) { }

QPoint TopLeftInfoBarManager::pos(InfoBar *infoBar, const QSize & /*parentSize*/)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return QPoint();
    }

    //    QSize size = (parentSize.isValid()) ? parentSize : p->size();
    int y     = this->margin;
    int index = this->infoBars[p].indexOf(infoBar);

    for (int i = 0; i < index; ++i) {
        y += this->infoBars[p].at(i)->height() + this->spacing;
    }

    return QPoint(this->margin, y);
}

QPoint TopLeftInfoBarManager::slideStartPos(InfoBar *infoBar)
{
    return QPoint(-infoBar->width(), this->pos(infoBar).y());
}

BottomLeftInfoBarManager::BottomLeftInfoBarManager(QObject *parent) : InfoBarManagerBase(parent) { }

QPoint BottomLeftInfoBarManager::pos(InfoBar *infoBar, const QSize &parentSize)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return QPoint();
    }

    QSize size = (parentSize.isValid()) ? parentSize : p->size();
    int y      = size.height() - infoBar->height() - this->margin;
    int index  = this->infoBars[p].indexOf(infoBar);

    for (int i = 0; i < index; ++i) {
        y -= this->infoBars[p].at(i)->height() + this->spacing;
    }

    return QPoint(this->margin, y);
}

QPoint BottomLeftInfoBarManager::slideStartPos(InfoBar *infoBar)
{
    return QPoint(-infoBar->width(), this->pos(infoBar).y());
}

BottomInfoBarManager::BottomInfoBarManager(QObject *parent) : InfoBarManagerBase(parent) { }

QPoint BottomInfoBarManager::pos(InfoBar *infoBar, const QSize &parentSize)
{
    QWidget *p = static_cast<QWidget *>(infoBar->parent());
    if (!p) {
        return QPoint();
    }

    QSize size = (parentSize.isValid()) ? parentSize : p->size();

    int x     = (size.width() - infoBar->width()) / 2;
    int y     = size.height() - infoBar->height() - this->margin;
    int index = this->infoBars[p].indexOf(infoBar);
    for (int i = 0; i < index; ++i) {
        y -= this->infoBars[p].at(i)->height() + this->spacing;
    }

    return QPoint(x, y);
}

QPoint BottomInfoBarManager::slideStartPos(InfoBar *infoBar)
{
    QPoint pos = this->pos(infoBar);
    return QPoint(pos.x(), pos.y() + 16);
}

InfoBarManagerBase &InfoBarManagerFactory::factory(InfoBarPosition postion)
{
    static TopInfoBarManager topInfoBarManager;
    static BottomInfoBarManager bottomInfoBarManager;
    static TopRightInfoBarManager topRightInfoBarManager;
    static BottomRightInfoBarManager bottomRightInfoBarManager;
    static TopLeftInfoBarManager topLeftInfoBarManager;
    static BottomLeftInfoBarManager bottomLeftInfoBarManager;

    switch (postion) {
        case InfoBarPosition::TOP:
            return topInfoBarManager;
        case InfoBarPosition::BOTTOM:
            return bottomInfoBarManager;
        case InfoBarPosition::TOP_RIGHT:
            return topRightInfoBarManager;
        case InfoBarPosition::BOTTOM_RIGHT:
            return bottomRightInfoBarManager;
        case InfoBarPosition::TOP_LEFT:
            return topLeftInfoBarManager;
        case InfoBarPosition::BOTTOM_LEFT:
            return bottomLeftInfoBarManager;

        default:
            return topInfoBarManager;
    }
}
