#include "SX_3D_Widget.h"

SX_Model *sx_model;
SX_Camera *sx_camera, *wire_camera;
vec3 cam_speed = vec3(0.0f, 0.0f, 0.0f);
vec3 cam_position = vec3(0.0f, 0.0f, 15.0f);
SX_Mesh *sx_mesh;


SX_3D_Widget::SX_3D_Widget(QWidget *parent):
    QOpenGLWidget(parent)
{
    init_format();
}

void SX_3D_Widget::initializeGL()
{
    sx_camera = new SX_Camera(":/shaders/vertex_shader.vert", ":/shaders/fragment_shader.frag", GL_TRIANGLES);
    wire_camera = new SX_Camera(":/shaders/vertex_shader.vert", ":/shaders/wire_fragment.frag", GL_LINES);

    sx_mesh = new SX_Mesh;
    sx_mesh->add_texture(":/textures/container.jpg", "Difuse_texture");
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

    sx_model->set_location(glm::vec3(-1.2f, -1.3f, -0.0f));
    sx_model->add_model(*sx_model);

    sx_model->set_location(glm::vec3(0.5f, -0.6f, -6.0f));
    sx_model->add_model(*sx_model);

    sx_model->set_location(glm::vec3(-0.5f, 1.2f, -6.0f));
    sx_model->add_model(*sx_model);

    sx_model->set_location(glm::vec3(0.7f, -3.4f, 5.5f));
    sx_model->add_model(*sx_model);

    sx_model->set_location(glm::vec3(-0.7f, 2.4f, -5.0f));
    sx_model->add_model(*sx_model);

    sx_model->set_location(glm::vec3(1.5f, -2.0f, 3.0f));
    sx_model->add_model(*sx_model);

    sx_model->set_location(glm::vec3(-1.5f, 2.4f, -3.0f));
    sx_model->add_model(*sx_model);

    sx_model->set_location(glm::vec3(0.0f, 0.0f, 0.0f));

    sx_camera->set_camera_params(glm::vec3(0.0f, 0.0f, 15.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));


    wire_camera->set_camera_params(glm::vec3(0.0f, 0.0f, 15.0f),
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, 1.0f, 0.0f));


    sx_camera->set_frustum_params(0.001f, 30.0f, 45.0f, 800.0f/600.0f);
    wire_camera->set_frustum_params(0.001f, 30.0f, 45.0f, 800.0f/600.0f);

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30);
}

void SX_3D_Widget::resizeGL(int __width, int __height)
{
    sx_camera->set_viewport(0, 0, __width, __height);
    wire_camera->set_viewport(0, 0, __width, __height);
}

void SX_3D_Widget::paintGL()
{
    static int counter = 0;
    counter += 1;

    cam_position  = cam_position + cam_speed;


    sx_camera->set_camera_params(cam_position, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    wire_camera->set_camera_params(cam_position, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

//    sx_model->get_model(0).rotate_arround_self_point(
//                glm::vec3(-0.25f, -0.25f, 0.0f),
//                glm::vec3(0.0f, 0.0f, 1.0f),
//                0.01f);

    if(counter < 300)
    {
        sx_model->draw(sx_camera);
    }
    else{
        sx_model->draw(wire_camera);
    }
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


void SX_3D_Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
    {
        cam_speed.z = -1.0e-1;
    }break;
    case Qt::Key_Down:
    {
        cam_speed.z = 1.0e-1;
    }break;
    case Qt::Key_Right:
    {
        cam_speed.x = 1.0e-1;
    }break;
    case Qt::Key_Left:
    {
        cam_speed.x = -1.0e-1;
    }break;
    }

    QOpenGLWidget::keyPressEvent(event);
}

void SX_3D_Widget::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
    {
        cam_speed.z = 0.0f;
    }break;
    case Qt::Key_Down:
    {
        cam_speed.z = 0.0f;
    }break;
    case Qt::Key_Right:
    {
        cam_speed.x = 0.0f;
    }break;
    case Qt::Key_Left:
    {
        cam_speed.x = 0.0f;
    }break;
    }
    QOpenGLWidget::keyReleaseEvent(event);
}
