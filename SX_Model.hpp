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

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::mat3x3;
using glm::mat4x4;


class SX_Model{
public:

    /*!
     * @enum enum model_status_type
     * @brief Перечисление состояний моделей.
    */
    enum model_status_type{
        model_not_ready,
        model_ready
    };

    /*
     *  TODO: примитив не должен храниться в модели - это скорее параметр камеры,
     *  чтобы была возможность отрисовки одной модели несколькими примитивами,
     *  которые хранятся в камерах!
     */
    SX_Model()
    {
        set_location(vec3(0, 0, 0));
        set_orientation(vec3(0, 0, 0));
        set_scaling(glm::vec3(1.0f));
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
        if(!is_ready())
        {
            update_meshes();
            status = model_ready;
        }

        update_model_view_projection_matrix();
        //            upload_model_view_projection_matrix();
        auto current_mesh = meshes.begin();
        while(current_mesh != meshes.end())
        {
            current_mesh->draw();
            ++current_mesh;
        }
    }

    void rotate_arround_world_point(const vec3 &rotation_origin, const vec3 &axis, const float angle_radians)
    {
        move_by_world_vector(-1.0f * rotation_origin);
        model_matrix = glm::rotate(glm::mat4x4(), angle_radians, axis) * model_matrix;
        move_by_world_vector(rotation_origin);
    }//OK

    void rotate_arround_self_point(const vec3 &rotation_origin, const vec3 &axis, const float angle_radians)
    {
        vec4 absolute_rotation_origin = model_matrix*vec4(rotation_origin, 1);
        vec4 absolute_axis = model_matrix * vec4(axis, 1) - model_matrix[3];
        rotate_arround_world_point(absolute_rotation_origin, absolute_axis, angle_radians);
    }//OK

    void move_by_world_vector(const vec3 &world_movement)
    {
        model_matrix = glm::translate(glm::mat4x4(1.0f), world_movement) * model_matrix;
    }

    void move_by_self_vector(const vec3 &self_vector)
    {
        model_matrix = glm::translate(model_matrix, self_vector);
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

    void set_orientation(const vec3 &Euler_ZYX_radians)
    {
        mat4x4 total_matrix = glm::eulerAngleZY(Euler_ZYX_radians.z, Euler_ZYX_radians.y)*
                glm::eulerAngleX(Euler_ZYX_radians.x);
        total_matrix[3] = model_matrix[3];
        model_matrix = total_matrix;
    };

    void set_scaling(const vec3 &new_scaling)
    {
        scaling = new_scaling;
    }

    const mat4x4 &get_model_matrix() const
    {
        return model_matrix;
    }

    void add_mesh(SX_Mesh &new_mesh)
    {
        meshes.push_back(new_mesh);
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

    void update_meshes()
    {
        auto current_mesh = meshes.begin();
        while(current_mesh != meshes.end())
        {
            current_mesh->set_responsible_camera(responsible_camera);
            ++current_mesh;
        }
    }

    void update_model_view_projection_matrix()
    {
        model_view_projection_matrix =
                responsible_camera->view_projection_matrix *
                model_matrix           *
                glm::scale(glm::mat4x4(1.0f), scaling);
    }

    bool is_ready() const{
        return (model_ready == status) && responsible_camera;
    }

private:

    /*!
     * @fun void upload_model_matrix()
     * @brief загружает матрицу ориентации модели в память шейдерной программы.
    */
    void upload_model_view_projection_matrix()
    {
        responsible_camera->gl_functions->glUniformMatrix4fv(responsible_camera->attributes_locations.model_matrix_location, 1, GL_FALSE,
                                                             glm::value_ptr(model_view_projection_matrix));
    }

    mat4x4 model_view_projection_matrix = glm::mat4x4(1.0f);
    mat4x4 model_matrix = glm::mat4x4(1.0f);
    vec3 scaling = glm::vec3(1.0f, 1.0f, 1.0f);

    model_status_type status = model_not_ready;


    SX_Drawable *responsible_camera = nullptr;

    list<SX_Mesh> meshes;
};
