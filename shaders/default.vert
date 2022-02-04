#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBiTangent;
layout (location = 10) in vec2 TexCoords0;
	
out VS_OUT{
    vec3 vs_position;//����ռ�����

    //TBN����
    vec3 vs_normal;
    vec3 vs_tangent;
    vec3 vs_bitangent;

    //uv
    vec2 uv;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    
    vs_out.vs_normal = aNormal;
    vs_out.vs_tangent = aTangent;
    vs_out.vs_bitangent = aBiTangent;
    vs_out.uv = TexCoords0;
    vec4 worldPos = model * vec4(aPos, 1.0);
    vs_out.vs_position =vec3(worldPos.xyz); //����ռ�����
    gl_Position = projection * view *model * vec4(aPos, 1.0);
}