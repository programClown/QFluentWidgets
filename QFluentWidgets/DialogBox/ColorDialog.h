#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QLabel>
#include <QWidget>
#include "Widgets/Slider.h"
#include "Widgets/LineEdit.h"
#include "MaskDialogBase.h"

class HuePanel;
class ScrollArea;
class PrimaryPushButton;

class BrightnessSlider : public Slider
{
    Q_OBJECT
public:
    explicit BrightnessSlider(const QColor &color, QWidget *parent = nullptr);

    void setColor(const QColor &color);

signals:
    void colorChanged(const QColor &);

private slots:
    void onValueChanged(int value);

private:
    QColor m_color;
};

class ColorCard : public QWidget
{
    Q_OBJECT
public:
    explicit ColorCard(const QColor &color, QWidget *parent = nullptr);

    void setColor(const QColor &color);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QColor m_color;
};

class ColorLineEdit : public LineEdit
{
    Q_OBJECT
public:
    explicit ColorLineEdit(const QString &value, QWidget *parent = nullptr);

private slots:
    void onTextEdited(const QString &text);

signals:
    void valueChanged(const QString &);
};

class HexColorLineEdit : public ColorLineEdit
{
    Q_OBJECT
public:
    explicit HexColorLineEdit(const QColor &color, QWidget *parent = nullptr);

    void setColor(const QColor &color);

    QLabel *prefixLabel;
};

class ColorDialog : public MaskDialogBase
{
    Q_OBJECT
public:
    explicit ColorDialog(const QColor &color, const QString &title, QWidget *parent = nullptr);

    void setColor(const QColor &color, bool movePicker = true);
    void updateStyle();

signals:
    void colorChanged(const QColor &);

private:
    void initWidget();
    void setQss();
    void initLayout();
    void connectSignalToSlot();

private slots:
    void onYesButtonClicked();
    void onHueChanged(const QColor &color);
    void onBrightnessChanged(const QColor &color);
    void onRedChanged(const QString &value);
    void onGreenChanged(const QString &value);
    void onBlueChanged(const QString &value);
    void onHexColorChanged(const QString &value);

private:
    QColor m_oldColor;
    QColor m_color;
    ScrollArea *m_scrollArea;
    QWidget *m_scrollWidget;
    QFrame *m_buttonGroup;
    PrimaryPushButton *m_yesButton;
    QPushButton *m_cancelButton;
    QLabel *m_titleLabel;
    HuePanel *m_huePanel;
    ColorCard *m_newColorCard;
    ColorCard *m_oldColorCard;
    BrightnessSlider *m_brightSlider;
    QLabel *m_editLabel;
    QLabel *m_redLabel;
    QLabel *m_greenLabel;
    QLabel *m_blueLabel;
    HexColorLineEdit *m_hexLineEdit;
    ColorLineEdit *m_redLineEdit;
    ColorLineEdit *m_greenLineEdit;
    ColorLineEdit *m_blueLineEdit;
    QVBoxLayout *m_vBoxLayout;
};

#endif  // COLORDIALOG_H
