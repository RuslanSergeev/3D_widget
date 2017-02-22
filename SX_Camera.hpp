#pragma once

#include <vector>
using std::vector;

#include <QtOpenGL>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::mat3x3;
using glm::mat4x4;



static const char *position_attribute_name = "vertex_position";
static const char *color_attribute_name = "vertex_color";
static const char *normal_attribute_name = "vertex_normal";
static const char *model_matrix_uniform_name = "model_view_projection_matrix";
static const char *texture_coordinate_attribute_name = "vertex_texture_coordinates";



typedef struct program_location_descriptor_{
    GLint position_location;
    GLint color_location;
    GLint normal_location;
    GLint model_matrix_location;
    GLint texture_coordinate_location;
} program_location_descriptor;

typedef struct viewport_descriptor_
{
    float origin_x;
    float origin_y;
    float width;
    float height;
}viewport_descriptor;

static const viewport_descriptor standard_veiwport =
{
    .origin_x = 0.0f,
    .origin_y = 0.0f,
    .width = 800.0f,
    .height = 600.0f
};

typedef struct frustum_descriptor_
{
    glm::vec3 camera_location;
    glm::vec3 camera_target;
    glm::vec3 camera_up;
    float frustum_near;
    float frustum_far;
    float frustum_field_of_view;
    float aspect_ratio;
}frustum_descriptor;



static const frustum_descriptor standard_frustum =
{
    .camera_location = glm::vec3(0.0f, 0.0f, 1.0f),
    .camera_target = glm::vec3(0.0f, 0.0f, 0.0f),
    .camera_up = glm::vec3(0.0f, 1.0f, 0.0f),
    .frustum_near = 1.0e-3,
    .frustum_far = 1.0f,
    .frustum_field_of_view = 45.0f,
    .aspect_ratio = 800.0f/600.0f
};

typedef struct renderer_descriptor_
{
    QOpenGLFunctions *gl_functions = nullptr;
    QOpenGLShaderProgram *program = nullptr;
    program_location_descriptor attributes_locations;
    GLuint primitive_type = GL_TRIANGLES;
}renderer_descriptor;

class SX_Camera
{
public:

    SX_Camera(const QString &VertexShaderFilename,
              const QString &FragmentShaderFilename,
              const GLuint primitive_type = GL_TRIANGLES,
              viewport_descriptor screen_viewport = standard_veiwport,
              frustum_descriptor cam_descriptor_param = standard_frustum)
    {
        QOpenGLShaderProgram *new_program = new QOpenGLShaderProgram;
        if(!new_program->addShaderFromSourceFile(QOpenGLShader::Vertex, VertexShaderFilename))
        {
            qDebug() << "Unable to add vertex shader:";
            qDebug() << new_program->log();
        }
        else if(!new_program->addShaderFromSourceFile(QOpenGLShader::Fragment, FragmentShaderFilename))
        {
            qDebug() << "Unable to add fragment shader:";
            qDebug() << new_program->log();
        }
        else if(!new_program->link())
        {
            qDebug() << "Unable to link shader new_program:";
            qDebug() << new_program->log();
        }
        else
        {
            qDebug() << "shader new_program succesfully linked.";
        }
        new_program->create();

        camera_renderer.primitive_type = primitive_type;

        set_program(new_program);
        configure_attributes_locations();

        QOpenGLFunctions *gl_functions = QOpenGLContext::currentContext()->functions();
        gl_functions->glClearColor(0.05f, 0.13f, 0.15f, 1.0f);
        gl_functions->glEnable(GL_DEPTH_TEST);
        gl_functions->glEnable(GL_MULTISAMPLE);
        set_functions(gl_functions);

        set_camera_descriptor(cam_descriptor_param);

        set_viewport(screen_viewport);
    }

    /*!
     * @fun bool configure_attributes_locations()
     * @brief Определяет расположение аттрибутов и униформов в памяти
     * шейдерной программы. Инициализирует структуру с "адресами" для последующей
     * передачи в меши модели.
    */
    bool configure_attributes_locations()
    {
        if(nullptr != camera_renderer.program &&
                camera_renderer.program->isLinked())
        {
            camera_renderer.attributes_locations.color_location =
                    camera_renderer.program->attributeLocation(color_attribute_name);
            camera_renderer.attributes_locations.model_matrix_location =
                    camera_renderer.program->uniformLocation(model_matrix_uniform_name);
            camera_renderer.attributes_locations.normal_location =
                    camera_renderer.program->attributeLocation(normal_attribute_name);
            camera_renderer.attributes_locations.position_location =
                    camera_renderer.program->attributeLocation(position_attribute_name);
            camera_renderer.attributes_locations.texture_coordinate_location =
                    camera_renderer.program->attributeLocation(texture_coordinate_attribute_name);
            return true;
        }
        return false;
    }

    void set_viewport(const float origin_x, const float origin_y,
                      const float width, const float height)
    {
        camera_needs_update = true;
        viewport.origin_x = origin_x;
        viewport.origin_y = origin_y;
        viewport.width = width;
        viewport.height = height;
        set_aspect_ratio(width/height);
    }

    void set_viewport(const viewport_descriptor &new_viewport)
    {
        camera_needs_update = true;
        viewport = new_viewport;
        set_aspect_ratio(new_viewport.width/new_viewport.height);
    }

    void set_aspect_ratio(float aspect_ratio)
    {
        camera_needs_update = true;
        frustum.aspect_ratio = aspect_ratio;
    }

    void set_camera_params(glm::vec3 location,
                           glm::vec3 target,
                           glm::vec3 up_direction)
    {
        camera_needs_update = true;
        frustum.camera_location = location;
        frustum.camera_target = target;
        frustum.camera_up = up_direction;
    }

    void set_frustum_params(float frustum_near,
                            float frustum_far,
                            float field_of_view,
                            float aspect_ratio)
    {
        camera_needs_update = true;
        frustum.frustum_near = frustum_near;
        frustum.frustum_far = frustum_far;
        frustum.frustum_field_of_view = field_of_view;
        frustum.aspect_ratio = aspect_ratio;
    }

    void set_camera_descriptor(const frustum_descriptor &new_cam_descriptor)
    {
        camera_needs_update = true;
        frustum = new_cam_descriptor;
    }

    void update_view_projection_matrix()
    {
        if(camera_needs_update)
        {
            camera_needs_update = false;

            view_projection_matrix = glm::perspective(frustum.frustum_field_of_view,
                                                      frustum.aspect_ratio,
                                                      frustum.frustum_near,
                                                      frustum.frustum_far)
                    * glm::lookAt(frustum.camera_location,
                                  frustum.camera_target,
                                  frustum.camera_up);
        }
    }

    bool use()
    {
        if(!camera_in_use && is_ready())
        {
            camera_in_use = true;

            camera_renderer.program->bind();
            update_view_projection_matrix();

            camera_renderer.gl_functions->glViewport(viewport.origin_x,
                                                     viewport.origin_y,
                                                     viewport.width,
                                                     viewport.height);

            return true;
        }
        else
        {
            return false;
        }
    }


    bool release()
    {
        if(camera_in_use && is_ready())
        {
            camera_in_use = false;
            camera_renderer.program->release();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool is_ready()
    {
        return camera_renderer.program->isLinked() &&
                (nullptr != camera_renderer.gl_functions);
    }

    renderer_descriptor camera_renderer; //TODO: убрать в приватные!
    mat4x4 view_projection_matrix = mat4x4(1.0f);

private:

    bool set_functions(QOpenGLFunctions *OpenGLContext)
    {
        if(OpenGLContext)
        {
            camera_renderer.gl_functions = OpenGLContext;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool set_program(QOpenGLShaderProgram *QOpenGLProgram)
    {
        if(QOpenGLProgram->isLinked())
        {
            camera_renderer.program = QOpenGLProgram;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool camera_needs_update = true;
    bool camera_in_use = false;

    viewport_descriptor viewport;
    frustum_descriptor frustum;
};
