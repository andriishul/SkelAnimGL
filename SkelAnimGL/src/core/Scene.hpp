#pragma once
#include "Light.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"

#include <graphics/Buffer.hpp>
#include <array>
#include <memory>

class Scene 
{
public: 
	Scene(const std::string& name);

	void SetCamera(const std::shared_ptr<Camera>& camera) { m_Camera = camera; }
	void AddLight(const std::shared_ptr<Light>& light);
	void AddObject(const std::shared_ptr<GameObject>& gObject) { m_Objects.push_back(gObject); }

	void Update(float_t deltaTime);
	void Render();
private:
	std::string m_Name;

	int32_t m_LightsInUse = 0;
	std::array<std::shared_ptr<Light>, 32> m_Lights;
	std::shared_ptr<Camera> m_Camera;
	std::vector<std::shared_ptr<GameObject>> m_Objects;

	float_t m_DeltaTime = 0;
	std::unique_ptr<UniformBuffer> m_CameraBuffer, m_LightBuffer;
};