#include "glTexture.hpp"
#include "utils/Logger.hpp"
#include "glad/glad.h"
#include <stb_image.h>

glTexture2D::glTexture2D(const std::string& filename)
{
	int32_t x, y, nrChannels;
	
	stbi_uc* data = stbi_load(filename.c_str(), &x,&y, &nrChannels, 0);
	m_Height = y, m_Width = x;
	if (!data)
	{
		LOG_ERROR("STB::ERROR Failed to load {}\n{}", filename, stbi_failure_reason());
	}
	switch (nrChannels)
	{
	case 1:
		m_Format = GL_RED;
		break;
	case 2:
		m_Format = GL_RG;
		break; 
	case 3:
		m_Format = GL_RGB;
		break;
	case 4:
		m_Format = GL_RGBA;
		break;
	}
	glGenTextures(1,&m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_Format, x, y, 0, m_Format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}
glTexture2D::~glTexture2D() 
{
	if (glIsTexture(m_ID))
		glDeleteTextures(1, &m_ID);
}


void glTexture2D::Bind(int32_t unit) const
{
	glBindTextureUnit(unit, m_ID);
}
