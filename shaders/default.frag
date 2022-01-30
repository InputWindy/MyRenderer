#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D specular_map;
uniform sampler2D metallic_map;
uniform sampler2D ao_map;

void main()
{    
    FragColor = texture(diffuse_map, TexCoords);
}