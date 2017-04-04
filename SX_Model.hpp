#pragma once

#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <SX_Camera.hpp>
#include <SX_Mesh.hpp>



#include <list>
using std::list;



#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/color4.h>


class SX_Model
{
public:

    /*!
     * @enum enum model_status_type
     * @brief Перечисление состояний моделей.
    */
    enum model_status_type{
        model_not_ready,
        model_ready
    };


    SX_Model()
    {
        set_location(vec3(0, 0, 0));
        set_orientation(vec3(0, 0, 0));
        set_scaling(glm::vec3(1.0f));
    }

    ~SX_Model()
    {
    }

    void load_vertices(const aiScene *scene, aiMesh *cur_mesh, SX_Mesh *new_mesh)
    {
        for(size_t cur_vertex = 0; cur_vertex < cur_mesh->mNumVertices; ++cur_vertex)
        {

            //инициализация вектора положения.
            aiVector3t<float> position = cur_mesh->mVertices[cur_vertex];
            vec3 new_position(position.x, position.y, position.z);

            //Если имеются нормали, инициализируем их фактическим значением, иначе нулевым.
            vec3 new_normal = glm::sphericalRand(1.0f);
            if(cur_mesh->HasNormals())
            {
                aiVector3t<float> normal = cur_mesh->mNormals[cur_vertex];
                new_normal = vec3(normal.x, normal.y, normal.z);
            }

            //Запрашиваем цвет вершины из нулевого цветового набора.
            vec4 new_color = glm::vec4(glm::vec3(0.0f, 0.5f, 0.5f) + glm::sphericalRand(0.2f), 1.0f);
            if(cur_mesh->HasVertexColors(0))
            {
                aiColor4D color = cur_mesh->mColors[cur_vertex][0];
                new_color = vec4(color.r, color.g, color.b, color.a);
            }

            //Запрашиваем текстурные координаты для нулевого набора текстур.
            vec2 new_ambient_coords = glm::circularRand(1.0f);
            int ambient_coords_index = get_uv_coords_index(scene, cur_mesh, aiTextureType_AMBIENT);
            if(cur_mesh->HasTextureCoords(ambient_coords_index))
            {
                aiVector3D texture_coords = cur_mesh->mTextureCoords[ambient_coords_index][cur_vertex];
                new_ambient_coords = vec2(texture_coords.x, texture_coords.y);
            }

            //определяем диффузные координаты вертекса, если таковые имеются
            vec2 new_diffuse_coords = glm::circularRand(1.0f);
            int diffuse_coords_index = get_uv_coords_index(scene, cur_mesh, aiTextureType_DIFFUSE);
            if(cur_mesh->HasTextureCoords(diffuse_coords_index))
            {
                aiVector3D texture_coords = cur_mesh->mTextureCoords[diffuse_coords_index][cur_vertex];
                new_diffuse_coords = vec2(texture_coords.x, texture_coords.y);
            }

            //определяем "глянцевые" координаты вертекса, если таковые имеются
            vec2 new_specular_coords = glm::circularRand(1.0f);
            int specular_coords_index = get_uv_coords_index(scene, cur_mesh, aiTextureType_SPECULAR);
            if(cur_mesh->HasTextureCoords(specular_coords_index))
            {
                aiVector3D texture_coords = cur_mesh->mTextureCoords[specular_coords_index][cur_vertex];
                new_specular_coords = vec2(texture_coords.x, texture_coords.y);
            }

            //определяем координаты вертекса на карте нормалей, если таковые имеются
            vec2 new_normals_coords = glm::circularRand(1.0f);
            int normals_coords_index = get_uv_coords_index(scene, cur_mesh, aiTextureType_NORMALS);
            if(cur_mesh->HasTextureCoords(normals_coords_index))
            {
                aiVector3D texture_coords = cur_mesh->mTextureCoords[normals_coords_index][cur_vertex];
                new_normals_coords = vec2(texture_coords.x, texture_coords.y);
            }

            new_mesh->add_point(new_position,
                                new_color,
                                new_normal,
                                new_ambient_coords,
                                new_diffuse_coords,
                                new_specular_coords,
                                new_normals_coords);
        }
    }

    void load_indices(aiMesh *cur_mesh, SX_Mesh *new_mesh)
    {
        if(cur_mesh->HasFaces())
        {
            for(size_t cur_face = 0; cur_face < cur_mesh->mNumFaces; ++cur_face)
            {
                new_mesh->add_index(cur_mesh->mFaces[cur_face].mIndices[0]);
                new_mesh->add_index(cur_mesh->mFaces[cur_face].mIndices[1]);
                new_mesh->add_index(cur_mesh->mFaces[cur_face].mIndices[2]);
            }
        }
        else
        {
            for(size_t cur_index = 0; cur_index < cur_mesh->mNumVertices; ++cur_index)
            {
                new_mesh->add_index(cur_index);
            }
        }
    }


    /*!
     * @brief получить индекс текстурных координат для данного типа текстур в данном материале.
     * @param scene - сцена, которой пренадлежит меш.
     * @param mesh - меш, для которого запрашивается индекс текстурных координат.
     * @param type тип материала (aiTextureType_AMBIENT, aiTextureType_DIFFUSE ...)
    */
    unsigned int get_uv_coords_index(const aiScene *scene, aiMesh *mesh, aiTextureType type)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        unsigned int uv_index = 0;
        aiString unused_path;
        material->GetTexture(type, 0, &unused_path, NULL, &uv_index);
        return uv_index;
    }


    void load_textures(const aiScene *scene, aiMesh *cur_mesh, SX_Mesh *new_mesh)
    {
        const size_t material_index = cur_mesh->mMaterialIndex;
        const aiMaterial *material = scene->mMaterials[material_index];
        aiString texture_path;

        if(material->GetTextureCount(aiTextureType_AMBIENT) > 0)
        {
            material->GetTexture(aiTextureType_AMBIENT, 0, &texture_path);
            new_mesh->add_texture(texture_path.C_Str(), ambient_sampler_name);
        }
        if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
            new_mesh->add_texture(texture_path.C_Str(), diffuse_sampler_name);
        }
        else
        {
            new_mesh->add_texture(default_diffuse_texture_path,diffuse_sampler_name);
        }
        if(material->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            material->GetTexture(aiTextureType_SPECULAR, 0, &texture_path);
            new_mesh->add_texture(texture_path.C_Str(), specular_sampler_name);
        }
        if(material->GetTextureCount(aiTextureType_NORMALS) > 0)
        {
            material->GetTexture(aiTextureType_NORMALS, 0, &texture_path);
            new_mesh->add_texture(texture_path.C_Str(), normals_sampler_name);
        }
    }

    void load_mesh(const aiScene *scene, aiMesh *cur_mesh, SX_Mesh *new_mesh)
    {
        //Загружаем вертексный буфер
        load_vertices(scene, cur_mesh, new_mesh);

        //Загружаем индексный буфер
        load_indices(cur_mesh, new_mesh);

        //Загружаем текстуры модели
        load_textures(scene, cur_mesh, new_mesh);
    }

    void load_models(const aiScene *scene, aiNode *node, SX_Model *node_model)
    {
        for(size_t mesh_index = 0; mesh_index < node->mNumMeshes; ++mesh_index)
        {
            SX_Mesh current_mesh;
            load_mesh(scene, scene->mMeshes[node->mMeshes[mesh_index]], &current_mesh);
            node_model->add_mesh(current_mesh);
        }

        for(size_t child_index = 0; child_index < node->mNumChildren; ++child_index)
        {
            SX_Model current_child_model;
            load_models(scene, node->mChildren[child_index], &current_child_model);
            node_model->add_model(current_child_model);
        }
    }

    bool load_from_file(const char *filename)
    {

        /*  Открытие файла модели при помощи QFile
         *  для поддержки файлов ресурсов Qt реализовать
         *  пока не удалось ввиду странностей в поведении ReadFileFromMemory
         *  в зависимости от типа модели, она требует разные размеры буфера
         *  size, либо size-1
         */

        /*QFile model_file(filename);
        if(!model_file.open(QIODevice::ReadOnly))
        {
            return false;
        }
        QByteArray model_buffer = model_file.readAll();
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFileFromMemory(model_buffer.data(), model_buffer.size()-1,
                                                           aiProcess_Triangulate|aiProcess_GenNormals);*/

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(filename,aiProcess_Triangulate|aiProcess_GenNormals);
        if(scene)
        {
            load_models(scene, scene->mRootNode, this);
            return true;
        }
        else
        {
            return false;
        }
    }

    void draw(SX_Camera *draw_camera)
    {
        draw_camera->use();
        draw_camera->update_view_projection_matrix();

        update_model_view_projection_matrix(draw_camera);
        upload_model_view_projection_matrix(draw_camera);
        auto current_mesh = meshes.begin();
        while(current_mesh != meshes.end())
        {
            current_mesh->draw(draw_camera);
            ++current_mesh;
        }

        if(has_child_models())
        {
            auto current_child_model = child_models.begin();
            while(current_child_model != child_models.end())
            {
                current_child_model->draw(draw_camera);
                ++current_child_model;
            }
        }

        draw_camera->release();
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

    const vec3 &get_scaling_vector() const
    {
        return scaling;
    }

    bool add_model(SX_Model &new_model)
    {
        new_model.set_parrent_model(this);

        const size_t size = child_models.size();
        if(size == child_models.capacity()){
            child_models.resize(2*size);
        }

        child_models.push_back(new_model);

        return true;
    }

    SX_Model &get_model(const int model_index){
        return child_models[model_index];
    }

    void add_mesh(SX_Mesh &new_mesh)
    {
        meshes.push_back(new_mesh);
    }

    void update_model_view_projection_matrix(SX_Camera *draw_camera)
    {
        if(has_parrent_models())
        {
            model_view_projection_matrix =
                    draw_camera->view_projection_matrix             *
                    parrent_model->get_model_matrix()               *
                    glm::scale(glm::mat4x4(1.0f), parrent_model->get_scaling_vector()) *
                    model_matrix                                    *
                    glm::scale(glm::mat4x4(1.0f), scaling);
        }
        else
        {
            model_view_projection_matrix =
                    draw_camera->view_projection_matrix             *
                    model_matrix                                    *
                    glm::scale(glm::mat4x4(1.0f), scaling);
        }
    }

    bool has_parrent_models()
    {
        return parrent_model != nullptr;
    }

    bool has_child_models()
    {
        return child_models.size() != 0;
    }

    bool is_ready() const{
        return model_ready == status;
    }

    bool set_parrent_model(SX_Model *new_model)
    {
        if(new_model)
        {
            parrent_model = new_model;
            return true;
        }
        else
        {
            return false;
        }
    }

    SX_Model *get_parrent_model()
    {
        return parrent_model;
    }


private:

    /*!
     * @fun void upload_model_matrix()
     * @brief загружает матрицу ориентации модели в память шейдерной программы.
    */
    void upload_model_view_projection_matrix(SX_Camera *draw_camera)
    {
        draw_camera->camera_renderer.gl_functions->glUniformMatrix4fv(draw_camera->camera_renderer.attributes_locations.model_matrix_location,
                                                                      1, GL_FALSE,
                                                                      glm::value_ptr(model_view_projection_matrix));
    }

    SX_Model *parrent_model = nullptr;

    mat4x4 model_view_projection_matrix = glm::mat4x4(1.0f);
    mat4x4 model_matrix = glm::mat4x4(1.0f);
    vec3 scaling = glm::vec3(1.0f, 1.0f, 1.0f);

    model_status_type status = model_not_ready;

    list<SX_Mesh> meshes;
    vector<SX_Model> child_models;
};
