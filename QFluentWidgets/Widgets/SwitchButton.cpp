#include "SwitchButton.h"
#include "Common/StyleSheet.h"

#include <QTimer>
#include <QStyle>
#include <QPainter>

Indicator::Indicator(QWidget *parent)
    : QToolButton(parent),
      m_sliderOnColor(QColor(Qt::white)),
      m_sliderOffColor(QColor(Qt::black)),
      m_sliderDisabledColor(QColor(155, 154, 153)),
      m_timer(new QTimer(this))
{
    setCheckable(true);
    QToolButton::setChecked(false);
    resize(37, 16);
    m_padding      = height() / 4;
    m_sliderX      = m_padding;
    m_sliderRadius = (height() - 2 * m_padding) / 2;
    m_sliderEndX   = width() - 2 * m_sliderRadius;
    m_sliderStep   = width() / 50.0;
    connect(m_timer, &QTimer::timeout, this, &Indicator::updateSliderPos);
}

void Indicator::setChecked(bool checked)
{
    if (checked == this->isChecked()) {
        return;
    }

    QToolButton::setChecked(checked);
    m_sliderRadius = (this->height() - 2 * m_padding) / 2;
    m_sliderEndX   = checked ? (this->width() - 2 * m_sliderRadius - m_padding) : m_padding;
    m_timer->start(5);
}

QColor Indicator::getSliderOnColor() const
{
    return m_sliderOnColor;
}

void Indicator::setSliderOnColor(const QColor &sliderOnColor)
{
    m_sliderOnColor = sliderOnColor;
    update();
}

QColor Indicator::getSliderOffColor() const
{
    return m_sliderOffColor;
}

void Indicator::setSliderOffColor(const QColor &sliderOffColor)
{
    m_sliderOffColor = sliderOffColor;
    update();
}

QColor Indicator::getSliderDisabledColor() const
{
    return m_sliderDisabledColor;
}

void Indicator::setSliderDisabledColor(const QColor &sliderDisabledColor)
{
    m_sliderDisabledColor = sliderDisabledColor;
    update();
}

/// update slider position
void Indicator::updateSliderPos()
{
    if (isChecked()) {
        if (m_sliderX + m_sliderStep < m_sliderEndX) {
            m_sliderX += m_sliderStep;
        } else {
            m_sliderX = m_sliderEndX;
            m_timer->stop();
        }
    } else {
        if (m_sliderX - m_sliderStep > m_sliderEndX) {
            m_sliderX -= m_sliderStep;
        } else {
            m_sliderX = m_padding;
            m_timer->stop();
        }
    }

    this->style()->polish(this);
}

/// toggle checked state when mouse release
void Indicator::mouseReleaseEvent(QMouseEvent *event)
{
    QToolButton::mouseReleaseEvent(event);
    m_sliderEndX = isChecked() ? (this->width() - 2 * m_sliderRadius - m_padding) : m_padding;
    m_timer->start(5);

    emit checkedChanged(isChecked());
}

/// paint indicator
void Indicator::paintEvent(QPaintEvent *event)
{
    // the background and border are specified by qss
    QToolButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QColor color;
    if (isEnabled()) {
        color = isChecked() ? m_sliderOnColor : m_sliderOffColor;
    } else {
        color = m_sliderDisabledColor;
    }

    painter.setBrush(color);
    painter.drawEllipse(int(m_sliderX), m_padding, m_sliderRadius * 2, m_sliderRadius * 2);
}

void Indicator::resizeEvent(QResizeEvent * /*event*/)
{
    m_padding      = this->height() / 4;
    m_sliderRadius = (height() - 2 * m_padding) / 2;
    m_sliderStep   = width() / 50.0;
    m_sliderEndX   = isChecked() ? (this->width() - 2 * m_sliderRadius - m_padding) : m_padding;
    update();
}

SwitchButton::SwitchButton(const QString &text, QWidget *parent, IndicatorPosition indicatorPos)
    : QWidget(parent),
      m_text(text),
      m_spacing(12),
      m_indicatorPos(indicatorPos),
      m_hBox(new QHBoxLayout(this)),
      m_indicator(new Indicator(this)),
      m_label(new QLabel(text, this))
{
    initWidget();
}

bool SwitchButton::isChecked() const
{
    return m_indicator->isChecked();
}

void SwitchButton::setChecked(bool checked)
{
    this->adjustSize();
    m_indicator->setChecked(checked);
}

void SwitchButton::toggleChecked()
{
    m_indicator->setChecked(!m_indicator->isChecked());
}

QString SwitchButton::text() const
{
    return m_text;
}

void SwitchButton::setText(const QString &text)
{
    m_text = text;
    m_label->setText(text);
    this->adjustSize();
}

int SwitchButton::getSpacing() const
{
    return m_spacing;
}

void SwitchButton::setSpacing(int spacing)
{
    m_spacing = spacing;
    m_hBox->setSpacing(spacing);
    update();
}

void SwitchButton::initWidget()
{
    setAttribute(Qt::WA_StyledBackground);

    m_hBox->setSpacing(m_spacing);
    m_hBox->setContentsMargins(0, 0, 0, 0);

    if (m_indicatorPos == IndicatorPosition::LEFT) {
        m_hBox->addWidget(m_indicator);
        m_hBox->addWidget(m_label);
        m_hBox->setAlignment(Qt::AlignLeft);
    } else {
        m_hBox->addWidget(m_label, 0, Qt::AlignRight);
        m_hBox->addWidget(m_indicator, 0, Qt::AlignRight);
        m_hBox->setAlignment(Qt::AlignRight);
    }

    // set default style sheet
    FluentStyleSheet::apply("SWITCH_BUTTON", this);

    connect(m_indicator, &Indicator::checkedChanged, this, &SwitchButton::checkedChanged);
}
