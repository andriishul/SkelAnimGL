#pragma once
#include <core/Transform.hpp>
#include "Mesh.hpp"

class MeshRenderer
{
public:
	MeshRenderer(const std::vector<std::shared_ptr<Mesh>>& meshes = {});

	void AddMesh(const std::shared_ptr<Mesh>& mesh) { m_Meshes.push_back(mesh); }

	void SetTransform(const Transform& transform) { m_Transform = transform; }

	void Render();
private:
	Transform m_Transform;
	std::vector<std::shared_ptr<Mesh>> m_Meshes;
	std::unique_ptr<UniformBuffer> m_TransformDataBuffer;
	std::shared_ptr<Shader> m_Shader;
};

