#pragma once

#include <SX_Drawable.hpp>

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


typedef struct mesh_vertex_descriptor_{
    glm::vec3 vertex_position;                  /*! Координаты точки в пространстве модели. */
    glm::vec3 vertex_color;                        /*! Цвет вершины. */
    glm::vec3 vertex_normal;                         /*! Нормаль в вершине для алгоритмов шейдинга. */
    glm::vec2 vertex_texture_coordinates;          /*! Координаты вершины в пространстве координат текстуры. */
} mesh_vertex_descriptor;

class SX_Mesh
{
public:

    typedef struct mesh_data_descriptor_{
        vector<mesh_vertex_descriptor> points_array;
        vector<GLuint> indexes_array;
        list<QOpenGLTexture *> textures_array;
        list<const char *> sampler_names;
    } mesh_data_descriptor;

    /*!
     * @enum enum mesh_status_type
     * @brief Перечисление состояний меша.
    */
    enum mesh_status_type{
        mesh_buffers_not_ready,
        mesh_buffers_ready
    };

    void draw(/*TODO: const OpenGL_camera &camera*/)
    {
        if(!is_ready())
        {
            setup_buffers();
        }

            activate_mesh_textures();
            VertexArrayObject->bind();
            responsible_camera->gl_functions->glDrawElements(responsible_camera->primitive_type, mesh_data.indexes_array.size(), GL_UNSIGNED_INT, 0);
            VertexArrayObject->release();
    }

    bool add_texture(const char *texture_filename, const char *sampler_name)
    {
        QOpenGLTexture *new_texture = new QOpenGLTexture(QImage(texture_filename).mirrored());
        new_texture->create();
        new_texture->generateMipMaps();
        new_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        new_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
        new_texture->setWrapMode(QOpenGLTexture::Repeat);

        return add_texture(new_texture, sampler_name);
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

    void activate_mesh_textures(){
        auto current_texture = mesh_data.textures_array.begin();
        auto current_sampler = mesh_data.sampler_names.begin();
        GLuint texture_number  = 0;

        while(current_texture != mesh_data.textures_array.end())
        {
            responsible_camera->gl_functions->glActiveTexture(GL_TEXTURE0 + texture_number);
            responsible_camera->program->setUniformValue(*current_sampler, texture_number);
            (*current_texture)->bind();

            ++current_texture;
            ++current_sampler;
            ++texture_number;
        }
    }

    void add_point(const mesh_vertex_descriptor &new_point)
    {
        mesh_data.points_array.push_back(new_point);
    }

    void add_index(const GLuint new_index)
    {
        mesh_data.indexes_array.push_back(new_index);
    }

    bool is_ready() const{
        return mesh_buffers_ready == status;
    }

    operator GLuint() const{
        return VertexArrayObject->objectId();
    }

    bool set_responsible_camera(SX_Drawable *new_responsible_camera)
    {
        if(new_responsible_camera)
        {
            responsible_camera = new_responsible_camera;
            return true;
        }
        else
        {
            return false;
        }
    }

private:

    bool textures_full() const
    {
        GLint max_textures;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_textures);
        return mesh_data.textures_array.size() == static_cast<size_t>(max_textures);
    }

    void setup_buffers()
    {
        if(!is_ready())
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

            VertexBufferObject->bind();
            VertexBufferObject->allocate(mesh_data.points_array.data(), mesh_data.points_array.size() * sizeof(mesh_vertex_descriptor));

            responsible_camera->program->enableAttributeArray(attributes_locations.position_location);
            responsible_camera->program->enableAttributeArray(attributes_locations.color_location);
            responsible_camera->program->enableAttributeArray(attributes_locations.normal_location);
            responsible_camera->program->enableAttributeArray(attributes_locations.texture_coordinate_location);

            responsible_camera->gl_functions->glVertexAttribPointer(attributes_locations.position_location, sizeof(glm::vec3)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                    sizeof(mesh_vertex_descriptor), (GLvoid *)offsetof(mesh_vertex_descriptor, vertex_position));

            responsible_camera->gl_functions->glVertexAttribPointer(attributes_locations.color_location, sizeof(glm::vec3)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                    sizeof(mesh_vertex_descriptor), (GLvoid *)offsetof(mesh_vertex_descriptor, vertex_color));

            responsible_camera->gl_functions->glVertexAttribPointer(attributes_locations.normal_location, sizeof(glm::vec3)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                    sizeof(mesh_vertex_descriptor), (GLvoid *)offsetof(mesh_vertex_descriptor, vertex_normal));

            responsible_camera->gl_functions->glVertexAttribPointer(attributes_locations.texture_coordinate_location, sizeof(glm::vec2)/sizeof(float), GL_FLOAT, GL_FALSE,
                                                    sizeof(mesh_vertex_descriptor), (GLvoid *)offsetof(mesh_vertex_descriptor, vertex_texture_coordinates));

            ElementBufferObject->bind();
            ElementBufferObject->allocate(mesh_data.indexes_array.data(), mesh_data.indexes_array.size() * sizeof(GLuint));

            VertexArrayObject->release();
            VertexBufferObject->release();
            ElementBufferObject->release();

            status = mesh_buffers_ready;
        }
    }

    void clear_buffers()
    {
        VertexArrayObject->destroy();
        VertexBufferObject->destroy();
        ElementBufferObject->destroy();
    }

    mesh_status_type status = mesh_buffers_not_ready;
    mesh_data_descriptor mesh_data;
    program_location_descriptor attributes_locations;

    SX_Drawable *responsible_camera;

    QOpenGLVertexArrayObject *VertexArrayObject = nullptr;
    QOpenGLBuffer *VertexBufferObject = nullptr;
    QOpenGLBuffer *ElementBufferObject = nullptr;
};
