#ifndef OPTIONSSETTINGCARD_H
#define OPTIONSSETTINGCARD_H

#include "ExpandGroupSettingCard.h"

#include <QLabel>

class OptionsSettingCard : public ExpandSettingCard
{
    Q_OBJECT
public:
    explicit OptionsSettingCard(const QVariant &configItem, const QString &title, const QString &content = "",
                                const QStringList &texts = {}, QWidget *parent = nullptr);

    void setValue(const QVariant &value) override;

private slots:
    void onButtonClicked(QAbstractButton *button);

signals:
    void optionChanged(const QVariant &);

private:
    QVariant m_configItem;
    QString m_configName;
    QStringList m_texts;
    QLabel *m_choiceLabel;
    QButtonGroup *m_buttonGroup;
};

#endif  // OPTIONSSETTINGCARD_H
