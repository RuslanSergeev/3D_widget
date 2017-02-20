#include "SX_3D_Widget.h"

SX_Model *sx_model;
SX_Camera *sx_camera;
SX_Mesh *sx_mesh;


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
    sx_mesh->add_point({glm::vec3(-0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.6f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f,-0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f,-0.25f, 0.25f), glm::vec3(0.0f, 0.6f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(1.0f, 0.0f)});
    sx_mesh->add_point({glm::vec3( 0.25f, 0.25f, 0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f, 0.25f,-0.25f), glm::vec3(0.95f, 0.0f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 1.0f)});
    sx_mesh->add_point({glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.95f, 0.6f,  0.25f), glm::vec3(0.0f, 0.0f, 0.99f), glm::vec2(0.0f, 0.0f)});
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

    sx_model->move_by_self_vector(glm::vec3(0.5f, 0.0f, 0.1f));
    sx_camera->add_model(*sx_model);

    sx_camera->set_camera_params(glm::vec3(0.0f, 0.0f, 2.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));

    sx_camera->set_frustum_params(0.01f, 12.0f, 45.0f, 800.0f/600.0f);

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(25);
}

void SX_3D_Widget::resizeGL(int __width, int __height)
{
    sx_camera->set_viewport(0, 0, __width, __height);
}

void SX_3D_Widget::paintGL()
{

    sx_camera->get_model(0).rotate_arround_self_point(
                glm::vec3(-0.25f, -0.25f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                0.01f);

    sx_camera->get_model(0).set_scaling(glm::vec3(1.5f, 2.0f, 1.0f));

    sx_camera->repaint();
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
