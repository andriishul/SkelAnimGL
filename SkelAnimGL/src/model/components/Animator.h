#pragma once
#include "Animation.hpp"
#include "Skeleton.h"

#include <graphics/Buffer.hpp>
#include <graphics/Shader.hpp>

#include <memory>

struct AnimationNode 
{
	uint32_t hash_name;
	Keyframe position_k1, position_k2;
	Keyframe scaling_k1, scaling_k2;
	Keyframe rotation_k1, rotation_k2;
};

class Animator 
{
private:
	void UploadSkeleton(const Skeleton& skeleton);
	void ApplyAnimation();
public:
	Animator(const Skeleton& skeleton);
	void Play(const Animation&);
	void Stop();
	void Update(float_t deltaTime);
private:
	const Animation* m_CurrentAnimation;
	std::vector<AnimationNode> m_AnimNodes;
	float_t m_CurrentTime;

	int32_t m_DispatchSize;
	std::shared_ptr<ComputeShader> m_CS;
	std::unique_ptr<ShaderStorageBuffer> m_BoneInfoBuffer, m_NodeBuffer, m_AnimNodeBuffer, m_FinalMatrices;
};