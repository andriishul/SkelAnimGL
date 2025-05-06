#version 450 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec3 a_Normal;     
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;
layout(location = 5) in vec4 a_Weights;    
layout(location = 6) in ivec4 a_BoneIndices; 

out FSInfo
{
    vec3 FragPos, ViewPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_out; 

layout(std140, binding = 0) buffer FinalMatricesBuffer {
    mat4 boneTransforms[];
};

layout(std140, binding = 1) uniform CameraBuffer {
    mat4 Projection;
    mat4 View;
};

layout(std140, binding = 2) uniform TransformDataBuffer {
    mat4 Model;
};

void main() 
{
    vec4 totalPosition = vec4(0.0);
    vec3 totalNormal = vec3(0.0);
    vec3 totalTangent = vec3(0.0);
    vec3 totalBitangent = vec3(0.0);

    for(int i = 0; i < 4; i++)
    {
       if (a_BoneIndices[i] == -1) 
           continue;

        if(a_BoneIndices[i] >= boneTransforms.length()) 
        {
            totalPosition = vec4(a_Pos, 1.0f);
            break;
        }

        mat4 boneMatrix = boneTransforms[a_BoneIndices[i]];
        
        vec4 localPosition = boneMatrix * vec4(a_Pos, 1.0f);
        totalPosition += localPosition * a_Weights[i];

        mat3 boneRotation = mat3(boneMatrix);

        totalNormal += boneRotation * a_Normal * a_Weights[i];
        totalTangent += boneRotation * a_Tangent * a_Weights[i];
        totalBitangent += boneRotation * a_Bitangent * a_Weights[i];
    }

    vec4 worldPosition = Model * totalPosition;

    gl_Position = Projection * View * worldPosition;

    mat3 normalMatrix = transpose(inverse(mat3(Model)));  
    vec3 N = normalize(normalMatrix * totalNormal);
    vec3 T = normalize(mat3(Model) * totalTangent);
    vec3 B = normalize(mat3(Model) * totalBitangent);

    T = normalize(T - dot(T, N) * N);
    B = cross(N, T);

    fs_out.ViewPos = (View * worldPosition).xyz;
    fs_out.FragPos = worldPosition.xyz;
    fs_out.TBN = mat3(T, B, N); 
    fs_out.TexCoords = a_TexCoords;
}
