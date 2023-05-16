#include "PickerBase.h"
#include "QFluentWidgets.h"
#include "Widgets/CycleListWidget.h"
#include "Widgets/Button.h"
#include "Widgets/ScrollArea.h"

#include <QPainter>
#include <QApplication>
#include <QScreen>

ItemMaskWidget::ItemMaskWidget(const QList<CycleListWidget *> &list, QWidget *parent)
    : QWidget(parent), listWidgets(list)
{
    setFixedHeight(37);
    FluentStyleSheet::apply("TIME_PICKER", this);
}

void ItemMaskWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // draw background
    painter.setPen(Qt::NoPen);
    painter.setBrush(themeColor());
    painter.drawRoundedRect(this->rect().adjusted(4, 0, -3, 0), 5, 5);

    // draw text
    if (QFWIns.isDarkTheme()) {
        painter.setPen(Qt::black);
    } else {
        painter.setPen(Qt::white);
    }
    painter.setFont(this->font());

    int w = 0;
    int h = this->height();

    for (auto p : listWidgets) {
        painter.save();

        // draw first item's text
        int x                  = p->itemSize().width() / 2 + 4 + this->x();
        QListWidgetItem *item1 = p->itemAt(QPoint(x, this->y() + 6));
        if (!item1) {
            painter.restore();
            continue;
        }

        int iw = item1->sizeHint().width();
        int y  = p->visualItemRect(item1).y();
        painter.translate(w, y - this->y() + 7);
        drawText(item1, &painter, 0);

        // draw second item's text
        QListWidgetItem *item2 = p->itemAt(this->pos() + QPoint(x, h - 6));
        drawText(item2, &painter, h);

        painter.restore();
        w += (iw + 8);  // margin: 0 4px;
    }
}

void ItemMaskWidget::drawText(QListWidgetItem *item, QPainter *p, int y)
{
    int align = item->textAlignment();
    int w     = item->sizeHint().width();
    int h     = item->sizeHint().height();
    QRectF rect;
    if (align & Qt::AlignLeft) {
        rect = QRectF(15, y, w, h);  // padding-left: 11px
    } else if (align & Qt::AlignRight) {
        rect = QRectF(4, y, w - 15, h);  // padding-right: 11px
    } else if (align & Qt::AlignCenter) {
        rect = QRectF(4, y, w, h);
    }

    p->drawText(rect, align, item->text());
}

PickerColumnButton::PickerColumnButton(const QString &name, const QVariantList &items, int width,
                                       Qt::AlignmentFlag align, PickerColumnFormatter *formatter, QWidget *parent)
    : QPushButton(parent), m_name(name), m_value("")
{
    setItems(items);
    setAlignment(align);
    setFormatter(formatter);
    setFixedSize(width, 30);
    setObjectName("pickerButton");
    setProperty("hasBorder", false);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

QVariantList PickerColumnButton::items() const
{
    return m_items;
}

void PickerColumnButton::setItems(const QVariantList &items)
{
    m_items = items;
}

QString PickerColumnButton::value() const
{
    return m_value;
}

void PickerColumnButton::setValue(const QString &value)
{
    m_value = value;
    if (value.isEmpty()) {
        setText(this->name());
        setProperty("hasValue", false);
    } else {
        setText(this->value());
        setProperty("hasValue", true);
    }
    setStyle(QApplication::style());
}

Qt::AlignmentFlag PickerColumnButton::alignment() const
{
    return m_align;
}

void PickerColumnButton::setAlignment(const Qt::AlignmentFlag &align)
{
    if (align == Qt::AlignLeft) {
        setProperty("align", "left");
    } else if (align == Qt::AlignRight) {
        setProperty("align", "right");
    } else {
        setProperty("align", "center");
    }

    m_align = align;
    setStyle(QApplication::style());
}

PickerColumnFormatter *PickerColumnButton::formatter() const
{
    return m_formatter;
}

void PickerColumnButton::setFormatter(PickerColumnFormatter *formatter)
{
    if (!formatter) {
        m_formatter = new PickerColumnFormatter(this);
    } else {
        m_formatter = formatter;
    }
}

QString PickerColumnButton::name() const
{
    return m_name;
}

void PickerColumnButton::setName(const QString &name)
{
    if (this->text() == m_name) {
        this->setText(name);
    }
    m_name = name;
}

PickerBase::PickerBase(QWidget *parent) : QPushButton(parent)
{
    hBoxLayout = new QHBoxLayout(this);

    hBoxLayout->setSpacing(0);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSizeConstraint(QHBoxLayout::SetFixedSize);

    FluentStyleSheet::apply("TIME_PICKER", this);

    connect(this, &PickerBase::clicked, this, &PickerBase::showPanel);
}

void PickerBase::addColumn(const QString &name, const QVariantList &items, int width, Qt::AlignmentFlag align,
                           PickerColumnFormatter *formatter)
{
    PickerColumnButton *button = new PickerColumnButton(name, items, width, align, formatter, this);
    columns.append(button);

    hBoxLayout->addWidget(button, 0, Qt::AlignLeft);

    // update the style of buttons
    for (int i = 0; i < columns.count() - 1; ++i) {
        columns[i]->setProperty("hasBorder", true);
        columns[i]->setStyle(QApplication::style());
    }
}

#define checkColumnIndex(index)                                                                                        \
    do {                                                                                                               \
        if (index < 0 || index >= this->columns.count())                                                               \
            return;                                                                                                    \
    } while (0)

void PickerBase::setColumnAlignment(int index, Qt::AlignmentFlag align)
{
    checkColumnIndex(index);

    columns[index]->setAlignment(align);
}

void PickerBase::setColumnWidth(int index, int width)
{
    checkColumnIndex(index);

    columns[index]->setFixedWidth(width);
}

void PickerBase::setColumnTight(int index)
{
    checkColumnIndex(index);

    QFontMetrics fm = fontMetrics();
    int w           = -1;
    for (auto c : columns[index]->items()) {
        if (w < fm.width(c.toString())) {
            w = fm.width(c.toString());
        }
    }

    w += 30;
    setColumnWidth(index, w);
}

void PickerBase::setColumnVisible(int index, bool visible)
{
    checkColumnIndex(index);

    columns[index]->setVisible(visible);
}

QStringList PickerBase::value() const
{
    QStringList ret;
    for (auto c : columns) {
        if (c->isVisible()) {
            ret.append(c->value());
        }
    }
    return ret;
}

void PickerBase::setColumnValue(int index, int value)
{
    checkColumnIndex(index);

    columns[index]->setValue(QString::number(value));
}

void PickerBase::setColumnFormatter(int index, PickerColumnFormatter *formatter)
{
    checkColumnIndex(index);

    columns[index]->setFormatter(formatter);
}

void PickerBase::setColumnItems(int index, const QVariantList &items)
{
    checkColumnIndex(index);

    columns[index]->setItems(items);
}

QVariant PickerBase::encodeValue(int index, const QVariant &value)
{
    if (index < 0 || index >= this->columns.count())
        return QVariant();

    return columns.at(index)->formatter()->encode(value);
}

QVariant PickerBase::decodeValue(int index, const QVariant &value)
{
    if (index < 0 || index >= this->columns.count())
        return QVariant();

    return columns.at(index)->formatter()->decode(value);
}

QStringList PickerBase::panelInitialValue()
{
    return value();
}

void PickerBase::setColumn(int index, const QString &name, const QVariantList & /*items*/, int width,
                           Qt::AlignmentFlag align)
{
    checkColumnIndex(index);

    PickerColumnButton *button = columns.at(index);
    button->setText(name);
    button->setFixedWidth(width);
    button->setAlignment(align);
}

void PickerBase::clearColumns()
{
    while (!columns.isEmpty()) {
        PickerColumnButton *btn = columns.takeFirst();
        hBoxLayout->removeWidget(btn);
        btn->deleteLater();
    }
}

void PickerBase::showPanel()
{
    PickerPanel *panel = new PickerPanel(this);

    for (auto column : columns) {
        if (column->isVisible()) {
            panel->addColumn(column->items(), column->width(), column->alignment());
        }
    }

    panel->setValue(this->panelInitialValue());

    connect(panel, &PickerPanel::confirmed, this, &PickerBase::onConfirmed);
    connect(panel, &PickerPanel::columnValueChanged,
            [=](int index, const QString &value) { onColumnValueChanged(panel, index, value); });
    panel->exec(this->mapToGlobal(QPoint(0, -37 * 4)));
}

void PickerBase::onConfirmed(const QStringList &value)
{
    for (int i = 0; i < value.count(); ++i) {
        setColumnValue(i, value.at(i).toInt());
    }
}

void PickerBase::onColumnValueChanged(PickerPanel *panel, int index, const QString &value)
{
    Q_UNUSED(panel)
    Q_UNUSED(index)
    Q_UNUSED(value)
}

void PickerBase::mousePressEvent(QMouseEvent *event)
{
    setButtonProperty("pressed", true);
    QPushButton::mousePressEvent(event);
}

void PickerBase::mouseReleaseEvent(QMouseEvent *event)
{
    setButtonProperty("pressed", false);
    QPushButton::mouseReleaseEvent(event);
}

void PickerBase::enterEvent(QEvent * /*event*/)
{
    setButtonProperty("enter", true);
}

void PickerBase::leaveEvent(QEvent * /*event*/)
{
    setButtonProperty("enter", false);
}

void PickerBase::setButtonProperty(const char *name, const QVariant &value)
{
    for (auto c : columns) {
        c->setProperty(name, value);
        c->setStyle(QApplication::style());
    }
}

PickerPanel::PickerPanel(QWidget *parent) : QWidget(parent)
{
    m_itemHeight = 37;

    m_view             = new QFrame(this);
    m_itemMaskWidget   = new ItemMaskWidget(m_listWidgets, this);
    m_hSeparatorWidget = new SeparatorWidget(Qt::Horizontal, m_view);
    m_yesButton        = new TransparentToolButton(NEWFLICON(FluentIcon::ACCEPT), m_view);
    m_cancelButton     = new TransparentToolButton(NEWFLICON(FluentIcon::CLOSE), m_view);

    m_hBoxLayout   = new QHBoxLayout(this);
    m_listLayout   = new QHBoxLayout();
    m_buttonLayout = new QHBoxLayout();
    m_vBoxLayout   = new QVBoxLayout(m_view);

    m_isExpanded   = false;
    m_ani          = nullptr;
    m_shadowEffect = nullptr;

    initWidget();
}

void PickerPanel::initWidget()
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setShadowEffect();
    m_yesButton->setIconSize(QSize(16, 16));
    m_cancelButton->setIconSize(QSize(13, 13));
    m_yesButton->setFixedHeight(33);
    m_cancelButton->setFixedHeight(33);

    m_hBoxLayout->setContentsMargins(12, 8, 12, 20);
    m_hBoxLayout->addWidget(m_view, 1, Qt::AlignCenter);
    m_hBoxLayout->setSizeConstraint(QHBoxLayout::SetMinimumSize);

    m_vBoxLayout->setSpacing(0);
    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxLayout->addLayout(m_listLayout, 1);
    m_vBoxLayout->addWidget(m_hSeparatorWidget);
    m_vBoxLayout->addLayout(m_buttonLayout, 1);
    m_vBoxLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);

    m_buttonLayout->setSpacing(6);
    m_buttonLayout->setContentsMargins(3, 3, 3, 3);
    m_buttonLayout->addWidget(m_yesButton);
    m_buttonLayout->addWidget(m_cancelButton);
    m_yesButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_cancelButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(m_yesButton, &TransparentToolButton::clicked, this, &PickerPanel::fadeOut);
    connect(m_yesButton, &TransparentToolButton::clicked, [=]() { emit confirmed(this->value()); });
    connect(m_cancelButton, &TransparentToolButton::clicked, this, &PickerPanel::fadeOut);

    m_view->setObjectName("view");
    FluentStyleSheet::apply("TIME_PICKER", this);
}

void PickerPanel::setShadowEffect(int blurRadius, const QPointF &offset, const QColor &color)
{
    m_shadowEffect = new QGraphicsDropShadowEffect(m_view);
    m_shadowEffect->setBlurRadius(blurRadius);
    m_shadowEffect->setOffset(offset);
    m_shadowEffect->setColor(color);
    m_view->setGraphicsEffect(nullptr);
    m_view->setGraphicsEffect(m_shadowEffect);
}

void PickerPanel::addColumn(const QVariantList &items, int width, Qt::AlignmentFlag align)
{
    if (!m_listWidgets.isEmpty()) {
        m_listLayout->addWidget(new SeparatorWidget(Qt::Vertical, m_view));
    }

    CycleListWidget *w = new CycleListWidget(items, QSize(width, m_itemHeight), align, this);
    connect(w->vScrollBar, &SmoothScrollBar::valueChanged, m_itemMaskWidget, QOverload<>::of(&ItemMaskWidget::update));

    const int N = m_listWidgets.count();
    connect(w, &CycleListWidget::currentItemChanged,
            [=](QListWidgetItem *item) { emit columnValueChanged(N, item->text()); });

    m_listWidgets.append(w);
    m_listLayout->addWidget(w);
}

/// return the value of columns
QStringList PickerPanel::value() const
{
    QStringList ret;
    for (auto w : m_listWidgets) {
        ret << w->currentItem()->text();
    }
    return ret;
}

/// set the value of columns
void PickerPanel::setValue(const QStringList &value)
{
    if (value.count() != m_listWidgets.count()) {
        return;
    }

    for (int i = 0; i < value.count(); ++i) {
        m_listWidgets.at(i)->setSelectedItem(value.at(i));
    }
}

QString PickerPanel::columnValue(int index) const
{
    if (index < 0 || index >= m_listWidgets.count()) {
        return "";
    }

    return m_listWidgets.at(index)->currentItem()->text();
}

void PickerPanel::setColumnValue(int index, const QString &value)
{
    if (index < 0 || index >= m_listWidgets.count()) {
        return;
    }

    m_listWidgets.at(index)->setSelectedItem(value);
}

CycleListWidget *PickerPanel::column(int index) const
{
    if (index < 0 || index >= m_listWidgets.count()) {
        return nullptr;
    }

    return m_listWidgets.at(index);
}

void PickerPanel::exec(const QPoint &pos, bool ani)
{
    if (isVisible()) {
        return;
    }

    // show before running animation, or the height calculation will be wrong
    this->show();

    QRect rect = QApplication::screenAt(QCursor::pos())->availableGeometry();
    int w      = this->width() + 5;
    int h      = this->height();

    QPoint newPos = pos;
    newPos.setX(qMin(pos.x() - this->layout()->contentsMargins().left(), rect.right() - w));
    newPos.setY(qMax(rect.top(), qMin(pos.y() - 4, rect.bottom() - h + 5)));
    this->move(newPos);

    if (!ani) {
        return;
    }

    m_isExpanded = false;
    m_ani        = new QPropertyAnimation(m_view, "windowOpacity", this);
    connect(m_ani, &QPropertyAnimation::valueChanged, this, &PickerPanel::onAniValueChanged);
    m_ani->setStartValue(0);
    m_ani->setEndValue(1);
    m_ani->setDuration(150);
    m_ani->setEasingCurve(QEasingCurve::OutQuad);
    m_ani->start();
}

void PickerPanel::resizeEvent(QResizeEvent * /*event*/)
{
    m_itemMaskWidget->resize(m_view->width() - 3, m_itemHeight);
    QMargins m = m_hBoxLayout->contentsMargins();
    m_itemMaskWidget->move(m.left() + 2, m.top() + 148);
}

void PickerPanel::fadeOut()
{
    m_isExpanded = true;
    m_ani        = new QPropertyAnimation(this, "windowOpacity", this);
    connect(m_ani, &QPropertyAnimation::valueChanged, this, &PickerPanel::onAniValueChanged);
    connect(m_ani, &QPropertyAnimation::finished, this, &PickerPanel::deleteLater);
    m_ani->setStartValue(1);
    m_ani->setEndValue(0);
    m_ani->setDuration(150);
    m_ani->setEasingCurve(QEasingCurve::OutQuad);
    m_ani->start();
}

void PickerPanel::onAniValueChanged(const QVariant &value)
{
    QMargins m = this->layout()->contentsMargins();
    int w      = m_view->width() + m.left() + m.right() + 120;
    int h      = m_view->height() + m.top() + m.bottom() + 12;

    double opacity = value.toDouble();
    if (!m_isExpanded) {
        int y = int(h / 2 * (1 - opacity));
        setMask(QRegion(0, y, w, h - y * 2));
    } else {
        int y = int(h / 3 * (1 - opacity));
        setMask(QRegion(0, y, w, h - y * 2));
    }
}
