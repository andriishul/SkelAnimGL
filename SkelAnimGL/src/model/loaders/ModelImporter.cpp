#include "ModelImporter.hpp"
#include "utils/StringHash.hpp"
#include "utils/Logger.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static std::unordered_map<std::string, std::shared_ptr<Texture2D>> s_CachedTextures;

static std::string s_Directory;

static glm::vec4 ConvertToGLM(const aiQuaternion& quat) 
{
    return glm::vec4(-quat.x, -quat.y, -quat.z, quat.w);
}
static glm::vec4 ConvertToGLM(const aiVector3D& vec)
{
    return glm::vec4(vec.x, vec.y, vec.z, 1.0f);
}
static glm::mat4 ConvertToGLM(const aiMatrix4x4& mat)
{
    glm::mat4 transform = glm::mat4(1.0f);

    transform[0][0] = mat.a1; transform[0][1] = mat.b1; transform[0][2] = mat.c1; transform[0][3] = mat.d1;
    transform[1][0] = mat.a2; transform[1][1] = mat.b2; transform[1][2] = mat.c2; transform[1][3] = mat.d2;
    transform[2][0] = mat.a3; transform[2][1] = mat.b3; transform[2][2] = mat.c3; transform[2][3] = mat.d3;
    transform[3][0] = mat.a4; transform[3][1] = mat.b4; transform[3][2] = mat.c4; transform[3][3] = mat.d4;

    return transform;
}

static void AssignBoneToVertex(MeshVertex& vertex, int32_t bone_id, float_t weight)
{
    for (int32_t i = 0; i < 4; i++)
    {
        if (vertex.a_BoneIndices[i] < 0)
        {
            vertex.a_BoneIndices[i] = bone_id;
            vertex.a_Weights[i] = weight;
            break;
        }
    }
}

static Animation LoadAnimation(const aiAnimation* ai_animation)
{
    AnimationTrack animTrack;

    for (uint32_t i = 0; i < ai_animation->mNumChannels; i++)
    {
        aiNodeAnim* aiNodeAnim = ai_animation->mChannels[i];
  
        NodeKeyframes keyframes;

        for (uint32_t k = 0; k < aiNodeAnim->mNumPositionKeys; k++)
        {
            auto positionKey = aiNodeAnim->mPositionKeys[k];
            Keyframe keyFrame;
            keyFrame.time = positionKey.mTime;
            keyFrame.value = ConvertToGLM(positionKey.mValue);
            keyframes.positionKeys.push_back(keyFrame);
        }

        for (uint32_t k = 0; k < aiNodeAnim->mNumScalingKeys; k++)
        {
            auto scalingKey = aiNodeAnim->mScalingKeys[k];
            Keyframe keyFrame;
            keyFrame.time = scalingKey.mTime;
            keyFrame.value = ConvertToGLM(scalingKey.mValue);
            keyframes.scalingKeys.push_back(keyFrame);
        }

        for (uint32_t k = 0; k < aiNodeAnim->mNumRotationKeys; k++)
        {
            auto rotationKey = aiNodeAnim->mRotationKeys[k];
            Keyframe keyFrame;
            keyFrame.time = rotationKey.mTime;
            keyFrame.value = ConvertToGLM(rotationKey.mValue);
            keyframes.rotationKeys.push_back(keyFrame);
        }

        animTrack.AddAnimNode(StringToHash(aiNodeAnim->mNodeName.C_Str()), keyframes);
    }

    return Animation(ai_animation->mName.C_Str(), animTrack, 
        static_cast<float_t>(ai_animation->mDuration), static_cast<float_t>(ai_animation->mTicksPerSecond));
}

static void LoadMaterial(const aiMesh* mesh, const aiScene* scene, Material& material)
{
    if (mesh->mMaterialIndex < 0 || mesh->mMaterialIndex >= scene->mNumMaterials) {
        LOG_ERROR("ASSIMP::ERROR Materials not found.");
    }

    aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];

    aiString pathDiff, pathSpec, pathNorm;
    
    if (ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &pathDiff) == AI_SUCCESS)
    {
        if (s_CachedTextures.find(pathDiff.C_Str()) == s_CachedTextures.end())
            s_CachedTextures[pathDiff.C_Str()] = std::shared_ptr<Texture2D>(Texture2D::Create(s_Directory + pathDiff.C_Str()));
        material.diffuse = s_CachedTextures.at(pathDiff.C_Str());
    }
    else LOG_ERROR("ASSIMP::ERROR Failed to load diffuse texture.");

    if (ai_material->GetTexture(aiTextureType_SPECULAR, 0, &pathSpec) == AI_SUCCESS)
    {
        if (s_CachedTextures.find(pathSpec.C_Str()) == s_CachedTextures.end())
            s_CachedTextures[pathSpec.C_Str()] = std::shared_ptr<Texture2D>(Texture2D::Create(s_Directory + pathSpec.C_Str()));
        material.specular = s_CachedTextures.at(pathSpec.C_Str());
    }

    if (ai_material->GetTexture(aiTextureType_NORMALS, 0, &pathNorm) == AI_SUCCESS)
    {
        if (s_CachedTextures.find(pathNorm.C_Str()) == s_CachedTextures.end())
            s_CachedTextures[pathNorm.C_Str()] = std::shared_ptr<Texture2D>(Texture2D::Create(s_Directory + pathNorm.C_Str()));
        material.normal = s_CachedTextures.at(pathNorm.C_Str());
    }
}

static void ExtractBoneInfo(const aiMesh* mesh, const Skeleton& skeleton, std::vector<MeshVertex>& vertices)
{
    const auto &boneInfos = skeleton.GetBoneInfos();
    int32_t boneID = -1;
    for (uint32_t i = 0; i < mesh->mNumBones; i++)
    {
        aiBone* bone = *(mesh->mBones + i);        
        uint32_t hash_name = StringToHash(bone->mName.C_Str());

        auto it = std::find_if(boneInfos.begin(), boneInfos.end(), 
            [&](const auto& boneInfo) { return boneInfo.hash_name == hash_name; });

        if (it != boneInfos.end()) 
            boneID = std::distance(boneInfos.begin(), it);
        else boneID = -1;
        

        for (unsigned j = 0; j < bone->mNumWeights; j++)
        {
            aiVertexWeight weight = bone->mWeights[j];
            AssignBoneToVertex(vertices[weight.mVertexId], boneID, weight.mWeight);
        }
    }
}

static void LoadMesh(const Skeleton& skeleton, const aiMesh* ai_mesh, std::vector<MeshVertex>& vertices, std::vector<unsigned>& indices)
{
	for (unsigned i = 0; i < ai_mesh->mNumVertices; i++)
    {
        MeshVertex vertex;
        vertex.a_Pos = glm::vec3(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);

        if (ai_mesh->mTextureCoords[0])
            vertex.a_TexCoords = glm::vec2(ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y);

        if (ai_mesh->HasTangentsAndBitangents())
        {
            vertex.a_Tangent = glm::vec3(ai_mesh->mTangents[i].x, ai_mesh->mTangents[i].y, ai_mesh->mTangents[i].z);
            vertex.a_Bitangent = glm::vec3(ai_mesh->mBitangents[i].x, ai_mesh->mBitangents[i].y, ai_mesh->mBitangents[i].z);
        }

        if (ai_mesh->HasNormals())
            vertex.a_Normal = glm::vec3(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);

        vertices.push_back(vertex);
    }

    for (unsigned i = 0; i < ai_mesh->mNumFaces; i++)
    {
        auto face = ai_mesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    ExtractBoneInfo(ai_mesh, skeleton, vertices);
}

static void ReadBoneNode(Skeleton& skeleton, const aiNode* ainode, uint32_t parentID)
{
    BoneNode node;
    node.hash_name = StringToHash(ainode->mName.C_Str());
    node.transformation = ConvertToGLM(ainode->mTransformation);
    node.parentIndex = parentID;

    uint32_t nodeIndex = skeleton
        .GetBoneNodes()
        .size();
    skeleton.AddBoneNode(node);

    for (uint32_t i = 0; i < ainode->mNumChildren; i++)
    {
        ReadBoneNode(skeleton, ainode->mChildren[i], nodeIndex);
    }
}

static void ReadBoneInfo(Skeleton& skeleton, const aiMesh* ai_mesh)
{
	for (uint32_t i = 0; i < ai_mesh->mNumBones; i++)
	{
		BoneInfo boneInfo;
		aiBone* ai_bone = ai_mesh->mBones[i];
        boneInfo.hash_name = StringToHash(ai_bone->mName.C_Str());
        boneInfo.offset = ConvertToGLM(ai_bone->mOffsetMatrix);
		skeleton.AddBoneInfo(boneInfo);
	}
}

const std::shared_ptr<Model> ModelImporter::Load(const std::string& filename)
{
    std::vector<Animation> animations;
    std::vector<std::shared_ptr<Mesh>> meshes;
    Skeleton skeleton;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_CalcTangentSpace
		| aiProcess_FlipUVs | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph
		| aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights);
    
    if (!scene)
        LOG_ERROR("ASSIMP::ERROR Failed to load {}", filename);

    s_CachedTextures.clear();
    s_Directory = filename.substr(0, filename.find_last_of("/\\"));
    std::replace(s_Directory.begin(), s_Directory.end(), '\\', '/');
    s_Directory += '/';

	for (uint32_t i = 0; i < scene->mNumMeshes; i++)
	{
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
        Material material;

        aiMesh* ai_mesh = scene->mMeshes[i];
        
		ReadBoneInfo(skeleton, ai_mesh);
        LoadMaterial(ai_mesh, scene, material);
        LoadMesh(skeleton, ai_mesh, vertices, indices);
        meshes.push_back(std::make_shared<Mesh>(vertices, indices, material));
	}

    ReadBoneNode(skeleton, scene->mRootNode, -1);


    for (uint32_t i = 0; i < scene->mNumAnimations; i++)
    {
        animations.push_back(LoadAnimation(scene->mAnimations[i]));
    }
    s_CachedTextures.clear();

    return std::make_shared<Model>(skeleton, meshes, animations);
}
