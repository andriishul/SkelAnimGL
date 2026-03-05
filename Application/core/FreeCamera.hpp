#pragma once
#include <core/Camera.hpp>

class FreeCamera : public Camera
{
private:
	void CalculateViewMatrix();
	void MoveForward(float_t speed);
	void MoveRight(float_t speed);
	void Rotate(const glm::vec3& rotation);
public:
	FreeCamera(float_t fov, float_t near, float_t far, const glm::vec3 &position, const glm::vec3& rotation);

	virtual void OnWindowResized(float_t width, float_t height) override;
	virtual void OnKeyPressed(int32_t key, int32_t scancode, int32_t action, int32_t mods) override;

	virtual void Update(float deltaTime) override { CalculateViewMatrix(); }

	const glm::mat4& GetProjection() const override { return m_Projection; }
	const glm::mat4& GetView() const override { return m_View; }
private:
	float m_Fov, m_Near, m_Far;
	const float m_MaxRotation;
	
	glm::mat4 m_Projection, m_View;
	glm::vec3 m_Rotation, m_Position, m_Front, m_Up, m_Right;
};