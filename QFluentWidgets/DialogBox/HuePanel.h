#ifndef HUEPANEL_H
#define HUEPANEL_H

#include <QWidget>

class HuePanel : public QWidget
{
    Q_OBJECT
public:
    explicit HuePanel(const QColor &c = QColor(255, 0, 0), QWidget *parent = nullptr);

    void setColor(const QColor &c);
    int hue() const;  // 0 <= hue < 360
    int saturation() const;
    void setPickerPosition(const QPoint &pos);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void colorChanged(const QColor &);

private:
    QPixmap huePixmap;
    QColor color;
    QPoint pickerPos;
};

#endif  // HUEPANEL_H
