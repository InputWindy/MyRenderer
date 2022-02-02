#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 10) in vec2 TexCoords0;
layout (location = 11) in vec2 TexCoords1;
layout (location = 12) in vec2 TexCoords2;
layout (location = 13) in vec2 TexCoords3;
layout (location = 14) in vec2 TexCoords4;
layout (location = 15) in vec2 TexCoords5;
	
out VS_OUT{
    vec3 vs_position;
    vec3 vs_normal;
    vec2 uv_diffuse;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.vs_position = aPos;
    vs_out.vs_normal = aNormal;
    vs_out.uv_diffuse = TexCoords0;
    gl_Position = projection * view *model * vec4(aPos, 1.0);
}