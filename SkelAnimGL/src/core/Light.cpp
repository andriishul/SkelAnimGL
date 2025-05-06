#include "Light.hpp"

Light::Light(const glm::vec3& position, const glm::vec3& color)
{
	SetColor(color);
	SetPosition(position);
}