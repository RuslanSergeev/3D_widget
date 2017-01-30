#ifndef SX_3D_WIDGET_H
#define SX_3D_WIDGET_H

#include <QOpenGLWidget>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QOpenGLTexture;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

class SX_3D_Widget : public QOpenGLWidget
{
    Q_OBJECT
public:
    SX_3D_Widget();
    SX_3D_Widget(QWidget *parent = 0);
protected:
    virtual void initializeGL();
    virtual void resizeGL(int __width, int __height);
    virtual void paintGL();
private:
    void init_format();
    void dump_format();

    QOpenGLFunctions *gl;
    QOpenGLShaderProgram *program;
    QOpenGLTexture *texture;
    QOpenGLBuffer *vertex_buffer;
    QOpenGLBuffer *index_buffer;
    QOpenGLVertexArrayObject *array_object;
};

#endif // SX_3D_WIDGET_H
