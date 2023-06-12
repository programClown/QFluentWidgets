#include "SettingCard.h"
#include "Widgets/IconWidget.h"
#include "Widgets/SwitchButton.h"
#include "Widgets/Slider.h"
#include "Widgets/Button.h"
#include "Widgets/ComboBox.h"
#include "QFluentWidgets.h"
#include "DialogBox/ColorDialog.h"

#include <QPainter>

SettingCard::SettingCard(FluentIconBase *ficon, const QString &title, const QString &content, QWidget *parent)
    : QFrame(parent), m_ficon(ficon)
{

    m_iconLabel    = new IconWidget(ficon, this);
    m_titleLabel   = new QLabel(title, this);
    m_contentLabel = new QLabel(content, this);
    hBoxLayout     = new QHBoxLayout();
    vBoxLayout     = new QVBoxLayout();

    if (content.isEmpty()) {
        m_contentLabel->hide();
    }

    hBoxLayout->setSpacing(0);
    hBoxLayout->setContentsMargins(16, 0, 0, 0);
    hBoxLayout->setAlignment(Qt::AlignVCenter);
    vBoxLayout->setSpacing(0);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setAlignment(Qt::AlignVCenter);

    hBoxLayout->addWidget(m_iconLabel, 0, Qt::AlignLeft);
    hBoxLayout->addSpacing(16);

    hBoxLayout->addLayout(vBoxLayout);
    vBoxLayout->addWidget(m_titleLabel, 0, Qt::AlignLeft);
    vBoxLayout->addWidget(m_contentLabel, 0, Qt::AlignLeft);

    hBoxLayout->addSpacing(16);
    hBoxLayout->addStretch(1);

    setLayout(hBoxLayout);

    m_contentLabel->setObjectName("contentLabel");
    FluentStyleSheet::apply("SETTING_CARD", this);
}

void SettingCard::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

void SettingCard::setContent(const QString &content)
{
    m_contentLabel->setText(content);
    m_contentLabel->setVisible(!content.isEmpty());
}

FluentIconBase *SettingCard::ficon() const
{
    return m_ficon.data();
}

void SettingCard::setValue(const QVariant &) { }

SwitchSettingCard::SwitchSettingCard(FluentIconBase *ficon, const QString &title, const QString &content,
                                     Qfw::ConfigItem *configItem, QWidget *parent)
    : SettingCard(ficon, title, content, parent), m_configItem(configItem)
{
    m_switchButton = new SwitchButton(tr("Off"), this, IndicatorPosition::RIGHT);

    if (configItem) {
        setValue(configItem->value());
        connect(configItem, &Qfw::ConfigItem::valueChanged, this, &SwitchSettingCard::setValue);
    }

    // add switch button to layout
    hBoxLayout->addWidget(m_switchButton, 0, Qt::AlignRight);
    hBoxLayout->addSpacing(16);

    connect(m_switchButton, &SwitchButton::checkedChanged, this, &SwitchSettingCard::onCheckedChanged);
}

void SwitchSettingCard::setValue(const QVariant &value)
{
    // 设计同步到配置文件里面去
    //    if self.configItem:
    //                qconfig.set(self.configItem, isChecked)

    m_switchButton->setChecked(value.toBool());
    if (value.toBool()) {
        m_switchButton->setText(tr("On"));
    } else {
        m_switchButton->setText(tr("Off"));
    }
}

bool SwitchSettingCard::isChecked() const
{
    return m_switchButton->isChecked();
}

void SwitchSettingCard::onCheckedChanged(bool isChecked)
{
    setValue(isChecked);
    emit checkedChanged(isChecked);
}

RangeSettingCard::RangeSettingCard(int min, int max, int value, FluentIconBase *ficon, const QString &title,
                                   const QString &content, QWidget *parent)
    : SettingCard(ficon, title, content, parent), m_min(min), m_max(max)
{
    m_slider     = new Slider(Qt::Horizontal, this);
    m_valueLabel = new QLabel(this);
    m_slider->setMinimumWidth(268);

    m_slider->setSingleStep(1);
    m_slider->setRange(m_min, m_max);
    m_slider->setValue(value);

    m_valueLabel->setNum(value);

    hBoxLayout->addStretch(1);
    hBoxLayout->addWidget(m_valueLabel, 0, Qt::AlignRight);
    hBoxLayout->addSpacing(6);
    hBoxLayout->addWidget(m_slider, 0, Qt::AlignRight);
    hBoxLayout->addSpacing(16);

    m_valueLabel->setObjectName("valueLabel");

    // TODO 配置文件修改也能调整这里

    connect(m_slider, &Slider::valueChanged, this, &RangeSettingCard::onValueChanged);
}

void RangeSettingCard::setValue(const QVariant &value)
{
    // TODO 更新配置文件，记录修改

    if (m_slider->value() != value.toInt()) {
        m_slider->setValue(value.toInt());
    }
    m_valueLabel->setNum(value.toInt());
    m_valueLabel->adjustSize();
}

void RangeSettingCard::onValueChanged(int value)
{
    setValue(value);
    emit valueChanged(value);
}

PushSettingCard::PushSettingCard(const QString &text, FluentIconBase *ficon, const QString &title,
                                 const QString &content, QWidget *parent)
    : SettingCard(ficon, title, content, parent)
{
    button = new QPushButton(text, this);
    hBoxLayout->addWidget(button, 0, Qt::AlignRight);
    hBoxLayout->addSpacing(16);
    connect(button, &QPushButton::clicked, this, &PushSettingCard::clicked);
}

PrimaryPushSettingCard::PrimaryPushSettingCard(const QString &text, FluentIconBase *ficon, const QString &title,
                                               const QString &content, QWidget *parent)
    : PushSettingCard(text, ficon, title, content, parent)
{
    button->setObjectName("primaryButton");
}

HyperlinkCard::HyperlinkCard(const QString &url, const QString &text, FluentIconBase *ficon, const QString &title,
                             const QString &content, QWidget *parent)
    : SettingCard(ficon, title, content, parent)
{
    linkButton = new HyperlinkButton(url, text, this);
    hBoxLayout->addWidget(linkButton, 0, Qt::AlignRight);
    hBoxLayout->addSpacing(16);
}

ColorPickerButton::ColorPickerButton(const QColor &color, const QString &title, QWidget *parent)
    : QPushButton(parent), m_title(title)
{
    setFixedSize(96, 32);
    setAttribute(Qt::WA_TranslucentBackground);

    setColor(color);
    setCursor(Qt::PointingHandCursor);
    connect(this, &ColorPickerButton::clicked, this, &ColorPickerButton::showColorDialog);
}

void ColorPickerButton::setColor(const QColor &color)
{
    m_color = color;
    update();
}

void ColorPickerButton::showColorDialog()
{
    ColorDialog *w = new ColorDialog(m_color, tr("Choose") + " " + m_title, this->window());
    w->updateStyle();
    connect(w, &ColorDialog::colorChanged, this, &ColorPickerButton::onColorChanged);
    w->exec();
    w->deleteLater();
}

void ColorPickerButton::onColorChanged(const QColor &color)
{
    setColor(color);
    emit colorChanged(color);
}

void ColorPickerButton::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing);
    auto pc = QFWIns.isDarkTheme() ? QColor(255, 255, 255, 10) : QColor(234, 234, 234);
    painter.setPen(pc);
    painter.setBrush(m_color);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
}

ColorSettingCard::ColorSettingCard(const QColor &color, FluentIconBase *ficon, const QString &title,
                                   const QString &content, QWidget *parent)
    : SettingCard(ficon, title, content, parent), m_color(color)
{
    m_colorPicker = new ColorPickerButton(color, title, this);
    hBoxLayout->addWidget(m_colorPicker, 0, Qt::AlignRight);
    hBoxLayout->addSpacing(16);
    connect(m_colorPicker, &ColorPickerButton::colorChanged, this, &ColorSettingCard::onColorChanged);
}

void ColorSettingCard::setValue(const QVariant &value)
{
    m_colorPicker->setColor(value.value<QColor>());
    /// TODO: 存起来值
}

void ColorSettingCard::onColorChanged(const QColor &color)
{
    /// TODO: 存起来值
    emit colorChanged(color);
}

ComboBoxSettingCard::ComboBoxSettingCard(const QString &defaultText, const QHash<QString, QVariant> &option,
                                         FluentIconBase *ficon, const QString &title, const QString &content,
                                         QWidget *parent)
    : SettingCard(ficon, title, content, parent), m_options(option)
{

    m_comboBox = new ComboBox(this);
    hBoxLayout->addWidget(m_comboBox, 0, Qt::AlignRight);
    hBoxLayout->addSpacing(16);

    //    QHashIterator<QVariant, QString> i(option);
    //    while (i.hasNext()) {
    //        i.next();
    //        m_comboBox->addItem(i.value(), i.key());
    //    }
    //    m_comboBox->setCurrentText(defaultText);

    //    connect(m_comboBox, &ComboBox::currentIndexChanged, this, &ComboBoxSettingCard::onCurrentIndexChanged);
}

void ComboBoxSettingCard::setValue(const QVariant &value)
{
    if (!m_options.values().contains(value)) {
        return;
    }

    //    m_comboBox->setCurrentText(m_options[value]);
    /// TODO: 存起来值
}

void ComboBoxSettingCard::onCurrentIndexChanged(int index)
{
    /// TODO: 存起来值
}
