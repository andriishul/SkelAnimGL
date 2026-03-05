#pragma once
#include "IRenderer.hpp"

#include <memory>
#include <stdexcept>
class RendererContext 
{
public:
	// Initializes the rendering backend if it hasn't been already.
	static inline void Init() 
	{
		if (!s_Renderer)
			s_Renderer = std::unique_ptr<IRenderer>(CreateRenderer());
	}
	static inline void DrawIndexed(VertexArray& vao) 
	{
		if (s_Renderer)
			s_Renderer->DrawIndexed(vao);
		else throw std::runtime_error("RendererContext isnot initialized\n");
	}
private:
	static std::unique_ptr<IRenderer> s_Renderer;
};