#pragma once
#include <graphics/Texture.hpp>

class glTexture2D : public Texture2D
{
public:
	virtual ~glTexture2D();
	glTexture2D(const std::string& filename);

	virtual void Bind(int32_t unit) const override;

	virtual int32_t GetHeight() const override { return m_Height; }
	virtual int32_t GetWidth() const override { return m_Width; }
private:
	uint32_t m_ID, m_Format;
	int32_t m_Height, m_Width;
};