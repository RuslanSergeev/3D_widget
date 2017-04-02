#pragma once

#include <SX_Camera.hpp>

#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <vector>
#include <list>
using std::vector;
using std::list;

#include <cstring>
#include <cstddef>


class SX_Mesh
{
public:

    /* TODO: разбить этот набор, чтобы у пользователя была возможность
     * выбора, как много буфферных объектов он желает использовать.
     * +ввести соответствующие enabler-ы.
     * (возможно тригающиеся в конструкторах вершин)
    */
    typedef struct mesh_vertex_descriptor_{
        glm::vec3 vertex_position;                  /*! Координаты точки в пространстве модели. */
        glm::vec3 vertex_color;                        /*! Цвет вершины. */
        glm::vec3 vertex_normal;                         /*! Нормаль в вершине для алгоритмов шейдинга. */
        glm::vec2 Ambient_texture_coordinates;          /*! Координаты вершины в пространстве координат текстуры. */
        glm::vec2 Diffuse_texture_coordinates;          /*! Координаты вершины в пространстве координат текстуры. */
        glm::vec2 Specular_texture_coordinates;          /*! Координаты вершины в пространстве координат текстуры. */
        glm::vec2 Normals_texture_coordinates;          /*! Координаты вершины в пространстве координат текстуры. */
    } mesh_vertex_descriptor;

    typedef struct mesh_data_descriptor_{
        vector<mesh_vertex_descriptor> points_array;    //набор вершин, важно последоватеьлное расположение в ОЗУ.
        vector<GLuint> indexes_array;                   //набор индексов вершин, важно последоватеьлное расположение в ОЗУ.
        list<QOpenGLTexture *> textures_array;          //набор текстур, последовательное расположение в памяти не необходимо.
        list<const char *> sampler_names;               //набор имён переменных семплеров в шейдерных программах.
    } mesh_data_descriptor;

    /*!
     * @enum enum mesh_status_type
     * @brief Перечисление состояний меша.
    */
    enum mesh_status_type{
        mesh_buffers_not_ready,
        mesh_buffers_ready
    };

    void draw(SX_Camera *draw_camera)
    {

        if(!is_ready() || camera_changed(draw_camera))
        {
            setup_buffers(draw_camera);
        }
        activate_mesh_textures(draw_camera);
        VertexArrayObject->bind();
        draw_camera->camera_renderer.gl_functions->glDrawElements(draw_camera->camera_renderer.primitive_type,
                                                                  mesh_data.indexes_array.size(), GL_UNSIGNED_INT, 0);
        VertexArrayObject->release();
        deactivate_mesh_textures(draw_camera);

    }


    //TODO: в финальной реализации необходимо переместить текстуру в класс модели,
    //иначе получается очень большой перерасход памяти.
    bool add_texture(const char *texture_filename, const char *sampler_name)
    {
        QOpenGLTexture *new_texture = new QOpenGLTexture(QImage(texture_filename).mirrored());
        if(!new_texture)
        {
            return false;
        }
        else
        {
            new_texture->create();
            new_texture->generateMipMaps();
            new_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            new_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
            new_texture->setWrapMode(QOpenGLTexture::Repeat);

            return add_texture(new_texture, sampler_name);
        }
    }

    bool add_texture(QOpenGLTexture *new_texture, const char *sampler_name)
    {
        if(!textures_full() && new_texture->isCreated())
        {
            mesh_data.textures_array.push_back(new_texture);
            mesh_data.sampler_names.push_back(sampler_name);
            return true;
        }
        return false;
    }

    void activate_mesh_textures(SX_Camera *draw_camera){
        auto current_texture = mesh_data.textures_array.begin();
        auto current_sampler = mesh_data.sampler_names.begin();
        GLuint texture_index  = 0;

        while(current_texture != mesh_data.textures_array.end())
        {
            draw_camera->camera_renderer.program->setUniformValue(*current_sampler, texture_index);
            draw_camera->camera_renderer.gl_functions->glActiveTexture(GL_TEXTURE0 + texture_index);
            draw_camera->camera_renderer.gl_functions->glBindTexture(GL_TEXTURE_2D, (*current_texture)->textureId());

            ++current_texture;
            ++current_sampler;
            ++texture_index;
        }
    }

    void deactivate_mesh_textures(SX_Camera *draw_camera)
    {
        for(size_t texture_index = 0; texture_index < mesh_data.textures_array.size(); ++texture_index)
        {
            draw_camera->camera_renderer.gl_functions->glActiveTexture(GL_TEXTURE0 + texture_index);
            draw_camera->camera_renderer.gl_functions->glBindTexture(GL_TEXTURE_2D, 0);
        }
    }


    void add_point(glm::vec3 vertex_position = glm::sphericalRand(1.0f),
                   glm::vec3 vertex_color = glm::sphericalRand(1.0f),
                   glm::vec3 vertex_normal = glm::sphericalRand(1.0f),
                   glm::vec2 Ambient_texture_coordinates = glm::circularRand(1.0f),
                   glm::vec2 Diffuse_texture_coordinates = glm::circularRand(1.0f),
                   glm::vec2 Specular_texture_coordinates = glm::circularRand(1.0f),
                   glm::vec2 Normals_texture_coordinates = glm::circularRand(1.0f))
    {
        mesh_data.points_array.push_back({vertex_position,
                                          vertex_color,
                                          vertex_normal,
                                          Ambient_texture_coordinates,
                                          Diffuse_texture_coordinates,
                                          Specular_texture_coordinates,
                                          Normals_texture_coordinates});
    }

    void add_index(const GLuint new_index)
    {
        mesh_data.indexes_array.push_back(new_index);
    }

    bool is_ready() const{
        return mesh_buffers_ready == status;
    }

    bool camera_changed(SX_Camera *new_draw_camera)
    {
        GLuint new_program_id = new_draw_camera->camera_renderer.program->programId();

        if(!old_programm_id_initialized)
        {
            old_programm_id_initialized = true;
            old_programm_id = new_program_id;
        }


        if(old_programm_id != new_program_id)
        {
            old_programm_id = new_program_id;
            return true;
        }
        else
        {
            return false;
        }
    }

    operator GLuint() const{
        return VertexArrayObject->objectId();
    }

private:

    bool textures_full() const
    {
        GLint max_textures;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_textures);
        return mesh_data.textures_array.size() == static_cast<size_t>(max_textures);
    }


    void enable_attributes()
    {

    }

    void enable_attributes(SX_Camera *draw_camera)
    {
        draw_camera->camera_renderer.program->enableAttributeArray(draw_camera->camera_renderer.attributes_locations.position_location);
        draw_camera->camera_renderer.program->enableAttributeArray(draw_camera->camera_renderer.attributes_locations.color_location);
        draw_camera->camera_renderer.program->enableAttributeArray(draw_camera->camera_renderer.attributes_locations.normal_location);
        draw_camera->camera_renderer.program->enableAttributeArray(draw_camera->camera_renderer.attributes_locations.Ambient_texture_coordinate_location);
        draw_camera->camera_renderer.program->enableAttributeArray(draw_camera->camera_renderer.attributes_locations.Diffuse_texture_coordinate_location);
        draw_camera->camera_renderer.program->enableAttributeArray(draw_camera->camera_renderer.attributes_locations.Specular_texture_coordinate_location);
        draw_camera->camera_renderer.program->enableAttributeArray(draw_camera->camera_renderer.attributes_locations.Normals_texture_coordinate_location);
    }

    /*!
     * @brief Настройка VAO, VBO, EBO буфферов модели.
     * Настройка происходит во время первой отрисовки
     * (buffer object streaming пока не поддерживается)
    */
    void setup_buffers(SX_Camera *draw_camera)
    {
        VertexBufferObject = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        VertexBufferObject->create();
        VertexBufferObject->setUsagePattern(QOpenGLBuffer::StaticDraw);

        ElementBufferObject = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        ElementBufferObject->create();
        ElementBufferObject->setUsagePattern(QOpenGLBuffer::StaticDraw);

        VertexArrayObject = new QOpenGLVertexArrayObject;
        VertexArrayObject->create();
        VertexArrayObject->bind();

        enable_attributes(draw_camera);

        VertexBufferObject->bind();
        draw_camera->camera_renderer.gl_functions->glBufferData(GL_ARRAY_BUFFER,
                                                                mesh_data.points_array.size() * sizeof(mesh_vertex_descriptor),
                                                                mesh_data.points_array.data(),
                                                                GL_STREAM_DRAW);

        /* Конфигурация основных аттрибутов вертексов: координаты в пространстве модели, цвет, "нормаль" */
        draw_camera->camera_renderer.gl_functions->glVertexAttribPointer(draw_camera->camera_renderer.attributes_locations.position_location,
                                                                         sizeof(glm::vec3)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                                         sizeof(mesh_vertex_descriptor),
                                                                         (GLvoid *)offsetof(mesh_vertex_descriptor, vertex_position));

        draw_camera->camera_renderer.gl_functions->glVertexAttribPointer(draw_camera->camera_renderer.attributes_locations.color_location,
                                                                         sizeof(glm::vec3)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                                         sizeof(mesh_vertex_descriptor),
                                                                         (GLvoid *)offsetof(mesh_vertex_descriptor, vertex_color));

        draw_camera->camera_renderer.gl_functions->glVertexAttribPointer(draw_camera->camera_renderer.attributes_locations.normal_location,
                                                                         sizeof(glm::vec3)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                                         sizeof(mesh_vertex_descriptor),
                                                                         (GLvoid *)offsetof(mesh_vertex_descriptor, vertex_normal));

        /* Конфгурация аттрибутов текстурных координат вертексов*/
        draw_camera->camera_renderer.gl_functions->glVertexAttribPointer(draw_camera->camera_renderer.attributes_locations.Ambient_texture_coordinate_location,
                                                                         sizeof(glm::vec2)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                                         sizeof(mesh_vertex_descriptor),
                                                                         (GLvoid *)offsetof(mesh_vertex_descriptor, Ambient_texture_coordinates));

        draw_camera->camera_renderer.gl_functions->glVertexAttribPointer(draw_camera->camera_renderer.attributes_locations.Diffuse_texture_coordinate_location,
                                                                         sizeof(glm::vec2)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                                         sizeof(mesh_vertex_descriptor),
                                                                         (GLvoid *)offsetof(mesh_vertex_descriptor, Diffuse_texture_coordinates));

        draw_camera->camera_renderer.gl_functions->glVertexAttribPointer(draw_camera->camera_renderer.attributes_locations.Specular_texture_coordinate_location,
                                                                         sizeof(glm::vec2)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                                         sizeof(mesh_vertex_descriptor),
                                                                         (GLvoid *)offsetof(mesh_vertex_descriptor, Specular_texture_coordinates));

        draw_camera->camera_renderer.gl_functions->glVertexAttribPointer(draw_camera->camera_renderer.attributes_locations.Normals_texture_coordinate_location,
                                                                         sizeof(glm::vec2)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                                         sizeof(mesh_vertex_descriptor),
                                                                         (GLvoid *)offsetof(mesh_vertex_descriptor, Normals_texture_coordinates));


        ElementBufferObject->bind();
        ElementBufferObject->allocate(mesh_data.indexes_array.data(), mesh_data.indexes_array.size() * sizeof(GLuint));
        draw_camera->camera_renderer.gl_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                                                mesh_data.indexes_array.size() * sizeof(GLuint),
                                                                mesh_data.indexes_array.data(),
                                                                GL_STREAM_DRAW);

        VertexArrayObject->release();
        VertexBufferObject->release();
        ElementBufferObject->release();

        status = mesh_buffers_ready;
    }

    void clear_buffers()
    {
        VertexArrayObject->destroy();
        VertexBufferObject->destroy();
        ElementBufferObject->destroy();
    }

    GLuint old_programm_id = 0;
    bool old_programm_id_initialized = false;

    mesh_status_type status = mesh_buffers_not_ready;
    mesh_data_descriptor mesh_data;

    QOpenGLVertexArrayObject *VertexArrayObject = nullptr;
    QOpenGLBuffer *VertexBufferObject = nullptr;
    QOpenGLBuffer *ElementBufferObject = nullptr;
};
