#pragma once
#include <string>

class Texture2D
{
public:
	virtual ~Texture2D() = default;

	virtual void Bind(int32_t unit) const = 0;

	virtual int32_t GetHeight() const = 0;
	virtual int32_t GetWidth() const = 0;

	static Texture2D* Create(const std::string& filename);
};