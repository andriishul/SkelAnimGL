#pragma once
#include "Transform.hpp"

class GameObject 
{
public:
	virtual ~GameObject() = default;

	virtual void SetTransform(const Transform& transform) { m_Transform = transform; }
	virtual const Transform& GetTranform() const { return m_Transform; }

	virtual void Update(float_t deltaTime) {}
	virtual void Render() {}
private:
	Transform m_Transform;
};