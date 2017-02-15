#pragma once

#include <QtOpenGL>

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
