#version 330 core

in vec3 fragment_color;
in vec3 fragment_normal;
in vec2 fragment_texture_coordinates;


/*
 * Все семплеры фрагментного шейдера должны иметь специализированные имена,
 * чтобы класс шейдерной программы смог их акт
*/
uniform sampler2D Ambient_texture;
uniform sampler2D Diffuse_texture;
uniform sampler2D Specular_texture;
uniform sampler2D Normals_texture;

void main()
{
    gl_FragColor = vec4(fragment_color, 1.0f);
}
