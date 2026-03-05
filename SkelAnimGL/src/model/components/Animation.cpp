#include "Animation.hpp"
#include <utils/Logger.hpp>
static int32_t findFrameIndex(float_t animTime, const std::vector<Keyframe>& nodeKeys)
{
    int32_t count = nodeKeys.size();
    if (count == 0)
        return -1;

    if (animTime <= nodeKeys[0].time)
        return 0;

    if (animTime >= nodeKeys[count - 1].time)
        return count - 1;

    auto it = std::lower_bound(nodeKeys.begin(), nodeKeys.end(), animTime, [&](const Keyframe& frame, float value) {   return frame.time <= value;});

    return std::distance(nodeKeys.begin(), it) - 1;
}

Animation::Animation(const std::string& name, const AnimationTrack& track, float_t duration, float_t ticks) :
	m_Name(name), m_Duration(duration), m_Ticks(ticks), m_AnimTrack(track)
{

}

void AnimationTrack::AddAnimNode(uint32_t hash_name, const NodeKeyframes& nodeKeyframes)
{
    m_AnimNodeHashes.push_back(hash_name);
    m_AnimNodesFrames.push_back(nodeKeyframes);
}

void AnimationTrack::GetFrames(uint32_t hash_name, float_t animTime, std::pair<Keyframe, Keyframe>& position,
    std::pair<Keyframe, Keyframe>& scale, std::pair<Keyframe, Keyframe>& rotation) const
{
    const NodeKeyframes& nodeFrames = GetFramesByHash(hash_name);
    int32_t positionIndex = findFrameIndex(animTime, nodeFrames.positionKeys);
    int32_t rotationIndex = findFrameIndex(animTime, nodeFrames.rotationKeys);
    int32_t scalingIndex = findFrameIndex(animTime, nodeFrames.scalingKeys);

    position.first = nodeFrames.positionKeys[positionIndex];
    rotation.first = nodeFrames.rotationKeys[rotationIndex];
    scale.first = nodeFrames.scalingKeys[scalingIndex];

    if (positionIndex < nodeFrames.positionKeys.size() - 1)
        positionIndex++;
    if (rotationIndex < nodeFrames.rotationKeys.size() - 1)
        rotationIndex++;
    if (scalingIndex < nodeFrames.scalingKeys.size() - 1)
        scalingIndex++;

    position.second = nodeFrames.positionKeys[positionIndex];
    rotation.second = nodeFrames.rotationKeys[rotationIndex];
    scale.second = nodeFrames.scalingKeys[scalingIndex];
}
const NodeKeyframes& AnimationTrack::GetFramesByHash(uint32_t hash_name) const
{
    auto it = std::find(m_AnimNodeHashes.begin(), m_AnimNodeHashes.end(), hash_name);
    if (it != m_AnimNodeHashes.end())
    {
        int32_t index = std::distance(m_AnimNodeHashes.begin(), it);
        return m_AnimNodesFrames[index];
    } 
    LOG_ERROR("Could not find frames for animation node with hash: {}", std::to_string(hash_name));
}