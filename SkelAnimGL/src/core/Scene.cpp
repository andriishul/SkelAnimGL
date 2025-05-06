#include "Scene.hpp"
#include <utils/Logger.hpp>

Scene::Scene(const std::string& name) : m_Name(name),
	m_CameraBuffer(std::unique_ptr<UniformBuffer>(UniformBuffer::Create(sizeof(glm::mat4) * 2, 1))),
	m_LightBuffer(std::unique_ptr<UniformBuffer>(UniformBuffer::Create(sizeof(glm::mat2x4) * 32, 3)))
{

}

void Scene::AddLight(const std::shared_ptr<Light>& light) 
{ 
	if (m_LightsInUse >= m_Lights.size()) 
	{
		LOG_ERROR("Exceeded maximum number of lights in Scene {} (32)\n", m_Name);
	}
	m_Lights[m_LightsInUse++] = light;
}

void Scene::Update(float_t deltaTime)
{
	m_Camera->Update(deltaTime);

	m_DeltaTime = deltaTime;
}

void Scene::Render()
{
	std::array<glm::mat2x4, 32> lightsInfo;
	for (int32_t i = 0; i < m_LightsInUse; i++)
	{
		lightsInfo[i] = glm::mat2x4
		{
			glm::vec4(m_Lights[i]->GetPosition(), 0.f),
			glm::vec4(m_Lights[i]->GetColor(), 0.f)
		};
	}

	glm::mat4 projectionView[2] =
	{
		m_Camera->GetProjection(),
		m_Camera->GetView()
	};

	m_LightBuffer->SetData(lightsInfo.data(), 0 , sizeof(glm::mat2x4) * m_LightsInUse);
	m_CameraBuffer->SetData(&projectionView[0][0], 0, sizeof(glm::mat4) * 2);

	for (const auto& gObject : m_Objects) 
	{
		gObject->Update(m_DeltaTime);
		gObject->Render();
	}
}
