#ifndef SETTINGS_H
#define SETTINGS_H

#include <QBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include "Common/Icon.h"
#include "Common/Config.h"

class ComboBox;
class IconWidget;
class SwitchButton;
class Slider;
class HyperlinkButton;

class SettingCard : public QFrame
{
    Q_OBJECT
public:
    SettingCard(FluentIconBase *ficon, const QString &title, const QString &content = "", QWidget *parent = nullptr);

    void setTitle(const QString &title);
    void setContent(const QString &content);
    FluentIconBase *ficon() const;
    virtual void setValue(const QVariant &value);

    QHBoxLayout *hBoxLayout;
    QVBoxLayout *vBoxLayout;

private:
    QScopedPointer<FluentIconBase> m_ficon;
    IconWidget *m_iconLabel;
    QLabel *m_titleLabel;
    QLabel *m_contentLabel;
};

/// Setting card with switch button
class SwitchSettingCard : public SettingCard
{
    Q_OBJECT
public:
    SwitchSettingCard(FluentIconBase *ficon, const QString &title, const QString &content = "",
                      Qfw::ConfigItem *configItem = nullptr, QWidget *parent = nullptr);

    void setValue(const QVariant &value) override;
    bool isChecked() const;

signals:
    void checkedChanged(bool);

private slots:
    void onCheckedChanged(bool isChecked);

private:
    Qfw::ConfigItem *m_configItem;
    SwitchButton *m_switchButton;
};

class RangeSettingCard : public SettingCard
{
    Q_OBJECT
public:
    RangeSettingCard(int min, int max, int value, FluentIconBase *ficon, const QString &title,
                     const QString &content = "", QWidget *parent = nullptr);

    void setValue(const QVariant &value) override;

signals:
    void valueChanged(int);

private slots:
    void onValueChanged(int value);

private:
    int m_min;
    int m_max;
    Slider *m_slider;
    QLabel *m_valueLabel;
};

class PushSettingCard : public SettingCard
{
    Q_OBJECT
public:
    PushSettingCard(const QString &text, FluentIconBase *ficon, const QString &title, const QString &content = "",
                    QWidget *parent = nullptr);

    QPushButton *button;

signals:
    void clicked();
};

class PrimaryPushSettingCard : public PushSettingCard
{
    Q_OBJECT
public:
    PrimaryPushSettingCard(const QString &text, FluentIconBase *ficon, const QString &title,
                           const QString &content = "", QWidget *parent = nullptr);
};

class HyperlinkCard : public SettingCard
{
    Q_OBJECT
public:
    HyperlinkCard(const QString &url, const QString &text, FluentIconBase *ficon, const QString &title,
                  const QString &content = "", QWidget *parent = nullptr);

    HyperlinkButton *linkButton;
};

class ColorPickerButton : public QPushButton
{
    Q_OBJECT
public:
    ColorPickerButton(const QColor &color, const QString &title, QWidget *parent = nullptr);

    void setColor(const QColor &color);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void colorChanged(const QColor &);

private slots:
    void showColorDialog();
    void onColorChanged(const QColor &color);

private:
    QString m_title;
    QColor m_color;
};

class ColorSettingCard : public SettingCard
{
    Q_OBJECT
public:
    explicit ColorSettingCard(const QColor &color, FluentIconBase *ficon, const QString &title,
                              const QString &content = "", QWidget *parent = nullptr);

    void setValue(const QVariant &value) override;

signals:
    void colorChanged(const QColor &);

private slots:
    void onColorChanged(const QColor &color);

private:
    QColor m_color;
    ColorPickerButton *m_colorPicker;
};

class ComboBoxSettingCard : public SettingCard
{
    Q_OBJECT
public:
    explicit ComboBoxSettingCard(const QString &defaultText, const QHash<QString, QVariant> &option,
                                 FluentIconBase *ficon, const QString &title, const QString &content = "",
                                 QWidget *parent = nullptr);

    void setValue(const QVariant &value) override;

private slots:
    void onCurrentIndexChanged(int index);

private:
    QHash<QString, QVariant> m_options;
    ComboBox *m_comboBox;
};
#endif  // SETTINGS_H
