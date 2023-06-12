#include "SpinBox.h"
#include "QFluentWidgets.h"
#include "Menu.h"

#include <QPainter>

QString SpinIcon::iconName(SpinIcon::IconType type)
{
    switch (type) {
        case UP:
            return "Up";
        case DOWN:
            return "Down";
    }

    return "Unknown";
}

SpinIcon::SpinIcon(IconType type, Qfw::Theme t) : FluentIconBase(""), m_theme(t), m_type(type)
{
    iconEngine->setIconPath(iconPath());
}

SpinIcon::~SpinIcon() { }

QString SpinIcon::iconPath()
{
    QString colorName;
    if (m_theme == Qfw::Theme::AUTO) {
        colorName = QFWIns.isDarkTheme() ? "white" : "black";
    } else {
        if (m_theme == Qfw::DARK) {
            colorName = "white";
        } else {
            colorName = "black";
        }
    }

    return QString(":/qfluentwidgets/images/spin_box/%1_%2.svg").arg(iconName(m_type)).arg(colorName);
}

QIcon SpinIcon::icon()
{
    return QIcon(iconEngine->clone());
}

QString SpinIcon::typeName() const
{
    return iconName(m_type);
}

FluentIconBase *SpinIcon::clone()
{
    return new SpinIcon(m_type, m_theme);
}

Qfw::Theme SpinIcon::theme() const
{
    return m_theme;
}

void SpinIcon::setTheme(const Qfw::Theme &theme)
{
    m_theme = theme;
    iconEngine->setIconPath(iconPath());
}

SpinButton::SpinButton(FluentIconBase *icon, QWidget *parent) : QToolButton(parent), m_icon(icon)
{
    setFixedSize(31, 23);
    setIconSize(QSize(10, 10));
    FluentStyleSheet::apply("SPIN_BOX", this);
}

void SpinButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    m_icon->render(&painter, QRect(10, 9, 11, 11));
}

void SpinBoxBase::setUpUi(QAbstractSpinBox *spinbox)
{
    FluentStyleSheet::apply("SPIN_BOX", spinbox);
    spinbox->setButtonSymbols(QSpinBox::NoButtons);
    spinbox->setFixedHeight(33);

    hBoxLayout = new QHBoxLayout(spinbox);
    upButton   = new SpinButton(NEWFLICON(SpinIcon, UP), spinbox);
    downButton = new SpinButton(NEWFLICON(SpinIcon, DOWN), spinbox);

    hBoxLayout->setContentsMargins(0, 4, 4, 4);
    hBoxLayout->setSpacing(5);
    hBoxLayout->addWidget(upButton, 0, Qt::AlignRight);
    hBoxLayout->addWidget(downButton, 0, Qt::AlignRight);
    hBoxLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // connect(upButton, &SpinButton::clicked, m_parent, &QAbstractSpinBox::stepUp);
    // connect(downButton, &SpinButton::clicked, m_parent, &QAbstractSpinBox::stepDown);

    spinbox->setAttribute(Qt::WA_MacShowFocusRect, false);
    spinbox->setContextMenuPolicy(Qt::CustomContextMenu);
    // connect(m_parent, &QAbstractSpinBox::customContextMenuRequested, this, &SpinBoxBase::showContextMenu);
}

void SpinBoxBase::showContextMenu(QAbstractSpinBox *spinbox, const QPoint &pos)
{
    QScopedPointer<LineEditMenu> menu(new LineEditMenu(lineEdit()));
    menu->exec(spinbox->mapToGlobal(pos));
}

void SpinBoxBase::drawBorderBottom(QAbstractSpinBox *spinbox)
{
    if (!spinbox->hasFocus()) {
        return;
    }

    QPainter painter(spinbox);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    int w = spinbox->width();
    int h = spinbox->height();
    path.addRoundedRect(QRectF(0, h - 10, w, 10), 5, 5);

    QPainterPath rectPath;
    rectPath.addRect(0, h - 10, w, 8);
    path = path.subtracted(rectPath);

    painter.fillPath(path, themeColor());
}

SpinBox::SpinBox(QWidget *parent) : QSpinBox(parent), SpinBoxBase()
{
    setUpUi(this);
    connect(upButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepUp);
    connect(downButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepDown);
    connect(this, &QAbstractSpinBox::customContextMenuRequested, this,
            [this](const QPoint &pos) { showContextMenu(this, pos); });
}

QLineEdit *SpinBox::lineEdit() const
{
    return QSpinBox::lineEdit();
}

void SpinBox::paintEvent(QPaintEvent *event)
{
    QSpinBox::paintEvent(event);
    drawBorderBottom(this);
}

DoubleSpinBox::DoubleSpinBox(QWidget *parent) : QDoubleSpinBox(parent), SpinBoxBase()
{
    setUpUi(this);
    connect(upButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepUp);
    connect(downButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepDown);
    connect(this, &QAbstractSpinBox::customContextMenuRequested, this,
            [this](const QPoint &pos) { showContextMenu(this, pos); });
}

QLineEdit *DoubleSpinBox::lineEdit() const
{
    return QDoubleSpinBox::lineEdit();
}

void DoubleSpinBox::paintEvent(QPaintEvent *event)
{
    QDoubleSpinBox::paintEvent(event);
    drawBorderBottom(this);
}

TimeEdit::TimeEdit(QWidget *parent) : QTimeEdit(parent), SpinBoxBase()
{
    setUpUi(this);
    connect(upButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepUp);
    connect(downButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepDown);
    connect(this, &QAbstractSpinBox::customContextMenuRequested, this,
            [this](const QPoint &pos) { showContextMenu(this, pos); });
}

QLineEdit *TimeEdit::lineEdit() const
{
    return QTimeEdit::lineEdit();
}

void TimeEdit::paintEvent(QPaintEvent *event)
{
    QTimeEdit::paintEvent(event);
    drawBorderBottom(this);
}

DateTimeEdit::DateTimeEdit(QWidget *parent) : QDateTimeEdit(parent), SpinBoxBase()
{
    setUpUi(this);
    connect(upButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepUp);
    connect(downButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepDown);
    connect(this, &QAbstractSpinBox::customContextMenuRequested, this,
            [this](const QPoint &pos) { showContextMenu(this, pos); });
}

QLineEdit *DateTimeEdit::lineEdit() const
{
    return QDateTimeEdit::lineEdit();
}

void DateTimeEdit::paintEvent(QPaintEvent *event)
{
    QDateTimeEdit::paintEvent(event);
    drawBorderBottom(this);
}

DateEdit::DateEdit(QWidget *parent) : QDateEdit(parent), SpinBoxBase()
{
    setUpUi(this);
    connect(upButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepUp);
    connect(downButton, &SpinButton::clicked, this, &QAbstractSpinBox::stepDown);
    connect(this, &QAbstractSpinBox::customContextMenuRequested, this,
            [this](const QPoint &pos) { showContextMenu(this, pos); });
}

QLineEdit *DateEdit::lineEdit() const
{
    return QDateEdit::lineEdit();
}

void DateEdit::paintEvent(QPaintEvent *event)
{
    QDateEdit::paintEvent(event);
    drawBorderBottom(this);
}
