#pragma once
#include "Mesh.hpp"
#include "Skeleton.h"
#include "Animation.hpp"

struct Model 
{
	explicit Model(Skeleton&& _skeleton, const std::vector<std::shared_ptr<Mesh>>&& _meshes, std::vector<Animation>&& _animations);
	
	const std::vector<std::shared_ptr<Mesh>> meshes;
	const std::vector<Animation> animations;
	const Skeleton skeleton;
};