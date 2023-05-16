#include "SpinBox.h"
#include "QFluentWidgets.h"
#include "Menu.h"

#include <QPainter>

SpinIcon::SpinIcon(SpinIcon::Type type, Qfw::Theme t) : m_type(type), m_theme(t)
{
    switch (type) {
        case UP:
            m_name = "Up";
            break;
        case DOWN:
            m_name = "Down";
            break;
    }
}

QString SpinIcon::path()
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

    return QString(":/qfluentwidgets/images/spin_box/%1_%2.svg").arg(m_name).arg(colorName);
}

QIcon SpinIcon::icon()
{
    return QIcon(path());
}

void SpinIcon::setTheme(const Qfw::Theme &theme)
{
    m_theme = theme;
}

SpinIcon::Type SpinIcon::type() const
{
    return m_type;
}

QString SpinIcon::typeName() const
{
    return m_name;
}

SpinButton::SpinButton(SpinIconSPtr icon, QWidget *parent) : QToolButton(parent), m_icon(icon)
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

void SpinBoxBase::setUpUi()
{
    FluentStyleSheet::apply("SPIN_BOX", m_parent);
    m_parent->setButtonSymbols(QSpinBox::NoButtons);
    m_parent->setFixedHeight(33);

    hBoxLayout = new QHBoxLayout(m_parent);
    upButton   = new SpinButton(SpinIconSPtr(new SpinIcon(SpinIcon::UP)), m_parent);
    downButton = new SpinButton(SpinIconSPtr(new SpinIcon(SpinIcon::DOWN)), m_parent);

    hBoxLayout->setContentsMargins(0, 4, 4, 4);
    hBoxLayout->setSpacing(5);
    hBoxLayout->addWidget(upButton, 0, Qt::AlignRight);
    hBoxLayout->addWidget(downButton, 0, Qt::AlignRight);
    hBoxLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    connect(upButton, &SpinButton::clicked, m_parent, &QAbstractSpinBox::stepUp);
    connect(downButton, &SpinButton::clicked, m_parent, &QAbstractSpinBox::stepDown);

    m_parent->setAttribute(Qt::WA_MacShowFocusRect, false);
    m_parent->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_parent, &QAbstractSpinBox::customContextMenuRequested, this, &SpinBoxBase::showContextMenu);
}

void SpinBoxBase::showContextMenu(const QPoint &pos)
{
    QScopedPointer<LineEditMenu> menu(new LineEditMenu(lineEdit()));
    menu->exec(m_parent->mapToGlobal(pos));
}

void SpinBoxBase::drawBorderBottom()
{
    if (!m_parent->hasFocus()) {
        return;
    }

    QPainter painter(m_parent);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    int w = m_parent->width();
    int h = m_parent->height();
    path.addRoundedRect(QRectF(0, h - 10, w, 10), 5, 5);

    QPainterPath rectPath;
    rectPath.addRect(0, h - 10, w, 8);
    path = path.subtracted(rectPath);

    painter.fillPath(path, themeColor());
}

SpinBox::SpinBox(QWidget *parent) : QSpinBox(parent), SpinBoxBase(this)
{
    setUpUi();
}

QLineEdit *SpinBox::lineEdit() const
{
    return QSpinBox::lineEdit();
}

void SpinBox::paintEvent(QPaintEvent *event)
{
    QSpinBox::paintEvent(event);
    drawBorderBottom();
}

DoubleSpinBox::DoubleSpinBox(QWidget *parent) : QDoubleSpinBox(parent), SpinBoxBase(this)
{
    setUpUi();
}

QLineEdit *DoubleSpinBox::lineEdit() const
{
    return QDoubleSpinBox::lineEdit();
}

void DoubleSpinBox::paintEvent(QPaintEvent *event)
{
    QDoubleSpinBox::paintEvent(event);
    drawBorderBottom();
}

TimeEdit::TimeEdit(QWidget *parent) : QTimeEdit(parent), SpinBoxBase(this)
{
    setUpUi();
}

QLineEdit *TimeEdit::lineEdit() const
{
    return QTimeEdit::lineEdit();
}

void TimeEdit::paintEvent(QPaintEvent *event)
{
    QTimeEdit::paintEvent(event);
    drawBorderBottom();
}

DateTimeEdit::DateTimeEdit(QWidget *parent) : QDateTimeEdit(parent), SpinBoxBase(this)
{
    setUpUi();
}

QLineEdit *DateTimeEdit::lineEdit() const
{
    return QDateTimeEdit::lineEdit();
}

void DateTimeEdit::paintEvent(QPaintEvent *event)
{
    QDateTimeEdit::paintEvent(event);
    drawBorderBottom();
}

DateEdit::DateEdit(QWidget *parent) : QDateEdit(parent), SpinBoxBase(this)
{
    setUpUi();
}

QLineEdit *DateEdit::lineEdit() const
{
    return QDateEdit::lineEdit();
}

void DateEdit::paintEvent(QPaintEvent *event)
{
    QDateEdit::paintEvent(event);
    drawBorderBottom();
}
