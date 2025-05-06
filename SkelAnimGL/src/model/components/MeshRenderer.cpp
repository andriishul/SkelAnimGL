#include "MeshRenderer.hpp"
#include <glm/ext/matrix_transform.hpp>

MeshRenderer::MeshRenderer(const std::vector<std::shared_ptr<Mesh>>& meshes)
	: m_Shader(Shader::Get("skinned")), 
	m_TransformDataBuffer(std::unique_ptr<UniformBuffer>(UniformBuffer::Create(sizeof(glm::mat4) * 2, 2))), 
	m_Meshes(meshes)
{
}

void MeshRenderer::Render()
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Transform.Position);

	model = glm::rotate(model, glm::radians(m_Transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(m_Transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_Transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	model = glm::scale(model, m_Transform.Scale);

	m_TransformDataBuffer->SetData(&model[0][0], 0, sizeof(model));
	for (auto& mesh : m_Meshes)
		mesh->Render(*m_Shader);
}
