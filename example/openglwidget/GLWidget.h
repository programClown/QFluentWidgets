#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include "Logo.h"

QT_BEGIN_NAMESPACE

class QOpenGLTexture;
class QOpenGLShaderProgram;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

QT_END_NAMESPACE

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
    Q_PROPERTY(float z READ z WRITE setZ NOTIFY zChanged FINAL)
    Q_PROPERTY(float r READ r WRITE setR NOTIFY rChanged FINAL)
    Q_PROPERTY(float r2 READ r2 WRITE setR2 NOTIFY r2Changed FINAL)

public:
    explicit GLWidget(QWidget *parent = nullptr, Qt::WindowFlags f = {});
    ~GLWidget() override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    float z() const { return m_eye.z(); }
    void setZ(float v);

    float r() const { return m_r; }
    void setR(float v);

    float r2() const { return m_r2; }
    void setR2(float v);

private Q_SLOTS:
    void startSecondStage();

Q_SIGNALS:
    void zChanged();
    void rChanged();
    void r2Changed();

private:
    QOpenGLTexture *m_texture       = nullptr;
    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLBuffer *m_vbo            = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;
    Logo m_logo;
    int m_projMatrixLoc  = 0;
    int m_camMatrixLoc   = 0;
    int m_worldMatrixLoc = 0;
    int m_myMatrixLoc    = 0;
    int m_lightPosLoc    = 0;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_world;
    QVector3D m_eye;
    QVector3D m_target   = { 0, 0, -1 };
    bool m_uniformsDirty = true;
    float m_r            = 0;
    float m_r2           = 0;
};

#endif  // GLWIDGET_H
