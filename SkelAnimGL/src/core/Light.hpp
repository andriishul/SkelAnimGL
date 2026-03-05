#pragma once
#include <glm/glm.hpp>

class Light 
{
public:
	Light(const glm::vec3& position, const glm::vec3& color);

	void SetColor(const glm::vec3& color) { m_Color = color; }
	void SetPosition(const glm::vec3& position) { m_Position = position; }

	const glm::vec3& GetColor() const { return m_Color; }
	const glm::vec3& GetPosition() const { return m_Position; }
private:
	glm::vec3 m_Color, m_Position;
};