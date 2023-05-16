#ifndef ACRYLICLABEL_H
#define ACRYLICLABEL_H

#include <QThread>
#include <QLabel>

class BlurCoverThread : public QThread
{
    Q_OBJECT

public:
    BlurCoverThread(QObject *parent = nullptr);

    void blur(const QString &imagePath, int blurRadius = 6, const QSize &maxSize = QSize(450, 450));

    // QThread interface
protected:
    void run() override;

signals:
    void blurFinished(const QPixmap &pix);

private:
    QString m_imagePath;
    int m_blurRadius;
    QSize m_maxSize;
};
class AcrylicTextureLabel : public QLabel
{
    Q_OBJECT
public:
    AcrylicTextureLabel(const QColor &tintColor, const QColor &luminosityColor, double noiseOpacity = 0.03,
                        QWidget *parent = nullptr);

    void setTintColor(const QColor &color);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_tintColor;
    QColor m_luminosityColor;
    double m_noiseOpacity;
    QImage m_noiseImage;
};

class AcrylicLabel : public QLabel
{
    Q_OBJECT
public:
    explicit AcrylicLabel(int blurRadius, const QColor &tintColor,
                          const QColor &luminosityColor = QColor(255, 255, 255, 0), const QSize &maxBlurSize = QSize(),
                          QWidget *parent = nullptr);

    void setImage(const QString &imagePath);
    void setTintColor(const QColor &color);
    void setBlurRadius(int blurRadius);
    void setMaxBlurSize(const QSize &maxBlurSize);

private slots:
    void onBlurFinished(const QPixmap &pixmap);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;

signals:

private:
    QString m_imagePath;
    int m_blurRadius;
    QSize m_maxBlurSize;
    QPixmap m_blurPixmap;
    AcrylicTextureLabel *m_acrylicTextureLabel;
    BlurCoverThread *m_blurThread;
};

#endif  // ACRYLICLABEL_H
