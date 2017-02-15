#pragma once

#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <SX_Camera.hpp>
#include <SX_Mesh.hpp>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <list>
using std::list;

//#include <vector>
//#include <algorithm>
//using std::vector;

#include <cstring>
#include <cstddef>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::mat3x3;
using glm::mat4x4;


class SX_Model{
public:

//    typedef struct model_location_descriptor_{
//        GLint position_location;
//        GLint color_location;
//        GLint normal_location;
//        GLint model_matrix_location;
//        GLint texture_coordinate_location;
//    } model_location_descriptor;

//    typedef struct model_data_descriptor_{
//        vector<model_vertex_descriptor> points_array;
//        vector<GLuint> indexes_array;
//        vector<QOpenGLTexture *> textures_array;
//        vector<const char *> sampler_names;
//    } model_data_descriptor;

//    /*!
//     * @enum enum model_status_type
//     * @brief Перечисление состояний моделей.
//    */
//    enum model_status_type{
//        model_buffers_not_ready,
//        model_buffers_ready
//    };

    /*!
     * @brief  Пустой конструктор для контейнеров.
    */
    SX_Model():
        SX_Model(GL_TRIANGLES)
    {
    }

    /*
     *  TODO: примитив не должен храниться в модели - это скорее параметр камеры,
     *  чтобы была возможность отрисовки одной модели несколькими примитивами,
     *  которые хранятся в камерах!
     */
    SX_Model(const GLuint drawing_primitive_type):
        primitive_type(drawing_primitive_type)
    {
        set_location(vec3(0, 0, 0));
        set_orientation(vec3(0, 0, 0));
    }

    ~SX_Model()
    {
//        clear_buffers();
    }

    /*
     * TODO: должен принимать камеру, с помощью которой будет производиться отрисовка.
     * Камера должна хранить: своё положение, ориентацию, параметры перспективы, примитивы отрисовки
     * и прочие параметры!
    */
    void draw(/*TODO: const OpenGL_camera &camera*/)
    {
//        if(!is_ready())
//        {
//            configure_attributes_locations();
//            setup_buffers();
//        }
//        else
//        {
            upload_model_matrix();
            auto meshes_iterator = meshes.begin();
            while(meshes_iterator != meshes.end())
            {
                meshes_iterator->draw();
            }



//            activate_model_textures();
//            VertexArrayObject->bind();
//            gl_functions->glDrawElements(primitive_type, model_data.indexes_array.size(), GL_UNSIGNED_INT, 0);
//            VertexArrayObject->release();
//        }
    }

    void rotate_arround_world_point(const vec3 &rotation_origin, const vec3 &axis, const float angle_radians)
    {
        move_by_world_vector(-1.0f * rotation_origin);
        glm::rotate(model_matrix, angle_radians, axis);
        move_by_world_vector(rotation_origin);
    }

    void rotate_arround_self_point(const vec3 &rotation_origin, const vec3 &axis, const float angle_radians)
    {
        vec4 absolute_rotation_origin = vec4(rotation_origin, 1) + model_matrix[3];
        vec4 absolute_axis = model_matrix * vec4(axis, 1) - model_matrix[3];
        rotate_arround_world_point(absolute_rotation_origin, absolute_axis, angle_radians);
    }

    void move_by_world_vector(const vec3 &world_movement)
    {
        mat4x4 world_movement_matrix(1);
        glm::translate(world_movement_matrix, world_movement);
        model_matrix = world_movement_matrix * model_matrix;
    }

    void move_by_self_vector(const vec3 &self_vector)
    {
        glm::translate(model_matrix, self_vector);
    }


    void set_model_view_projection(const glm::mat4 &mvp)
    {
        model_matrix = mvp;
    }

    void set_location(const vec3 &model_location)
    {
        model_matrix[3] = vec4(model_location, 1);
    }

    void set_orientation(const quat &model_orientation)
    {
        mat4x4 total_matrix = glm::mat4_cast(model_orientation);
        total_matrix[3] = model_matrix[3];
        model_matrix = total_matrix;
    }

    void set_orientation(const vec3 &Euler_ZYX_angles)
    {
        mat4x4 total_matrix = glm::eulerAngleZY(Euler_ZYX_angles.z, Euler_ZYX_angles.y)*
                glm::eulerAngleX(Euler_ZYX_angles.x);
        total_matrix[3] = model_matrix[3];
        model_matrix = total_matrix;
    };

    const mat4x4 &get_model_matrix() const
    {
        return model_matrix;
    }

    void add_mesh(SX_Mesh &new_mesh)
    {
        new_mesh.set_primitive_type(primitive_type);
        meshes.push_back(new_mesh);
    }


//    bool add_texture(const char *texture_filename, const char *sampler_name)
//    {
//        QOpenGLTexture *new_texture = new QOpenGLTexture(QImage(texture_filename).mirrored());
//        new_texture->create();
//        new_texture->generateMipMaps();
//        new_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
//        new_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
//        new_texture->setWrapMode(QOpenGLTexture::Repeat);

//        return add_texture(new_texture, sampler_name);
//    }

//    bool add_texture(QOpenGLTexture *new_texture, const char *sampler_name)
//    {
//        if(!textures_full() && new_texture->isCreated())
//        {
//            model_data.textures_array.push_back(new_texture);
//            model_data.sampler_names.push_back(sampler_name);
//            return true;
//        }
//        return false;
//    }

//    void activate_model_textures(){
//        auto current_texture = model_data.textures_array.begin();
//        auto current_sampler = model_data.sampler_names.begin();
//        GLuint texture_number  = 0;

//        while(current_texture != model_data.textures_array.end())
//        {
//            gl_functions->glActiveTexture(GL_TEXTURE0 + texture_number);
//            program->setUniformValue(*current_sampler, texture_number);
//            (*current_texture)->bind();

//            ++current_texture;
//            ++current_sampler;
//            ++texture_number;
//        }
//    }

//    void add_point(const model_vertex_descriptor &new_point)
//    {
//        model_data.points_array.push_back(new_point);
//    }

//    void add_index(const GLuint new_index)
//    {
//        model_data.indexes_array.push_back(new_index);
//    }

//    bool is_ready() const{
//        return model_buffers_ready == status;
//    }

//    operator GLuint() const{
//        return VertexArrayObject->objectId();
//    }

    bool set_functions(QOpenGLFunctions *OpenGLContext)
    {
        if(nullptr!=OpenGLContext)
        {
            gl_functions = OpenGLContext;
            return true;
        }
        return false;
    }

    bool set_program(QOpenGLShaderProgram *OpenGLProgram)
    {
        if(nullptr != OpenGLProgram && OpenGLProgram->isLinked())
        {
            program = OpenGLProgram;
        }
        return false;
    }

    void set_attributes_locations(const program_location_descriptor &new_locations)
    {
        attributes_locations = new_locations;
    }

private:

//    bool textures_full() const
//    {
//        GLint max_textures;
//        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_textures);
//        return model_data.textures_array.size() == static_cast<size_t>(max_textures);
//    }


//    /*!
//     * @fun bool configure_attributes_locations()
//     * @brief Определяет расположение аттрибутов и униформов в памяти
//     * шейдерной программы. Инициализирует структуру с "адресами" для последующей
//     * передачи в меши модели.
//    */
//    bool configure_attributes_locations()
//    {
//        if(nullptr != program && program->isLinked())
//        {
//            attributes_locations.color_location = program->attributeLocation(color_attribute_name);
//            attributes_locations.model_matrix_location = program->uniformLocation(model_matrix_uniform_name);
//            attributes_locations.normal_location = program->attributeLocation(normal_attribute_name);
//            attributes_locations.position_location = program->attributeLocation(position_attribute_name);
//            attributes_locations.texture_coordinate_location = program->attributeLocation(texture_coordinate_attribute_name);
//            return true;
//        }
//        return false;
//    }

    /*!
     * @fun void upload_model_matrix()
     * @brief загружает матрицу ориентации модели в память шейдерной программы.
    */
    void upload_model_matrix()
    {
        gl_functions->glUniformMatrix4fv(attributes_locations.model_matrix_location, 1, GL_FALSE, glm::value_ptr(model_matrix));
    }

//    void setup_buffers()
//    {
//        if(!is_ready())
//        {
//            VertexBufferObject = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//            VertexBufferObject->create();
//            VertexBufferObject->setUsagePattern(QOpenGLBuffer::StaticDraw);

//            ElementBufferObject = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//            ElementBufferObject->create();
//            ElementBufferObject->setUsagePattern(QOpenGLBuffer::StaticDraw);

//            VertexArrayObject = new QOpenGLVertexArrayObject;
//            VertexArrayObject->create();
//            VertexArrayObject->bind();

//            VertexBufferObject->bind();
//            VertexBufferObject->allocate(model_data.points_array.data(), model_data.points_array.size() * sizeof(model_vertex_descriptor));

//            program->enableAttributeArray(attributes_locations.position_location);
//            program->enableAttributeArray(attributes_locations.color_location);
//            program->enableAttributeArray(attributes_locations.normal_location);
//            program->enableAttributeArray(attributes_locations.texture_coordinate_location);

//            gl_functions->glVertexAttribPointer(attributes_locations.position_location, sizeof(glm::vec3)/sizeof(float), GL_FLOAT, GL_FALSE,
//                                                    sizeof(model_vertex_descriptor), (GLvoid *)offsetof(model_vertex_descriptor, vertex_position));

//            gl_functions->glVertexAttribPointer(attributes_locations.color_location, sizeof(glm::vec3)/sizeof(float), GL_FLOAT, GL_FALSE,
//                                                    sizeof(model_vertex_descriptor), (GLvoid *)offsetof(model_vertex_descriptor, vertex_color));

//            gl_functions->glVertexAttribPointer(attributes_locations.normal_location, sizeof(glm::vec3)/sizeof(float), GL_FLOAT, GL_FALSE,
//                                                    sizeof(model_vertex_descriptor), (GLvoid *)offsetof(model_vertex_descriptor, vertex_normal));

//            gl_functions->glVertexAttribPointer(attributes_locations.texture_coordinate_location, sizeof(glm::vec2)/sizeof(float), GL_FLOAT, GL_FALSE,
//                                                    sizeof(model_vertex_descriptor), (GLvoid *)offsetof(model_vertex_descriptor, vertex_texture_coordinates));

//            ElementBufferObject->bind();
//            ElementBufferObject->allocate(model_data.indexes_array.data(), model_data.indexes_array.size() * sizeof(GLuint));

//            VertexArrayObject->release();
//            VertexBufferObject->release();
//            ElementBufferObject->release();

//            status = model_buffers_ready;
//        }
//    }

//    void clear_buffers()
//    {
//        VertexArrayObject->destroy();
//        VertexBufferObject->destroy();
//        ElementBufferObject->destroy();
//    }

    QOpenGLFunctions *gl_functions;
    QOpenGLShaderProgram *program;

    const GLuint primitive_type;
    mat4x4  model_matrix;

//    model_status_type status = model_buffers_not_ready;
//    model_data_descriptor model_data;
    program_location_descriptor attributes_locations;

    list<SX_Mesh> meshes;

//    QOpenGLVertexArrayObject *VertexArrayObject = nullptr;
//    QOpenGLBuffer *VertexBufferObject = nullptr;
//    QOpenGLBuffer *ElementBufferObject = nullptr;
};
