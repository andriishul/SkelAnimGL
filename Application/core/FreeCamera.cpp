#include "FreeCamera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <GLFW/glfw3.h>

FreeCamera::FreeCamera(float_t fov, float_t near, float_t far, const glm::vec3& position, const glm::vec3& rotation)
    : m_Fov(fov), m_Near(near), m_Far(far), m_Position(position), m_Rotation(rotation), 
    m_MaxRotation(360.f), m_Front(1.f, 0.f, 0), m_Up(0, 1, 0), m_Right(0.f, 0.f, 1.f)
{
    CalculateViewMatrix();
}

void FreeCamera::CalculateViewMatrix()
{
    m_Rotation = glm::mod(m_Rotation, m_MaxRotation);

    glm::vec3 radians = glm::radians(m_Rotation);
    // rotating using yaw and pitch
    m_Front = glm::normalize(glm::vec3(
        cos(radians.z) * cos(radians.y),  
        sin(radians.y),                   
        sin(radians.z) * cos(radians.y) 
    ));

    m_Right = glm::normalize(glm::cross(m_Front, m_Up));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));

    float_t cosRoll = cos(radians.x);
    float_t sinRoll = sin(radians.x);

    m_Right = cosRoll * m_Right + sinRoll * m_Up;
    m_Up = -sinRoll * m_Right + cosRoll * m_Up;

    m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void FreeCamera::MoveForward(float_t speed)
{
    m_Position += m_Front * speed;
}

void FreeCamera::MoveRight(float_t speed)
{
    m_Position += m_Right * speed;
}

void FreeCamera::Rotate(const glm::vec3& rotation)
{
    m_Rotation += rotation;
}

void FreeCamera::OnWindowResized(float_t width, float_t height) 
{
    if (width > 0.f && height > 0.f)
    {
        float_t aspectRatio = static_cast<float_t>(width) / static_cast<float_t>(height);
        m_Projection = glm::perspective(glm::radians(m_Fov), aspectRatio, m_Near, m_Far);
    }
}

void FreeCamera::OnKeyPressed(int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    if (key == GLFW_KEY_W)
        MoveForward(1.f);
    else if (key == GLFW_KEY_S)
        MoveForward(-1.f);
    else if (key == GLFW_KEY_A)
        MoveRight(-1.f);
    else if (key == GLFW_KEY_D)
        MoveRight(1.f);
    
    if (key == GLFW_KEY_RIGHT)
        Rotate({ 0.f, 0.f, 1.f });
    if (key == GLFW_KEY_LEFT)
        Rotate({ 0.f, 0.f,  -1.f });
}
