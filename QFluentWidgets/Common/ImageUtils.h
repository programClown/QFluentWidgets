#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QPixmap>

class GaussianBlur
{
public:
    GaussianBlur(int blurRadius, float sigma);

    QImage BlurImage(const QImage &in);

    float GaussFunc(float x);

    int getBlurRadius() const;
    void setBlurRadius(int value);

    float getSigma() const;
    void setSigma(float value);

    ~GaussianBlur();

private:
    QVector<float> m_convolutionMatrix;

    int ReflectIndex(int x, int length);
    void CreateConvolutionMatrix();

    int m_blurRadius;
    float m_sigma;
};

class DominantColor
{
public:
    /// 获取主色
    static QColor getDominantColor(const QString &imagePath, int step = 20);
};

#endif  // IMAGEUTILS_H
