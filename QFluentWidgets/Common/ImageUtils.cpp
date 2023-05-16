#include "ImageUtils.h"
#include <QtMath>
#include <QDebug>

GaussianBlur::GaussianBlur(int blurRadius, float sigma) : m_blurRadius(blurRadius), m_sigma(sigma)
{
    CreateConvolutionMatrix();
}

QImage GaussianBlur::BlurImage(const QImage &in)
{
    if (in.isNull())
        return QImage();

    QImage image(in.size(), in.format());
    qDebug() << m_convolutionMatrix;

    int matrixSize     = m_convolutionMatrix.size();
    int halfMatrixSize = matrixSize / 2;

    float sumRed      = 0.0f;
    float sumBlue     = 0.0f;
    float sumGreen    = 0.0f;
    float matrixValue = 0.0f;
    int x1 = 0, y1 = 0;

    for (int x = 0; x < in.width(); ++x) {
        for (int y = 0; y < in.height(); ++y) {
            for (int kx = -halfMatrixSize; kx <= halfMatrixSize; ++kx) {
                x1 = ReflectIndex(x - kx, in.width());

                QColor color(in.pixel(x1, y));

                matrixValue = m_convolutionMatrix[kx + halfMatrixSize];

                sumRed += color.red() * matrixValue;
                sumBlue += color.blue() * matrixValue;
                sumGreen += color.green() * matrixValue;
            }

            QRgb finalColor = qRgb(sumRed, sumGreen, sumBlue);
            image.setPixel(x, y, finalColor);

            sumRed = sumGreen = sumBlue = 0.0f;
        }
    }

    for (int x = 0; x < in.width(); ++x) {
        for (int y = 0; y < in.height(); ++y) {
            for (int ky = -halfMatrixSize; ky <= halfMatrixSize; ++ky) {
                y1 = ReflectIndex(y - ky, in.height());

                QColor color(image.pixel(x, y1));
                matrixValue = m_convolutionMatrix[ky + halfMatrixSize];

                sumRed += color.red() * matrixValue;
                sumBlue += color.blue() * matrixValue;
                sumGreen += color.green() * matrixValue;
            }

            QRgb finalColor = qRgb(sumRed, sumGreen, sumBlue);
            image.setPixel(x, y, finalColor);

            sumRed = sumGreen = sumBlue = 0.0f;
        }
    }

    return image;
}

float GaussianBlur::GaussFunc(float x)
{
    // Gaussian function in one dimension
    return (1 / sqrtf(2 * M_PI * m_sigma * m_sigma)) * exp(-(x * x) / (2 * m_sigma * m_sigma));
}

void GaussianBlur::CreateConvolutionMatrix()
{
    int x = 0;
    size_t matrixSize, halfMatrixSize;

    matrixSize     = (size_t)(2 * m_blurRadius + 1);
    halfMatrixSize = matrixSize / 2;

    m_convolutionMatrix.resize(matrixSize);

    QVector<float>::iterator begin = m_convolutionMatrix.begin();
    QVector<float>::iterator end   = m_convolutionMatrix.end();

    x = -(int)halfMatrixSize;
    std::for_each(begin, end, [&](float &val) mutable {
        val = GaussFunc(x);
        x++;
    });

    // normalize the values in the convolution matrix
    float sum = std::accumulate(begin, end, 0.0f);

    std::for_each(begin, end, [&](float &val) { val /= sum; });
}

int GaussianBlur::ReflectIndex(int x, int length)
{
    if (x < 0)
        return -x - 1;
    else if (x >= length)
        return 2 * length - x - 1;

    return x;
}

float GaussianBlur::getSigma() const
{
    return m_sigma;
}

void GaussianBlur::setSigma(float value)
{
    m_sigma = value;
    CreateConvolutionMatrix();
}

int GaussianBlur::getBlurRadius() const
{
    return m_blurRadius;
}

void GaussianBlur::setBlurRadius(int value)
{
    m_blurRadius = value;
    CreateConvolutionMatrix();
}

GaussianBlur::~GaussianBlur() { }

/// 最好用聚类均值法
QColor DominantColor::getDominantColor(const QString &imagePath, int step)
{
    // step步长：在图片中取点时两点之间的间隔，若为1,则取所有点，适当将此值调大有利于提高运行速度
    int t = 0;                // 点数：记录一共取了多少个点，用于做计算平均数的分母
    QImage image(imagePath);  // 将Pixmap类型转为QImage类型
    int r = 0, g = 0, b = 0;  // 三元色的值，分别用于记录各个点的rgb值的和
    for (int i = 0; i < image.width(); i += step) {
        for (int j = 0; j < image.height(); j += step) {
            if (image.valid(i, j)) {           // 判断该点是否有效
                t++;                           // 点数加一
                QColor c = image.pixel(i, j);  // 获取该点的颜色
                r += c.red();                  // 将获取到的各个颜色值分别累加到rgb三个变量中
                b += c.blue();
                g += c.green();
            }
        }
    }
    return QColor(int(r / t) > 255 ? 255 : int(r / t), int(g / t) > 255 ? 255 : int(g / t),
                  int(b / t) > 255 ? 255 : int(b / t));
}
