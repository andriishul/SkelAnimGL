#pragma once
#include <glm/glm.hpp>

class Camera 
{
public:
	virtual ~Camera() = default;

	virtual void OnWindowResized(float_t width, float_t height) = 0;
	virtual void OnKeyPressed(int32_t key, int32_t scancode, int32_t action, int32_t mods) = 0;

	virtual const glm::mat4& GetProjection() const = 0;
	virtual const glm::mat4& GetView() const = 0;

	virtual void Update(float deltaTime) = 0;
};