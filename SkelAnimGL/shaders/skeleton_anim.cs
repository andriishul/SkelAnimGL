#version 450 core

layout(local_size_x = 32) in;

uniform float currentTime;

layout(std140, binding = 0) buffer FinalMatrices {
    mat4 boneTransforms[];
};

struct BoneNode 
{
    uint hash_name;
    int parentIndex;
    
    mat4 transformation;
};

struct BoneInfo 
{
    uint hash_name;

    mat4 offset;
};

struct Keyframe 
{
    float time;

    vec4 value;
};

struct AnimNode
{
    uint hash_name;
	Keyframe position_k1, position_k2;
	Keyframe scaling_k1, scaling_k2;
	Keyframe rotation_k1, rotation_k2;
};

layout(std430, binding = 4) buffer BoneInfoBuffer 
{
    BoneInfo boneInfos[];
};

layout(std430, binding = 5) buffer NodeBuffer 
{    
    BoneNode nodes[];
};

layout(std430, binding = 6) buffer AnimNodeBuffer
{
    AnimNode animNodes[];
};

int GetNodeIndexByName(uint hash_name);
int GetAnimNodeIndexByName(uint hash_name);

vec4 Lerp(Keyframe k1, Keyframe k2);
vec4 Slerp(Keyframe k1, Keyframe k2);
mat4 Interpolate(AnimNode nodeAnim);

mat4 Scale(vec4 scale);
mat4 Translate(vec4 translation);
mat4 QuatToMat4(vec4 quat);

void main() 
{
    uint boneIndex = gl_GlobalInvocationID.x;
    if (boneIndex >= 0 && boneIndex < boneInfos.length()) 
    {
        BoneInfo bone = boneInfos[boneIndex];

        int nodeIndex = GetNodeIndexByName(bone.hash_name);
        if (nodeIndex != -1 && nodeIndex < nodes.length()) 
        {
            mat4 globalTransformation = mat4(1.0);
            int currentIndex = nodeIndex;

            while (currentIndex != -1) 
            {
                BoneNode currentNode = nodes[currentIndex];
                mat4 nodeTransformation = currentNode.transformation;
                int animNodeIndex = GetAnimNodeIndexByName(currentNode.hash_name);
                if(animNodeIndex != -1)
                {
                    AnimNode animNode = animNodes[animNodeIndex];
                    nodeTransformation = Interpolate(animNode);
                }
             
                globalTransformation = nodeTransformation * globalTransformation;
                currentIndex = nodes[currentIndex].parentIndex;
            }
            globalTransformation = globalTransformation * bone.offset;

            boneTransforms[boneIndex] = globalTransformation;

        }
    }
}

int GetNodeIndexByName(uint hash_name)
{
	for(int i = 0; i < nodes.length(); i++)
	{
		if(nodes[i].hash_name == hash_name)
			return i;
	}
	return -1;
}

int GetAnimNodeIndexByName(uint hash_name)
{
	for(int i = 0; i < nodes.length(); i++)
	{
		if(animNodes[i].hash_name == hash_name)
			return i;
	}
	return -1;
}

vec4 Lerp(Keyframe k1, Keyframe k2)
{        
    float t = clamp((currentTime - k1.time) / (k2.time - k1.time), 0.0f, 1.0f);
    return mix(k1.value, k2.value, t);
}

vec4 Slerp(Keyframe k1, Keyframe k2)
{
    float t = clamp((currentTime - k1.time) / (k2.time - k1.time), 0.0f, 1.0f);
    vec4 q1 = k1.value;
    vec4 q2 = k2.value;

    float dotProduct = dot(q1, q2);
    if (dotProduct < 0.0) 
    {
        q2 = -q2;
        dotProduct = -dotProduct;
    }

    float theta = acos(dotProduct);
    if (theta > 0.001) 
    {
        return (sin((1.0 - t) * theta) * q1 + sin(t * theta) * q2) / sin(theta);
    }
    return mix(q1, q2, t);
}

mat4 Interpolate(AnimNode nodeAnim)
{
    vec4 position = Lerp(nodeAnim.position_k1, nodeAnim.position_k2); 
    vec4 scaling = Lerp(nodeAnim.scaling_k1, nodeAnim.scaling_k2);
    vec4 rotation = Slerp(nodeAnim.rotation_k1, nodeAnim.rotation_k2);

    return Translate(position) * Scale(scaling) * QuatToMat4(rotation);
}

mat4 Scale(vec4 scale)
{
    return mat4(
        scale.x, 0.0,    0.0,    0.0,
        0.0,    scale.y, 0.0,    0.0,
        0.0,    0.0,    scale.z, 0.0,
        0.0,    0.0,    0.0,    1.0
    );
}

mat4 Translate(vec4 translation)
{
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        translation.x, translation.y, translation.z, 1.0
    );
}

mat4 QuatToMat4(vec4 quat) 
{
    float x = quat.x, y = quat.y, z = quat.z, w = quat.w;

    float xx = x * x, yy = y * y, zz = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;

    return mat4(
        1.0 - 2.0 * (yy + zz), 2.0 * (xy - wz),     2.0 * (xz + wy),     0.0,
        2.0 * (xy + wz),        1.0 - 2.0 * (xx + zz), 2.0 * (yz - wx),     0.0,
        2.0 * (xz - wy),        2.0 * (yz + wx),     1.0 - 2.0 * (xx + yy), 0.0,
        0.0,                   0.0,                0.0,                1.0
    );
}