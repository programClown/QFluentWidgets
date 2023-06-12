#include "CustomColorSettingCard.h"
#include "Widgets/Button.h"
#include "Layout/VBoxLayout.h"
#include "DialogBox/ColorDialog.h"

#include <QButtonGroup>

CustomColorSettingCard::CustomColorSettingCard(const QVariant &configItem, FluentIconBase *icon, const QString &title,
                                               const QString &content, QWidget *parent)
    : ExpandGroupSettingCard(icon, title, content, parent)
{
    // TODO: 这里是配置项，未完成
    m_configItem   = configItem;
    m_defaultColor = QColor(configItem.value<QColor>());
    m_customColor  = QColor(configItem.value<QColor>());

    m_choiceLabel = new QLabel(this);

    m_radioWidget        = new QWidget(view());
    m_radioLayout        = new QVBoxLayout(m_radioWidget);
    m_defaultRadioButton = new RadioButton(tr("Default color"), m_radioWidget);
    m_customRadioButton  = new RadioButton(tr("Custom color"), m_radioWidget);
    m_buttonGroup        = new QButtonGroup(this);

    m_customColorWidget = new QWidget(view());
    m_customColorLayout = new QHBoxLayout(m_customColorWidget);
    m_customLabel       = new QLabel(tr("Custom color"), m_customColorWidget);
    m_chooseColorButton = new QPushButton(tr("Choose color"), m_customColorWidget);

    initWidget();
}

void CustomColorSettingCard::initWidget()
{
    initLayout();

    if (m_defaultColor != m_customColor) {
        m_customRadioButton->setChecked(true);
        m_chooseColorButton->setEnabled(true);
    } else {
        m_defaultRadioButton->setChecked(true);
        m_chooseColorButton->setEnabled(false);
    }

    m_choiceLabel->setText(m_buttonGroup->checkedButton()->text());
    m_choiceLabel->adjustSize();

    m_chooseColorButton->setObjectName("chooseColorButton");

    connect(m_buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this,
            &CustomColorSettingCard::onRadioButtonClicked);
    connect(m_chooseColorButton, &QPushButton::clicked, this, &CustomColorSettingCard::showColorDialog);
}

void CustomColorSettingCard::initLayout()
{
    addWidget(m_choiceLabel);

    m_radioLayout->setSpacing(19);
    m_radioLayout->setAlignment(Qt::AlignTop);
    m_radioLayout->setContentsMargins(48, 18, 0, 18);
    m_buttonGroup->addButton(m_customRadioButton);
    m_buttonGroup->addButton(m_defaultRadioButton);
    m_radioLayout->addWidget(m_customRadioButton);
    m_radioLayout->addWidget(m_defaultRadioButton);
    m_radioLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);

    m_customColorLayout->setContentsMargins(48, 18, 44, 18);
    m_customColorLayout->addWidget(m_customLabel, 0, Qt::AlignLeft);
    m_customColorLayout->addWidget(m_chooseColorButton, 0, Qt::AlignRight);
    m_customColorLayout->setSizeConstraint(QHBoxLayout::SetMinimumSize);

    viewLayout->setSpacing(0);
    viewLayout->setContentsMargins(0, 0, 0, 0);
    addGroupWidget(m_radioWidget);
    addGroupWidget(m_customColorWidget);

    adjustViewSize();
}

void CustomColorSettingCard::onRadioButtonClicked(QAbstractButton *button)
{
    if (button->text() == m_choiceLabel->text()) {
        return;
    }

    m_choiceLabel->setText(button->text());
    m_choiceLabel->adjustSize();

    if (button == m_defaultRadioButton) {
        m_chooseColorButton->setDisabled(true);
        // qconfig.set(self.configItem, self.defaultColor)
        if (m_defaultColor != m_customColor) {
            emit colorChanged(m_defaultColor);
        }
    } else {
        m_chooseColorButton->setDisabled(false);
        // qconfig.set(self.configItem, self.customColor)
        if (m_defaultColor != m_customColor) {
            emit colorChanged(m_customColor);
        }
    }
}

void CustomColorSettingCard::showColorDialog()
{
    QScopedPointer<ColorDialog> w(new ColorDialog(m_defaultColor, tr("Choose color"), this->window()));
    w->updateStyle();
    connect(w.data(), &ColorDialog::colorChanged, this, &CustomColorSettingCard::onCustomColorChanged);
    w->exec();
}

void CustomColorSettingCard::onCustomColorChanged(const QColor &color)
{
    //    qconfig.set(self.configItem, color)
    m_customColor = color;
    emit colorChanged(color);
}
