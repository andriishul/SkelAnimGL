#pragma once
#include <glm/glm.hpp>

struct Transform 
{
	Transform(const glm::vec3& position = glm::vec3(0.f),
		const glm::vec3& scale = glm::vec3(1.f),
		const glm::vec3& rotation = glm::vec3(0.f)
		) : Position(position), Rotation(rotation), Scale(scale) {}
	glm::vec3 Position, Rotation, Scale;
};