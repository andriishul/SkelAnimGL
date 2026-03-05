#include "Animator.h"
#include <stdexcept>

constexpr int32_t WORKGROUP_SIZE = 32;

Animator::Animator(const Skeleton& skeleton)
	: m_CS(ComputeShader::Get("skeleton_anim")),
	m_CurrentTime(0.f), m_CurrentAnimation(0)
{
	UploadSkeleton(skeleton);
}

void Animator::Play(const Animation& animation)
{
	m_CurrentAnimation = &animation;
	m_AnimNodeBuffer.reset();
}

void Animator::Stop() 
{
	m_CurrentAnimation = 0;
}

void Animator::UploadSkeleton(const Skeleton& skeleton)
{
	const auto& boneInfos = skeleton.GetBoneInfos();
	int32_t boneNum = boneInfos.size();
	const auto& boneNodes = skeleton.GetBoneNodes();
	int32_t nodeNum = boneNodes.size();
	
	m_DispatchSize = (boneNum + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;
	
	m_BoneInfoBuffer = std::unique_ptr<ShaderStorageBuffer>(
		ShaderStorageBuffer::Create(boneInfos.data(), boneNum * sizeof(BoneInfo), 4));
	
	m_NodeBuffer = std::unique_ptr<ShaderStorageBuffer>(
		ShaderStorageBuffer::Create(boneNodes.data(), nodeNum * sizeof(BoneNode), 5));
		
	m_FinalMatrices = std::unique_ptr<ShaderStorageBuffer>(
		ShaderStorageBuffer::Create(sizeof(glm::mat4) * boneNum, 0));
}

void Animator::ApplyAnimation()
{
	const auto& animTrack = m_CurrentAnimation->GetTrack();
	const auto& hashes = animTrack.GetNodeHashes();

	m_AnimNodes.resize(hashes.size());
	for (int32_t i = 0; i < m_AnimNodes.size(); i++) 
	{
		m_AnimNodes[i].hash_name = hashes[i];
		
		std::pair<Keyframe, Keyframe> positionKeys, scalingKeys, rotationKeys;
		animTrack.GetFrames(m_AnimNodes[i].hash_name, m_CurrentTime, positionKeys, scalingKeys, rotationKeys);
		m_AnimNodes[i].position_k1 = positionKeys.first; 	m_AnimNodes[i].position_k2 = positionKeys.second;
		m_AnimNodes[i].scaling_k1 = scalingKeys.first; 	m_AnimNodes[i].scaling_k2 = scalingKeys.second;
		m_AnimNodes[i].rotation_k1 = rotationKeys.first; 	m_AnimNodes[i].rotation_k2 = rotationKeys.second;
	}
}


void Animator::Update(float_t deltaTime)
{
	if (m_CurrentAnimation)
	{
		ApplyAnimation();

		if (!m_AnimNodeBuffer)
			m_AnimNodeBuffer = std::unique_ptr<ShaderStorageBuffer>(
				ShaderStorageBuffer::Create(sizeof(AnimationNode) * m_AnimNodes.size(), 6));
		
		m_CurrentTime += deltaTime * m_CurrentAnimation->GetTicks();
		m_CurrentTime = fmodf(m_CurrentTime, m_CurrentAnimation->GetDuration());
		m_AnimNodeBuffer->SetData(m_AnimNodes.data(), 0, m_AnimNodes.size() * sizeof(AnimationNode));
	}
	m_FinalMatrices->Bind();
	m_NodeBuffer->Bind();
	m_BoneInfoBuffer->Bind();
	
	m_CS->Use();

	m_CS->SetFloat("currentTime", m_CurrentTime);
	m_CS->Dispatch(m_DispatchSize, 1, 1);

	m_FinalMatrices->Bind();
}
