#pragma once
#include "Buffer.hpp"

class IRenderer 
{
public:
	virtual void DrawIndexed(VertexArray& vao) = 0;
};

IRenderer* CreateRenderer();