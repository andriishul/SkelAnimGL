#include "IRenderer.hpp"
#include <openGL/glRenderer.hpp>

IRenderer* CreateRenderer()
{
	return new glRenderer;
}
