#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

/// Indicator of switch button
class Indicator : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QColor sliderOnColor READ getSliderOnColor WRITE setSliderOnColor)
    Q_PROPERTY(QColor sliderOffColor READ getSliderOffColor WRITE setSliderOffColor)
    Q_PROPERTY(QColor sliderDisabledColor READ getSliderDisabledColor WRITE setSliderDisabledColor)

public:
    explicit Indicator(QWidget *parent = nullptr);

    void setChecked(bool checked);

    // QWidget interface
    QColor getSliderOnColor() const;
    void setSliderOnColor(const QColor &sliderOnColor);

    QColor getSliderOffColor() const;
    void setSliderOffColor(const QColor &sliderOffColor);

    QColor getSliderDisabledColor() const;
    void setSliderDisabledColor(const QColor &sliderDisabledColor);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

signals:
    void checkedChanged(bool);

private slots:
    void updateSliderPos();

private:
    QColor m_sliderOnColor;
    QColor m_sliderOffColor;
    QColor m_sliderDisabledColor;
    QTimer *m_timer;
    int m_padding;
    double m_sliderX;
    int m_sliderRadius;
    double m_sliderEndX;
    double m_sliderStep;
};

enum IndicatorPosition
{
    LEFT  = 0,
    RIGHT = 1
};

class SwitchButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int spacing READ getSpacing WRITE setSpacing)

public:
    explicit SwitchButton(const QString &text = "Off", QWidget *parent = nullptr,
                          IndicatorPosition indicatorPos = IndicatorPosition::LEFT);

    bool isChecked() const;
    void setChecked(bool checked);
    void toggleChecked();

    QString text() const;
    void setText(const QString &text);

    int getSpacing() const;
    void setSpacing(int spacing);

signals:
    void checkedChanged(bool);

private:
    void initWidget();

private:
    QString m_text;
    int m_spacing;
    IndicatorPosition m_indicatorPos;
    QHBoxLayout *m_hBox;
    Indicator *m_indicator;
    QLabel *m_label;
};

#endif  // SWITCHBUTTON_H
