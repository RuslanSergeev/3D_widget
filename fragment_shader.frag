#version 330 core

in vec3 fragment_color;
in vec3 fragment_normal;
in vec2 fragment_texture_coordinates;


/*
 * Все семплеры фрагментного шейдера должны иметь специализированные имена,
 * чтобы класс шейдерной программы смог их акт
*/
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main()
{
    gl_FragColor = vec4(fragment_color, 1.0f);
    //gl_FragColor = texture(texture0, fragment_texture_coordinates) * vec4(fragment_color, 1.0f);
//    gl_FragColor = texture(texture0, fragment_texture_coordinates);
}
