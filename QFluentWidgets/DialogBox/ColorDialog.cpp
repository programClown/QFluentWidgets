#include "ColorDialog.h"
#include "HuePanel.h"
#include "Common/StyleSheet.h"
#include "Widgets/ScrollArea.h"
#include "Widgets/Button.h"

#include <QApplication>
#include <QIntValidator>
#include <QPainter>
#include <QRegExp>

BrightnessSlider::BrightnessSlider(const QColor &color, QWidget *parent)
    : Slider(Qt::Horizontal, parent), m_color(color)
{
    setRange(0, 255);
    setSingleStep(1);
    setColor(color);
    connect(this, &BrightnessSlider::valueChanged, this, &BrightnessSlider::onValueChanged);
}

void BrightnessSlider::setColor(const QColor &color)
{
    m_color = color;
    setValue(color.value());

    QString qss = FluentStyleSheet::content("COLOR_DIALOG");

    qss = qss.replace("--slider-hue", QString::number(color.hue()));
    qss = qss.replace("--slider-saturation", QString::number(color.saturation()));

    setStyleSheet(qss);
}

void BrightnessSlider::onValueChanged(int value)
{
    m_color.setHsv(m_color.hue(), m_color.saturation(), value);
    setColor(m_color);
    emit colorChanged(m_color);
}

ColorCard::ColorCard(const QColor &color, QWidget *parent) : QWidget(parent)
{
    setFixedSize(44, 128);
    setColor(color);
}

void ColorCard::setColor(const QColor &color)
{
    m_color = color;
    update();
}

void ColorCard::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing);

    painter.setBrush(m_color);
    painter.setPen(QColor(0, 0, 0, 13));
    painter.drawRoundedRect(rect(), 4, 4);
}

ColorLineEdit::ColorLineEdit(const QString &value, QWidget *parent) : LineEdit(parent)
{
    setText(value);
    setFixedSize(136, 33);
    setClearButtonEnabled(true);
    setValidator(new QIntValidator(0, 255, this));

    connect(this, &ColorLineEdit::textEdited, this, &ColorLineEdit::onTextEdited);
}

void ColorLineEdit::onTextEdited(const QString &text)
{
    int pos                 = 0;
    QString input           = text;
    QValidator::State state = validator()->validate(input, pos);

    if (state == QIntValidator::Acceptable) {

        emit valueChanged(text);
    }
}

HexColorLineEdit::HexColorLineEdit(const QColor &color, QWidget *parent) : ColorLineEdit(color.name().mid(1), parent)
{
    setValidator(new QRegExpValidator(QRegExp("[A-Fa-f0-9]{6}"), this));
    setTextMargins(4, 0, 33, 0);
    prefixLabel = new QLabel("#", this);
    prefixLabel->move(7, 2);
    prefixLabel->setObjectName("prefixLabel");
}

void HexColorLineEdit::setColor(const QColor &color)
{
    setText(color.name().mid(1));
}

ColorDialog::ColorDialog(const QColor &color, const QString &title, QWidget *parent) : MaskDialogBase(parent)
{
    m_oldColor = color;
    m_color    = color;

    m_scrollArea   = new ScrollArea(this->widget);
    m_scrollWidget = new QWidget(m_scrollArea);

    m_buttonGroup  = new QFrame(this->widget);
    m_yesButton    = new PrimaryPushButton(tr("OK"), m_buttonGroup);
    m_cancelButton = new QPushButton(tr("Cancel"), m_buttonGroup);

    m_titleLabel   = new QLabel(title, m_scrollWidget);
    m_huePanel     = new HuePanel(color, m_scrollWidget);
    m_newColorCard = new ColorCard(color, m_scrollWidget);
    m_oldColorCard = new ColorCard(color, m_scrollWidget);
    m_brightSlider = new BrightnessSlider(color, m_scrollWidget);

    m_editLabel     = new QLabel(tr("Edit Color"), m_scrollWidget);
    m_redLabel      = new QLabel(tr("Red"), m_scrollWidget);
    m_greenLabel    = new QLabel(tr("Green"), m_scrollWidget);
    m_blueLabel     = new QLabel(tr("Blue"), m_scrollWidget);
    m_hexLineEdit   = new HexColorLineEdit(color, m_scrollWidget);
    m_redLineEdit   = new ColorLineEdit(QString::number(color.red()), m_scrollWidget);
    m_greenLineEdit = new ColorLineEdit(QString::number(color.green()), m_scrollWidget);
    m_blueLineEdit  = new ColorLineEdit(QString::number(color.blue()), m_scrollWidget);

    m_vBoxLayout = new QVBoxLayout(this->widget);

    initWidget();
}

void ColorDialog::setColor(const QColor &color, bool movePicker)
{
    m_color = QColor(color);
    m_brightSlider->setColor(color);
    m_newColorCard->setColor(color);
    m_hexLineEdit->setColor(color);
    m_redLineEdit->setText(QString::number(color.red()));
    m_blueLineEdit->setText(QString::number(color.blue()));
    m_greenLineEdit->setText(QString::number(color.green()));
    if (movePicker) {
        m_huePanel->setColor(color);
    }
}

void ColorDialog::updateStyle()
{
    setStyle(QApplication::style());
    m_titleLabel->adjustSize();
    m_editLabel->adjustSize();
}

void ColorDialog::initWidget()
{
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setViewportMargins(48, 24, 0, 24);
    m_scrollArea->setWidget(m_scrollWidget);

    this->widget->setMaximumSize(488, 696);
    this->widget->resize(488, 696);
    m_scrollWidget->resize(440, 560);
    m_buttonGroup->setFixedSize(486, 81);
    m_yesButton->setFixedWidth(216);
    m_cancelButton->setFixedWidth(216);

    this->setShadowEffect(60, QPointF(0, 10), QColor(0, 0, 0, 80));
    this->setMaskColor(QColor(0, 0, 0, 76));

    setQss();
    initLayout();
    connectSignalToSlot();
}

void ColorDialog::setQss()
{
    m_editLabel->setObjectName("editLabel");
    m_titleLabel->setObjectName("titleLabel");
    m_yesButton->setObjectName("yesButton");
    m_cancelButton->setObjectName("cancelButton");
    m_buttonGroup->setObjectName("buttonGroup");
    FluentStyleSheet::apply("COLOR_DIALOG", this);
    m_titleLabel->adjustSize();
    m_editLabel->adjustSize();
}

void ColorDialog::initLayout()
{
    m_huePanel->move(0, 46);
    m_newColorCard->move(288, 46);
    m_oldColorCard->move(288, m_newColorCard->geometry().bottom() + 1);
    m_brightSlider->move(0, 324);

    m_editLabel->move(0, 381);
    m_redLineEdit->move(0, 426);
    m_greenLineEdit->move(0, 470);
    m_blueLineEdit->move(0, 515);
    m_redLabel->move(144, 434);
    m_greenLabel->move(144, 478);
    m_blueLabel->move(144, 524);
    m_hexLineEdit->move(196, 381);

    m_vBoxLayout->setSpacing(0);
    m_vBoxLayout->setAlignment(Qt::AlignTop);
    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxLayout->addWidget(m_scrollArea, 1);
    m_vBoxLayout->addWidget(m_buttonGroup, 0, Qt::AlignBottom);

    m_yesButton->move(24, 25);
    m_cancelButton->move(250, 25);
}

void ColorDialog::connectSignalToSlot()
{
    connect(m_cancelButton, &QPushButton::clicked, this, &ColorDialog::reject);
    connect(m_yesButton, &PrimaryPushButton::clicked, this, &ColorDialog::onYesButtonClicked);

    connect(m_huePanel, &HuePanel::colorChanged, this, &ColorDialog::onHueChanged);
    connect(m_brightSlider, &BrightnessSlider::colorChanged, this, &ColorDialog::onBrightnessChanged);

    connect(m_redLineEdit, &ColorLineEdit::valueChanged, this, &ColorDialog::onRedChanged);
    connect(m_greenLineEdit, &ColorLineEdit::valueChanged, this, &ColorDialog::onGreenChanged);
    connect(m_blueLineEdit, &ColorLineEdit::valueChanged, this, &ColorDialog::onBlueChanged);
    connect(m_hexLineEdit, &HexColorLineEdit::valueChanged, this, &ColorDialog::onHexColorChanged);
}

void ColorDialog::onYesButtonClicked()
{
    this->accept();
    if (m_color != m_oldColor) {
        emit colorChanged(m_color);
    }
}

void ColorDialog::onHueChanged(const QColor &color)
{
    m_color.setHsv(color.hue(), color.saturation(), m_color.value());
    setColor(m_color);
}

void ColorDialog::onBrightnessChanged(const QColor &color)
{
    m_color.setHsv(m_color.hue(), m_color.saturation(), color.value());
    setColor(m_color, false);
}

void ColorDialog::onRedChanged(const QString &red)
{
    m_color.setRed(red.toInt());
    setColor(m_color);
}

void ColorDialog::onGreenChanged(const QString &green)
{
    m_color.setGreen(green.toInt());
    setColor(m_color);
}

void ColorDialog::onBlueChanged(const QString &blue)
{
    m_color.setBlue(blue.toInt());
    setColor(m_color);
}

void ColorDialog::onHexColorChanged(const QString &value)
{
    m_color.setNamedColor("#" + value);
    setColor(m_color);
}
