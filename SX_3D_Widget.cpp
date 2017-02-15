#include "SX_3D_Widget.h"

#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QDebug>

#include <SX_Camera.hpp>
#include <SX_Model.hpp>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

using glm::vec3;
using glm::mat4x4;
using glm::quat;

SX_Model *sx_model;//, *wire_box;
SX_Camera *sx_camera;
SX_Mesh *sx_mesh;

//SX_Camera *wire_camera;
//SX_Model *arrow;

SX_3D_Widget::SX_3D_Widget(QWidget *parent):
    QOpenGLWidget(parent)
{
    init_format();
}

void SX_3D_Widget::initializeGL()
{


    sx_camera = new SX_Camera(":/shaders/vertex_shader.vert", ":/shaders/fragment_shader.frag");

    sx_mesh = new SX_Mesh;
    sx_mesh->add_texture(":/textures/container.jpg", "texture0");
    sx_mesh->add_point({glm::vec3(-0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
    sx_mesh->add_index(0);sx_mesh->add_index(2);sx_mesh->add_index(3);
    sx_mesh->add_index(0);sx_mesh->add_index(1);sx_mesh->add_index(2);
    sx_mesh->add_index(2);sx_mesh->add_index(6);sx_mesh->add_index(3);
    sx_mesh->add_index(3);sx_mesh->add_index(6);sx_mesh->add_index(7);
    sx_mesh->add_index(6);sx_mesh->add_index(4);sx_mesh->add_index(5);
    sx_mesh->add_index(4);sx_mesh->add_index(7);sx_mesh->add_index(6);
    sx_mesh->add_index(0);sx_mesh->add_index(4);sx_mesh->add_index(1);
    sx_mesh->add_index(1);sx_mesh->add_index(4);sx_mesh->add_index(5);
    sx_mesh->add_index(8);sx_mesh->add_index(9);sx_mesh->add_index(10);
    sx_mesh->add_index(11);sx_mesh->add_index(9);sx_mesh->add_index(8);
    sx_mesh->add_index(12);sx_mesh->add_index(13);sx_mesh->add_index(14);
    sx_mesh->add_index(12);sx_mesh->add_index(14);sx_mesh->add_index(15);

    sx_model = new SX_Model;
    sx_model->add_mesh(*sx_mesh);

    sx_camera->add_model(*sx_model);

//    sx_model = new SX_Model(GL_TRIANGLES);
//    sx_model->add_texture(":/textures/container.jpg", "texture0");
//    sx_model->add_point({glm::vec3(-0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    sx_model->add_point({glm::vec3( 0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
//    sx_model->add_point({glm::vec3( 0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
//    sx_model->add_point({glm::vec3(-0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
//    sx_model->add_point({glm::vec3(-0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
//    sx_model->add_point({glm::vec3( 0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
//    sx_model->add_point({glm::vec3( 0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
//    sx_model->add_point({glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    sx_model->add_point({glm::vec3( 0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
//    sx_model->add_point({glm::vec3( 0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    sx_model->add_point({glm::vec3( 0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
//    sx_model->add_point({glm::vec3( 0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
//    sx_model->add_point({glm::vec3(-0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
//    sx_model->add_point({glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    sx_model->add_point({glm::vec3(-0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
//    sx_model->add_point({glm::vec3(-0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
//    sx_model->add_index(0);sx_model->add_index(2);sx_model->add_index(3);
//    sx_model->add_index(0);sx_model->add_index(1);sx_model->add_index(2);
//    sx_model->add_index(2);sx_model->add_index(6);sx_model->add_index(3);
//    sx_model->add_index(3);sx_model->add_index(6);sx_model->add_index(7);
//    sx_model->add_index(6);sx_model->add_index(4);sx_model->add_index(5);
//    sx_model->add_index(4);sx_model->add_index(7);sx_model->add_index(6);
//    sx_model->add_index(0);sx_model->add_index(4);sx_model->add_index(1);
//    sx_model->add_index(1);sx_model->add_index(4);sx_model->add_index(5);
//    sx_model->add_index(8);sx_model->add_index(9);sx_model->add_index(10);
//    sx_model->add_index(11);sx_model->add_index(9);sx_model->add_index(8);
//    sx_model->add_index(12);sx_model->add_index(13);sx_model->add_index(14);
//    sx_model->add_index(12);sx_model->add_index(14);sx_model->add_index(15);

//    sx_camera->add_model(sx_model);


//    wire_camera = new SX_Camera(":/shaders/vertex_shader.vert", ":/shaders/wire_fragment.frag");

//    arrow = new SX_Model(GL_LINES);
//    arrow->add_point({glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    arrow->add_point({glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.99f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    arrow->add_point({glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.99f, 0.0f), glm::vec3(0.0f, 0.99f, 0.0f), glm::vec2(0.0f, 0.0f)});
//    arrow->add_point({glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec3(0.99f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
//    arrow->add_index(0);arrow->add_index(1);
//    arrow->add_index(0);arrow->add_index(2);
//    arrow->add_index(0);arrow->add_index(3);

//    wire_camera->add_model(arrow);
//    wire_camera->set_viewport(0, height() - 175, 175, 175);

//    wire_box = new SX_Model(GL_LINES);
//    wire_box->add_texture(":/textures/container.jpg", "texture0");
//    wire_box->add_point({glm::vec3(-0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    wire_box->add_point({glm::vec3( 0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
//    wire_box->add_point({glm::vec3( 0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
//    wire_box->add_point({glm::vec3(-0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
//    wire_box->add_point({glm::vec3(-0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
//    wire_box->add_point({glm::vec3( 0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
//    wire_box->add_point({glm::vec3( 0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
//    wire_box->add_point({glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    wire_box->add_point({glm::vec3( 0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
//    wire_box->add_point({glm::vec3( 0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    wire_box->add_point({glm::vec3( 0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
//    wire_box->add_point({glm::vec3( 0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
//    wire_box->add_point({glm::vec3(-0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
//    wire_box->add_point({glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
//    wire_box->add_point({glm::vec3(-0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
//    wire_box->add_point({glm::vec3(-0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
//    wire_box->add_index(0);wire_box->add_index(2);wire_box->add_index(3);
//    wire_box->add_index(0);wire_box->add_index(1);wire_box->add_index(2);
//    wire_box->add_index(2);wire_box->add_index(6);wire_box->add_index(3);
//    wire_box->add_index(3);wire_box->add_index(6);wire_box->add_index(7);
//    wire_box->add_index(6);wire_box->add_index(4);wire_box->add_index(5);
//    wire_box->add_index(4);wire_box->add_index(7);wire_box->add_index(6);
//    wire_box->add_index(0);wire_box->add_index(4);wire_box->add_index(1);
//    wire_box->add_index(1);wire_box->add_index(4);wire_box->add_index(5);
//    wire_box->add_index(8);wire_box->add_index(9);wire_box->add_index(10);
//    wire_box->add_index(11);wire_box->add_index(9);wire_box->add_index(8);
//    wire_box->add_index(12);wire_box->add_index(13);wire_box->add_index(14);
//    wire_box->add_index(12);wire_box->add_index(14);wire_box->add_index(15);

//    wire_camera->add_model(wire_box);

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

void SX_3D_Widget::resizeGL(int __width, int __height)
{
//    sx_camera->set_viewport(0, 0, __width, __height);
}

void SX_3D_Widget::paintGL()
{

//    static float angle = 0;
//    angle += 0.5;

//    mat4x4 mvp;
//    mvp = glm::perspective(45.0f, width()*1.0f/height(), 0.005f, 6.0f)
//            *glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f))
//            *glm::translate(mat4x4(), vec3(0, 0, 2*cos(glm::radians(angle))))
//            *glm::rotate(mat4x4(), glm::radians(angle), glm::vec3(1.0f, 0.0f, 1.0f));

//    glEnable(GL_DEPTH_TEST);

//    sx_camera->get_model(0)->set_model_view_projection(mvp);
//    sx_camera->repaint();

//    glDisable(GL_DEPTH_TEST);

//    wire_camera->get_model(0)->set_model_view_projection(mvp);
//    wire_camera->get_model(1)->set_model_view_projection(mvp);
//    wire_camera->repaint();

}


void SX_3D_Widget::init_format()
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(2);
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
