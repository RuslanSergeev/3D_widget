#pragma once

#include <vector>
using std::vector;

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QString>

#include <SX_Attributes.hpp>
#include <SX_Model.hpp>

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

typedef struct camera_descriptor_
{
    glm::vec3 camera_location;
    glm::vec3 camera_target;
    glm::vec3 camera_up;
    float frustum_near;
    float frustum_far;
    float frustum_field_of_view;
    float aspect_ratio;
}camera_descriptor;

static const camera_descriptor standard_camera =
{
    .camera_location = glm::vec3(0.0f, 0.0f, 1.0f),
    .camera_target = glm::vec3(0.0f, 0.0f, 0.0f),
    .camera_up = glm::vec3(0.0f, 1.0f, 0.0f),
    .frustum_near = 1.0e-3,
    .frustum_far = 1.0f,
    .frustum_field_of_view = 45.0f,
    .aspect_ratio = 800.0f/600.0f
};

class SX_Camera
{
public:

    SX_Camera(const QString &VertexShaderFilename, const QString &FragmentShaderFilename,
              viewport_descriptor screen_viewport = standard_veiwport,
              camera_descriptor cam_descriptor_param = standard_camera,
              const size_t max_number_of_models = 100)
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
        set_program(new_program);
        configure_attributes_locations();

        QOpenGLFunctions *gl_functions = QOpenGLContext::currentContext()->functions();
        gl_functions->glClearColor(0.05f, 0.13f, 0.15f, 1.0f);
        gl_functions->glEnable(GL_DEPTH_TEST);
        gl_functions->glEnable(GL_MULTISAMPLE);
        set_functions(gl_functions);

        set_camera_descriptor(cam_descriptor_param);

        set_viewport(screen_viewport);
        models.reserve(max_number_of_models);
    }

    /*!
     * @fun bool configure_attributes_locations()
     * @brief Определяет расположение аттрибутов и униформов в памяти
     * шейдерной программы. Инициализирует структуру с "адресами" для последующей
     * передачи в меши модели.
    */
    bool configure_attributes_locations()
    {
        if(nullptr != program && program->isLinked())
        {
            attributes_locations.color_location = program->attributeLocation(color_attribute_name);
            attributes_locations.model_matrix_location = program->uniformLocation(model_matrix_uniform_name);
            attributes_locations.normal_location = program->attributeLocation(normal_attribute_name);
            attributes_locations.position_location = program->attributeLocation(position_attribute_name);
            attributes_locations.texture_coordinate_location = program->attributeLocation(texture_coordinate_attribute_name);
            return true;
        }
        return false;
    }

    bool add_model(SX_Model &new_model)
    {
        new_model.set_functions(gl_functions);
        new_model.set_program(program);
        new_model.set_attributes_locations(attributes_locations);

        const size_t size = models.size();
        if(size == models.capacity()){
            qDebug() << "warning, attempt to reallocate models space!";
            models.resize(2*size);
        }

        models.push_back(new_model);

        return true;
    }

    SX_Model &get_model(const int model_index){
        return models[model_index];
    }

    void set_viewport(const float origin_x, const float origin_y,
                      const float width, const float height)
    {
        viewport.origin_x = origin_x;
        viewport.origin_y = origin_y;
        viewport.width = width;
        viewport.height = height;
        set_aspect_ratio(width/height);
    }

    void set_viewport(const viewport_descriptor &new_viewport)
    {
        viewport = new_viewport;
        set_aspect_ratio(new_viewport.width/new_viewport.height);
    }

    bool repaint(){
        if(is_ready()){

            update_view_projection_matrix();

            gl_functions->glViewport(viewport.origin_x,
                                     viewport.origin_y,
                                     viewport.width,
                                     viewport.height);

            program->bind();
            auto current_model = models.begin();
            while(current_model != models.end())
            {
                current_model->draw();
                ++current_model;
            }

            program->release();
            return true;
        }
        else
        {
            return false;
        }
    }

    void set_location(glm::vec3 new_location)
    {
        cam_descriptor.camera_location = new_location;
    }

    void set_target(glm::vec3 new_target)
    {
        cam_descriptor.camera_target = new_target;
    }

    void set_up_direction(glm::vec3 new_up_direction)
    {
        cam_descriptor.camera_up = new_up_direction;
    }

    void set_frustum_near(float new_frustum_near)
    {
        cam_descriptor.frustum_near = new_frustum_near;
    }

    void set_frustum_far(float new_frustum_far)
    {
        cam_descriptor.frustum_far = new_frustum_far;
    }

    void set_frustum_field_of_view(float new_frustum_field_of_view)
    {
        cam_descriptor.frustum_field_of_view = new_frustum_field_of_view;
    }

    void set_aspect_ratio(float new_aspect_ratio)
    {
        cam_descriptor.aspect_ratio = new_aspect_ratio;
    }

    void set_camera_descriptor(const camera_descriptor &new_cam_descriptor)
    {
        cam_descriptor = new_cam_descriptor;
    }

    void update_view_projection_matrix()
    {
        view_projection_matrix = glm::perspective(cam_descriptor.frustum_field_of_view,
                                                  cam_descriptor.aspect_ratio,
                                                  cam_descriptor.frustum_near,
                                                  cam_descriptor.frustum_far)
                *glm::lookAt(cam_descriptor.camera_location,
                             cam_descriptor.camera_target,
                             cam_descriptor.camera_up);
    }



    bool use()
    {
        if(is_ready())
        {
            program->bind();
            return true;
        }
        else
        {
            return false;
        }
    }


    bool release()
    {
        if(is_ready())
        {
            program->release();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool is_ready()
    {
        return program->isLinked() && (nullptr != gl_functions);
    }

private:

    bool set_functions(QOpenGLFunctions *OpenGLContext)
    {
        if(OpenGLContext)
        {
            gl_functions = OpenGLContext;
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
            program = QOpenGLProgram;
            return true;
        }
        else
        {
            return false;
        }
    }



    QOpenGLFunctions *gl_functions = nullptr;
    QOpenGLShaderProgram *program = nullptr;

    program_location_descriptor attributes_locations;
    vector<SX_Model> models;
    viewport_descriptor viewport;
    camera_descriptor cam_descriptor;

    glm::mat4x4 view_projection_matrix;
};
