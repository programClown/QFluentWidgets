#include "OptionsSettingCard.h"
#include "Layout/VBoxLayout.h"
#include "Widgets/Button.h"

#include <QButtonGroup>

OptionsSettingCard::OptionsSettingCard(const QVariant &configItem, const QString &title, const QString &content,
                                       const QStringList &texts, QWidget *parent)
    : ExpandSettingCard(NEWFLICON(FluentIcon, FOLDER_ADD), title, content, parent),
      m_configItem(configItem),
      m_configName(configItem.toString()),
      m_texts(texts)
{
    m_choiceLabel = new QLabel(this);
    m_buttonGroup = new QButtonGroup(this);

    addWidget(m_choiceLabel);

    // create buttons
    viewLayout->setSpacing(19);
    viewLayout->setContentsMargins(48, 18, 0, 18);

    for (int i = 0; i < m_texts.count(); ++i) {
        RadioButton *button = new RadioButton(m_texts.at(i), view());
        m_buttonGroup->addButton(button);
        viewLayout->addWidget(button);
        // button->setProperty(m_configName, option);
    }

    adjustViewSize();
    setValue(configItem);
    // configItem.valueChanged.connect(self.setValue)
    connect(m_buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this,
            &OptionsSettingCard::onButtonClicked);
}

void OptionsSettingCard::setValue(const QVariant &value)
{
    // qconfig.set(self.configItem, value)

    for (auto button : viewLayout->widgets()) {
        // isChecked = button.property(self.configName) == value
        bool isChecked  = false;
        RadioButton *rb = static_cast<RadioButton *>(button);
        rb->setChecked(isChecked);

        if (isChecked) {
            m_choiceLabel->setText(rb->text());
            m_choiceLabel->adjustSize();
        }
    }
}

void OptionsSettingCard::onButtonClicked(QAbstractButton *button)
{
    if (button->text() == m_choiceLabel->text()) {
        return;
    }

    // value = button.property(self.configName)
    // qconfig.set(self.configItem, value)

    m_choiceLabel->setText(button->text());
    m_choiceLabel->adjustSize();
    emit optionChanged(m_configItem);
}
