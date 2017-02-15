#pragma once

#include <vector>
using std::vector;

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QString>

#include <SX_Attributes.hpp>
#include <SX_Model.hpp>

//static const char *position_attribute_name = "vertex_position";
//static const char *color_attribute_name = "vertex_color";
//static const char *normal_attribute_name = "vertex_normal";
//static const char *model_matrix_uniform_name = "model_view_projection_matrix";
//static const char *texture_coordinate_attribute_name = "vertex_texture_coordinates";

//typedef struct program_location_descriptor_{
//    GLint position_location;
//    GLint color_location;
//    GLint normal_location;
//    GLint model_matrix_location;
//    GLint texture_coordinate_location;
//} program_location_descriptor;

typedef struct viewport_descriptor_
{
    float origin_x, origin_y;
    float width, height;
}viewport_descriptor;

static const viewport_descriptor standard_veiwport =
{
    .origin_x = 0.0f,
    .origin_y = 0.0f,
    .width = 800.0f,
    .height = 600.0f
};

class SX_Camera
{
public:

    SX_Camera(const QString &VertexShaderFilename, const QString &FragmentShaderFilename,
              viewport_descriptor screen_viewport = standard_veiwport,
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
    }

    void set_viewport(const viewport_descriptor &new_viewport)
    {
        viewport = new_viewport;
    }

    bool repaint(){
        if(is_ready()){

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

    program_location_descriptor attributes_locations;
    QOpenGLFunctions *gl_functions;
    QOpenGLShaderProgram *program;
    vector<SX_Model> models;
    viewport_descriptor viewport;

};
