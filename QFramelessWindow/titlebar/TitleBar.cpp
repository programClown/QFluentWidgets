#include "TitleBar.h"
#include "TitleBarButton.h"

#include <QEvent>
#include <QMouseEvent>

#if defined(Q_OS_WIN32)
    #include "utils/Win32Utils.h"
#elif defined(Q_OS_OSX)
    #include "utils/macUtils.h"
#else

#endif

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    minBtn   = new MinimizeButton(this);
    closeBtn = new CloseButton(this);
    maxBtn   = new MaximizeButton(this);

    isDoubleClickEnabled = true;

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    setLayout(hBoxLayout);

    hBoxLayout->setSpacing(0);
    hBoxLayout->setMargin(0);
    hBoxLayout->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
    hBoxLayout->addStretch(1);
    hBoxLayout->addWidget(minBtn, 0, Qt::AlignRight);
    hBoxLayout->addWidget(maxBtn, 0, Qt::AlignRight);
    hBoxLayout->addWidget(closeBtn, 0, Qt::AlignRight);

    resize(200, 32);
    setFixedHeight(32);

    connect(minBtn, &MinimizeButton::clicked, this->window(), &QWidget::showMinimized);
    connect(maxBtn, &MaximizeButton::clicked, this, &TitleBar::onToggleMaxState);
    connect(closeBtn, &CloseButton::clicked, this->window(), &QWidget::close);

    this->window()->installEventFilter(this);
}

MinimizeButton *TitleBar::minimizeButton() const
{
    return minBtn;
}

CloseButton *TitleBar::closeButton() const
{
    return closeBtn;
}

MaximizeButton *TitleBar::maximizeButton() const
{
    return maxBtn;
}

void TitleBar::onToggleMaxState()
{
    if (this->window()->isMaximized()) {
        this->window()->showNormal();
    } else {
        this->window()->showMaximized();
    }
}

bool TitleBar::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->window()) {
        if (event->type() == QEvent::WindowStateChange) {
            maxBtn->setMaxState(this->window()->isMaximized());
            return false;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if ((event->button() != Qt::LeftButton) || !isDoubleClickEnabled) {
        return;
    }

    onToggleMaxState();
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
#if defined(Q_OS_WIN32)
    Win32Utils::WindowsMoveResize::startSystemMove(this->window(), event->globalPos());
#elif defined(Q_OS_OSX)
    #include "utils/macUtils.h"
#else

#endif
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
#if defined(Q_OS_WIN32)
    Win32Utils::WindowsMoveResize::startSystemMove(this->window(), event->globalPos());
#elif defined(Q_OS_OSX)
    #include "utils/macUtils.h"
#else

#endif
}

void TitleBar::setDoubleClickEnabled(bool isEnabled)
{
    isDoubleClickEnabled = isEnabled;
}

bool TitleBar::canDrag(QPoint pos)
{
    return isDragRegion(pos) && (!hasButtonPressed());
}

bool TitleBar::isDragRegion(QPoint pos)
{
    int width = 0;

    for (auto button : this->findChildren<TitleBarButton *>()) {
        if (button->isVisible())
            width += button->width();
    }

    return (pos.x() > 0) && (pos.x() < this->width() - width);
}

bool TitleBar::hasButtonPressed()
{
    for (auto button : this->findChildren<TitleBarButton *>()) {
        if (button->isPressed())
            return true;
    }

    return false;
}

StandardTitleBar::StandardTitleBar(QWidget *parent) : TitleBar(parent)
{
    // add window icon
    iconLabel = new QLabel(this);
    iconLabel->setFixedSize(20, 20);
    hBoxLayout->insertSpacing(0, 10);
    hBoxLayout->insertWidget(1, iconLabel, 0, Qt::AlignLeft);

    // add title label
    titleLabel = new QLabel(this);
    hBoxLayout->insertWidget(2, titleLabel, 0, Qt::AlignLeft);
    titleLabel->setStyleSheet("QLabel{"
                              "background: transparent;"
                              "font: 13px 'Segoe UI';"
                              "padding: 0 4px}");

    connect(this->window(), &QWidget::windowTitleChanged, this, &StandardTitleBar::setTitle);
}

/// set the title of title bar
void StandardTitleBar::setTitle(const QString &title)
{
    titleLabel->setText(title);
    titleLabel->adjustSize();
}

/// set the icon of title bar
void StandardTitleBar::setIcon(const QIcon &icon)
{
    iconLabel->setPixmap(icon.pixmap(20, 20));
}
