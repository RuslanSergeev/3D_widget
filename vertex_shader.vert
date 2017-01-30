#version 330 core

in vec3 vertex_position;
in vec3 vertex_color;
in vec3 vertex_normal;
in vec2 vertex_texture_coordinates;

//the MVP matrix for the model,
//with initial identity default value
uniform mat4 model_view_projection_matrix = mat4(1.0f);

out vec3 fragment_color;
out vec3 fragment_normal;
out vec2 fragment_texture_coordinates;

void main()
{
    gl_Position = model_view_projection_matrix * vec4(vertex_position, 1.0);
    fragment_color = vertex_color;
    fragment_normal = vertex_normal;
    fragment_texture_coordinates = vertex_texture_coordinates;
}
