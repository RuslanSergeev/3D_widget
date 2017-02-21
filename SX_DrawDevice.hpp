#pragma once

#include <QtOpenGL>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include <glm/glm.hpp>
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

class SX_DrawDevice
{
public:
    QOpenGLFunctions *gl_functions = nullptr;
    QOpenGLShaderProgram *program = nullptr;
    program_location_descriptor attributes_locations;
    GLuint primitive_type = GL_TRIANGLES;

    glm::mat4x4 view_projection_matrix = glm::mat4x4(1.0f);
};
