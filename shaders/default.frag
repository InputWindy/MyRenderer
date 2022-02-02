#version 330 core
in VS_OUT{
    vec3 vs_position;
    vec3 vs_normal;
    vec2 uv_diffuse;
}fs_in;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D diffuse_map;

void main()
{    
    vec3 color = texture(diffuse_map, fs_in.uv_diffuse).rgb;
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.vs_position);
    vec3 normal = normalize(fs_in.vs_normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.vs_position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.3) * spec; 
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}