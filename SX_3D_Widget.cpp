#include "SX_3D_Widget.h"

#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "QDebug"


static const float vertices_data[] = {
    /*position                color             normal            texture*/
    -0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,   0.9f,  0.4f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f
};

static const GLuint indices_data[] = {
    0, 1, 2,
    2, 1, 3
};


SX_3D_Widget::SX_3D_Widget()
{
    init_format();
}

SX_3D_Widget::SX_3D_Widget(QWidget *parent):
    QOpenGLWidget(parent)
{
    init_format();
}

void SX_3D_Widget::initializeGL()
{
    gl = QOpenGLContext::currentContext()->functions();
    gl->glClearColor(0.31f, 0.1f, 0.45f, 1.0f);
    gl->glEnable(GL_DEPTH_TEST);

    program = new QOpenGLShaderProgram;
    if(!program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex_shader.vert"))
    {
        qDebug() << "Unable to add vertex shader:";
        qDebug() << program->log();
        close();
    }
    else if(!program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment_shader.frag"))
    {
        qDebug() << "Unable to add fragment shader:";
        qDebug() << program->log();
        close();
    }
    else if(!program->link())
    {
        qDebug() << "Unable to link shader program:";
        qDebug() << program->log();
        close();
    }
    else
    {
        qDebug() << "shader programm succesfully linked.";
    }

    if(!program->bind()){
        qDebug() << "cannot bind current programm";
    }

    texture = new QOpenGLTexture(QImage(":/textures/container.jpg"));
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    array_object = new QOpenGLVertexArrayObject;
    array_object->create();
    array_object->bind();

    vertex_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertex_buffer->create();
    vertex_buffer->bind();
    vertex_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertex_buffer->allocate(vertices_data, sizeof(vertices_data));

    index_buffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    index_buffer->create();
    index_buffer->bind();
    index_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    index_buffer->allocate(indices_data, sizeof(indices_data));

    program->enableAttributeArray("vertex_position");
    program->setAttributeBuffer("vertex_position", GL_FLOAT, 0*sizeof(float), 3, 11*sizeof(float));

    program->enableAttributeArray("vertex_color");
    program->setAttributeBuffer("vertex_color", GL_FLOAT, 3*sizeof(float), 3, 11*sizeof(float));

    program->enableAttributeArray("vertex_normal");
    program->setAttributeBuffer("vertex_normal", GL_FLOAT, 6*sizeof(float), 3, 11*sizeof(float));

    program->enableAttributeArray("vertex_texture_coordinates");
    program->setAttributeBuffer("vertex_texture_coordinates", GL_FLOAT, 9*sizeof(float), 2, 11*sizeof(float));


    array_object->release();
    vertex_buffer->release();
    index_buffer->release();
    program->release();
}

void SX_3D_Widget::resizeGL(int __width, int __height)
{
    gl->glViewport(0, 0, __width, __height);
}

void SX_3D_Widget::paintGL()
{
    gl->glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    program->bind();
    array_object->bind();

    texture->bind();
    program->setUniformValue("texture0", 0);

    gl->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    program->release();
}


void SX_3D_Widget::init_format()
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
}

void SX_3D_Widget::dump_format()
{
    qDebug() << QString::number(format().majorVersion()) + "." + QString::number(format().minorVersion());
    if(QSurfaceFormat::CoreProfile == format().profile())
    {
        qDebug() << "Core profile";
    }
    else
    {
        qDebug() << "Immediate profile";
    }
}
