#include "AcrylicLabel.h"
#include "Common/ImageUtils.h"

#include <QPainter>

BlurCoverThread::BlurCoverThread(QObject *parent)
    : QThread(parent), m_imagePath(""), m_blurRadius(0), m_maxSize(QSize())
{
}

void BlurCoverThread::blur(const QString &imagePath, int blurRadius, const QSize &maxSize)
{
    m_imagePath  = imagePath;
    m_blurRadius = blurRadius;
    m_maxSize    = maxSize;
    start();
}

void BlurCoverThread::run()
{
    if (m_imagePath.isEmpty()) {
        return;
    }
    GaussianBlur gaussBlur(m_blurRadius, 18);
    QPixmap pix;
    QImage origin(m_imagePath);
    if (m_maxSize.isValid()) {
        pix = QPixmap::fromImage(
                gaussBlur.BlurImage(origin.scaled(m_maxSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    } else {
        pix = QPixmap::fromImage(gaussBlur.BlurImage(origin));
    }
    emit blurFinished(pix);
}

AcrylicTextureLabel::AcrylicTextureLabel(const QColor &tintColor, const QColor &luminosityColor, double noiseOpacity,
                                         QWidget *parent)
    : QLabel(parent), m_tintColor(tintColor), m_luminosityColor(luminosityColor), m_noiseOpacity(noiseOpacity)
{
    m_noiseImage = QImage(":/qfluentwidgets/images/acrylic/noise.png");
    setAttribute(Qt::WA_TranslucentBackground);
}

void AcrylicTextureLabel::setTintColor(const QColor &color)
{
    m_tintColor = color;
    update();
}

void AcrylicTextureLabel::paintEvent(QPaintEvent * /*event*/)
{
    QImage acrylicTexture = QImage(64, 64, QImage::Format_ARGB32_Premultiplied);

    // paint luminosity layer
    acrylicTexture.fill(m_luminosityColor);

    // paint tint color
    QPainter painter(&acrylicTexture);
    painter.fillRect(acrylicTexture.rect(), m_tintColor);

    // paint noise
    painter.setOpacity(m_noiseOpacity);
    painter.drawImage(acrylicTexture.rect(), m_noiseImage);

    QBrush acrylicBrush(acrylicTexture);
    QPainter painterThis(this);
    painterThis.fillRect(rect(), acrylicBrush);
}

AcrylicLabel::AcrylicLabel(int blurRadius, const QColor &tintColor, const QColor &luminosityColor,
                           const QSize &maxBlurSize, QWidget *parent)
    : QLabel(parent),
      m_imagePath(""),
      m_blurRadius(blurRadius),
      m_maxBlurSize(maxBlurSize),
      m_blurPixmap(QPixmap()),
      m_acrylicTextureLabel(new AcrylicTextureLabel(tintColor, luminosityColor, 0.03, this)),
      m_blurThread(new BlurCoverThread(this))
{
    connect(m_blurThread, &BlurCoverThread::blurFinished, this, &AcrylicLabel::onBlurFinished);
}

void AcrylicLabel::setImage(const QString &imagePath)
{
    m_imagePath = imagePath;
    m_blurThread->blur(m_imagePath, m_blurRadius, m_maxBlurSize);
}

void AcrylicLabel::setTintColor(const QColor &color)
{
    m_acrylicTextureLabel->setTintColor(color);
}

void AcrylicLabel::setBlurRadius(int blurRadius)
{
    m_blurRadius = blurRadius;
}

void AcrylicLabel::setMaxBlurSize(const QSize &maxBlurSize)
{
    m_maxBlurSize = maxBlurSize;
}

void AcrylicLabel::onBlurFinished(const QPixmap &pixmap)
{
    m_blurPixmap = pixmap;
    setPixmap(m_blurPixmap);
    adjustSize();
}

void AcrylicLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    m_acrylicTextureLabel->resize(size());

    if ((!m_blurPixmap.isNull()) && (m_blurPixmap.size() != size())) {
        setPixmap(m_blurPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
}
