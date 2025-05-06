#pragma once
#include <graphics/IRenderer.hpp>

class glRenderer : public IRenderer
{
public:
	virtual void DrawIndexed(VertexArray& vao) override;
};