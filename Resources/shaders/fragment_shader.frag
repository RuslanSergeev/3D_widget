#version 330 core

in vec3 fragment_color;
in vec3 fragment_normal;

//Fragment texture attributes
in vec2 fragment_Ambient_texture_coordoutates;
in vec2 fragment_Diffuse_texture_coordoutates;
in vec2 fragment_Specular_texture_coordoutates;
in vec2 fragment_Normals_texture_coordoutates;

/*
 * Все семплеры фрагментного шейдера должны иметь специализированные имена,
 * чтобы класс шейдерной программы смог их активировать.
*/
uniform sampler2D Ambient_texture;
uniform sampler2D Diffuse_texture;
uniform sampler2D Specular_texture;
uniform sampler2D Normals_texture;

void main()
{
//    gl_FragColor = vec4(fragment_color, 1.0f);
//    gl_FragColor = texture(texture0, fragment_texture_coordinates) * vec4(fragment_color, 1.0f);
    gl_FragColor = mix(texture(Diffuse_texture, fragment_Diffuse_texture_coordoutates), vec4(fragment_color, 1.0f), 0.1f);
//    gl_FragColor = texture(Difuse_texture, fragment_texture_coordinates);
}
