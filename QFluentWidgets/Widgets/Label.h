#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class PixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PixmapLabel(QWidget *parent = nullptr);

    void setPixmap(const QPixmap &pixmap);

    QPixmap pixmap() const;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

signals:

private:
    QPixmap m_pixmap;
};

#endif  // LABEL_H
