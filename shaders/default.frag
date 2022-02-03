#version 330 core
in VS_OUT{
    vec3 vs_position;
    vec3 vs_normal;
    vec3 vs_tangent;
    vec3 vs_bitangent;
    vec2 uv;
}fs_in;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D ao_map;
uniform sampler2D roughness_map;

float PI = 3.1415926535;

//H:半角向量。a：粗糙度。N:顶点法线——NDF方程
float D_GGX_TR(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

//几何函数G：计算微表面模型自遮挡比率   k是粗糙度的重映射   N：法线   V:视线  L:光线
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

//菲涅尔项
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{    

    mat3 TBN = mat3(fs_in.vs_tangent, fs_in.vs_bitangent, fs_in.vs_normal);
    vec3 normal = texture(normal_map, fs_in.uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(TBN * normal);

    vec3 color = texture(diffuse_map, fs_in.uv).rgb;
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.vs_position);
    float bias = max(0.05 * (1.0 - dot(fs_in.vs_normal, lightDir)), 0.005);//阴影bias（暂时无用）

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
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/2.2));
}