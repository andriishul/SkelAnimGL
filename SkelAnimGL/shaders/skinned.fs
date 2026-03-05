#version 450 core
in FSInfo
{
    vec3 FragPos, ViewPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

layout(binding = 0) uniform sampler2D diffuse_texture;
layout(binding = 1) uniform sampler2D specular_texture;
layout(binding = 2) uniform sampler2D normal_texture;

#define MAX_LIGHT_NUM 32

struct Light {
    vec4 Position;
    vec4 Color;
};

layout(std140, binding = 3) uniform LightBuffer
{
    Light lights[MAX_LIGHT_NUM];
};

out vec4 FragColor;

void main()
{
    vec3 normalMap = texture(normal_texture, fs_in.TexCoords).rgb;
    normalMap = normalize(normalMap * 2.0 - 1.0); 

    vec3 worldNormal = normalize(fs_in.TBN * normalMap);

    vec4 diffuseColor = texture(diffuse_texture, fs_in.TexCoords);
    vec4 specularColor = texture(specular_texture, fs_in.TexCoords);

    vec3 viewDir = normalize(fs_in.ViewPos - fs_in.FragPos);

    vec3 finalColor = vec3(0.0);

    for (uint i = 0; i < MAX_LIGHT_NUM; ++i)
    {
        vec3 lightPos = lights[i].Position.xyz;
        vec3 lightColor = lights[i].Color.rgb;

        vec3 lightDir = normalize(lightPos - fs_in.FragPos);
        float diff = max(dot(worldNormal, lightDir), 0.0);

        float spec = 0.0;
        if (diff > 0.0)
        {
            vec3 halfwayDir = normalize(lightDir + viewDir);
            spec = pow(max(dot(worldNormal, halfwayDir), 0.0), 32.0);
        }

        vec3 ambient = 0.1 * diffuseColor.rgb * lightColor;
        vec3 diffuse = diff * diffuseColor.rgb * lightColor;
        vec3 specular = spec * specularColor.rgb * lightColor;

        finalColor += ambient + diffuse + specular;
    }

    FragColor = vec4(finalColor, diffuseColor.a);
}
