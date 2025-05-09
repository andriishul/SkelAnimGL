#include "Model.hpp"

Model::Model(Skeleton&& _skeleton, const std::vector<std::shared_ptr<Mesh>>&& _meshes, std::vector<Animation>&& _animations) :
	meshes(_meshes), skeleton(_skeleton), animations(_animations)
{
}