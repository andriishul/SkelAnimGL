#include "Texture.hpp"
#include <openGL/glTexture.hpp>
Texture2D* Texture2D::Create(const std::string& filename)
{
	return new glTexture2D(filename);
}
