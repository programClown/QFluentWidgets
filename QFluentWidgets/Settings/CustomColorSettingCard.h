#ifndef CUSTOMCOLORSETTINGCARD_H
#define CUSTOMCOLORSETTINGCARD_H

#include "ExpandGroupSettingCard.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

class RadioButton;

class CustomColorSettingCard : public ExpandGroupSettingCard
{
    Q_OBJECT
public:
    explicit CustomColorSettingCard(const QVariant &configItem, FluentIconBase *icon, const QString &title,
                                    const QString &content = "", QWidget *parent = nullptr);

private:
    void initWidget();
    void initLayout();

private slots:
    void onRadioButtonClicked(QAbstractButton *button);
    void showColorDialog();
    void onCustomColorChanged(const QColor &color);

signals:
    void colorChanged(const QColor &);

private:
    QVariant m_configItem;
    QColor m_defaultColor;
    QColor m_customColor;
    QLabel *m_choiceLabel;
    QWidget *m_radioWidget;
    QVBoxLayout *m_radioLayout;
    RadioButton *m_defaultRadioButton;
    RadioButton *m_customRadioButton;
    QButtonGroup *m_buttonGroup;
    QWidget *m_customColorWidget;
    QHBoxLayout *m_customColorLayout;
    QLabel *m_customLabel;
    QPushButton *m_chooseColorButton;
};
#endif  // CUSTOMCOLORSETTINGCARD_H
