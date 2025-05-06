#pragma once
#include <glm/glm.hpp>
#include <vector>

// Holds data related to a bone used for animation
struct BoneInfo 
{
    uint32_t hash_name; // Unique identifier for the bone, typically a hashed bone name string

    alignas(16) glm::mat4 offset; // Offset matrix (inverse of the bind pose), used in skinning
};

// Represents a bone in the skeleton hierarchy
struct BoneNode 
{
    uint32_t hash_name; // Unique identifier for the bone node, typically a hashed node name string

    int32_t parentIndex; // Index of the parent node in the hierarchy (-1 if it is the root)
    
    alignas(16) glm::mat4 transformation; // Local transformation relative to the parent bone
};

class Skeleton 
{
public:    
    void AddBoneNode(const BoneNode& boneNode) { m_BoneNodes.push_back(boneNode); }
    void AddBoneInfo(const BoneInfo& boneInfo) { m_BoneInfos.push_back(boneInfo); }

    const std::vector<BoneInfo>& GetBoneInfos() const { return m_BoneInfos; }
    const std::vector<BoneNode>& GetBoneNodes() const { return m_BoneNodes; }
private:
    std::vector<BoneNode> m_BoneNodes;
    std::vector<BoneInfo> m_BoneInfos;
};