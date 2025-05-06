#include "Model.hpp"

Model::Model(const Skeleton& _skeleton, const std::vector<std::shared_ptr<Mesh>>& _meshes, const std::vector<Animation>& _animations) :
	meshes(_meshes), skeleton(_skeleton), animations(_animations)
{
}