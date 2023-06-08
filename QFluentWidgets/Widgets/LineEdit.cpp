#include "LineEdit.h"
#include "Menu.h"
#include "Common/StyleSheet.h"
#include "Common/SmoothScroll.h"
#include "QFluentWidgets.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QPainterPath>
#include <QMouseEvent>
#include <QDebug>

LineEditButton::LineEditButton(QSharedPointer<FluentIconBase> ficon, QWidget *parent)
    : QToolButton(parent), m_ficon(ficon)
{
    setFixedSize(31, 23);
    setIconSize(QSize(10, 10));
    setCursor(Qt::PointingHandCursor);
    setObjectName("lineEditButton");
    FluentStyleSheet::apply("LINE_EDIT", this);
}

void LineEditButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
    if (!m_ficon) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    int iw      = iconSize().width();
    int ih      = iconSize().height();
    int w       = width();
    int h       = height();
    QRectF rect = QRectF((w - iw) / 2, (h - ih) / 2, iw, ih);

    if (QFWIns.isDarkTheme()) {
        m_ficon->render(&painter, rect.toRect());
    } else {
        m_ficon->render(&painter, rect.toRect(), "#656565");
    }
}

LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent), m_isClearButtonEnabled(false), m_clearButton(nullptr)
{
    FluentStyleSheet::apply("LINE_EDIT", this);
    setFixedHeight(33);
    setAttribute(Qt::WA_MacShowFocusRect, false);

    m_hBoxLayout  = new QHBoxLayout(this);
    m_clearButton = new LineEditButton(NEWFLICON(FluentIcon::CLOSE), this);
    m_clearButton->setFixedSize(29, 25);
    m_clearButton->hide();

    m_hBoxLayout->setSpacing(3);
    m_hBoxLayout->setContentsMargins(4, 4, 4, 4);
    m_hBoxLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_hBoxLayout->addWidget(m_clearButton, 0, Qt::AlignRight);

    connect(m_clearButton, &LineEditButton::clicked, this, &LineEdit::clear);
    connect(this, &LineEdit::textChanged, this, &LineEdit::onTextChanged);
}

bool LineEdit::isClearButtonEnabled() const
{
    return m_isClearButtonEnabled;
}

void LineEdit::setIsClearButtonEnabled(bool enable)
{
    m_isClearButtonEnabled = enable;
    setTextMargins(0, 0, 28 * int(enable), 0);
}

void LineEdit::onTextChanged(const QString &text)
{
    if (m_isClearButtonEnabled) {
        m_clearButton->setVisible((!text.isEmpty() && hasFocus()));
    }
}

void LineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    if (m_isClearButtonEnabled) {
        m_clearButton->setVisible(!text().isEmpty());
    }
}

void LineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    m_clearButton->hide();
}

void LineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    if (!hasFocus()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    int w = width();
    int h = height();
    path.addRoundedRect(QRectF(0, h - 10, w, 10), 5, 5);

    QPainterPath rectPath;
    rectPath.addRect(0, h - 10, w, 8);
    path = path.subtracted(rectPath);
    painter.fillPath(path, themeColor());
}

void LineEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QScopedPointer<LineEditMenu> menu(new LineEditMenu(this));
    connect(menu->cutAct, &QAction::triggered, this, &LineEdit::cut);
    connect(menu->copyAct, &QAction::triggered, this, &LineEdit::copy);
    connect(menu->pasteAct, &QAction::triggered, this, &LineEdit::paste);
    connect(menu->cancelAct, &QAction::triggered, this, &LineEdit::undo);
    connect(menu->selectAllAct, &QAction::triggered, this, &LineEdit::selectAll);
    menu->exec(event->globalPos(), true);
}

LineEditButton *LineEdit::clearButton() const
{
    return m_clearButton;
}

QHBoxLayout *LineEdit::hBoxLayout() const
{
    return m_hBoxLayout;
}

SearchLineEdit::SearchLineEdit(QWidget *parent) : LineEdit(parent)
{
    m_searchButton       = new LineEditButton(NEWFLICON(FluentIcon::SEARCH), this);
    QHBoxLayout *hLayout = hBoxLayout();
    hLayout->addWidget(m_searchButton, 0, Qt::AlignRight);
    setClearButtonEnabled(true);
    setTextMargins(0, 0, 59, 0);

    connect(m_searchButton, &LineEditButton::clicked, this, &SearchLineEdit::search);
    connect(clearButton(), &LineEditButton::clicked, this, &SearchLineEdit::clearSignal);
}

void SearchLineEdit::search()
{
    QString txt = text().trimmed();
    if (!txt.isEmpty()) {
        emit searchSignal(txt);
    } else {
        emit clearSignal();
    }
}

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent)
{
    m_verticalSmoothScroll = new SmoothScroll(this, Qt::Vertical);
    m_horizonSmoothScroll  = new SmoothScroll(this, Qt::Horizontal);
    FluentStyleSheet::apply("LINE_EDIT", this);
}

void TextEdit::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::NoModifier) {
        if ((m_verticalSmoothScroll->smoothMode() == SmoothMode::NO_SMOOTH)
            || (abs(event->angleDelta().y()) % 120 != 0)) {
            QTextEdit::wheelEvent(event);
        } else {
            m_verticalSmoothScroll->wheelEvent(event);
        }
    } else {
        if ((m_horizonSmoothScroll->smoothMode() == SmoothMode::NO_SMOOTH)
            || (abs(event->angleDelta().y()) % 120 != 0)) {
            QTextEdit::wheelEvent(event);
        } else {
            m_horizonSmoothScroll->wheelEvent(event);
        }
    }
}

void TextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QScopedPointer<TextEditMenu> menu(new TextEditMenu(this));
    connect(menu->cutAct, &QAction::triggered, this, &TextEdit::cut);
    connect(menu->copyAct, &QAction::triggered, this, &TextEdit::copy);
    connect(menu->pasteAct, &QAction::triggered, this, &TextEdit::paste);
    connect(menu->cancelAct, &QAction::triggered, this, &TextEdit::undo);
    connect(menu->selectAllAct, &QAction::triggered, this, &TextEdit::selectAll);
    menu->exec(event->globalPos(), true);
}

PlainTextEdit::PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    m_verticalSmoothScroll = new SmoothScroll(this, Qt::Vertical);
    m_horizonSmoothScroll  = new SmoothScroll(this, Qt::Horizontal);
    FluentStyleSheet::apply("LINE_EDIT", this);
}

void PlainTextEdit::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::NoModifier) {
        if ((m_verticalSmoothScroll->smoothMode() == SmoothMode::NO_SMOOTH)
            || (abs(event->angleDelta().y()) % 120 != 0)) {
            QPlainTextEdit::wheelEvent(event);
        } else {
            m_verticalSmoothScroll->wheelEvent(event);
        }
    } else {
        if ((m_horizonSmoothScroll->smoothMode() == SmoothMode::NO_SMOOTH)
            || (abs(event->angleDelta().y()) % 120 != 0)) {
            QPlainTextEdit::wheelEvent(event);
        } else {
            m_horizonSmoothScroll->wheelEvent(event);
        }
    }
}

void PlainTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QScopedPointer<TextEditMenu> menu(new TextEditMenu(this));
    menu->exec(event->globalPos(), true);
}
