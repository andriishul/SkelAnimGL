#pragma once
#include <string>
#include <vector>

#include <glm/glm.hpp>

struct Keyframe 
{
	float_t time;

	alignas(16)	glm::vec4 value;
};

// Contains multiple keyframes for a animated node
struct NodeKeyframes
{
	std::vector<Keyframe> positionKeys, rotationKeys, scalingKeys;
};

// Contains animation data for all nodes in a track
struct AnimationTrack
{
public:
	void AddAnimNode(uint32_t hash_name, const NodeKeyframes& nodeKeyframes);
	
	// Retrieves the current and next keyframes (position, rotation, scale) based on animTime for a specific node.
	// It looks up keyframes using hash_name and stores them in pairs for position, rotation, and scale.
	void GetFrames(uint32_t hash_name, float_t animTime, std::pair<Keyframe, Keyframe>& position,
		std::pair<Keyframe, Keyframe>& scale, std::pair<Keyframe, Keyframe>& rotation) const;

	const std::vector<uint32_t>& GetNodeHashes() const{ return m_AnimNodeHashes; }
private:
	const NodeKeyframes& GetFramesByHash(uint32_t hash_name) const;
private:
	std::vector<uint32_t> m_AnimNodeHashes;
	std::vector<NodeKeyframes> m_AnimNodesFrames;
};


class Animation
{
public:
	Animation(const std::string& name, const AnimationTrack& track, float_t duration, float_t ticks);

	const AnimationTrack& GetTrack() const { return m_AnimTrack; }
	float GetDuration() const { return m_Duration; }
	float GetTicks() const { return m_Ticks; }

	const std::string& GetName() const { return m_Name; }
private:
	AnimationTrack m_AnimTrack;     
	const std::string m_Name;     
	float_t m_Duration, m_Ticks;    
};