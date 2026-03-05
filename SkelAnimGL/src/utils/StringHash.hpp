#pragma once
#include <string>

inline int StringToHash(const std::string& value)
{
	return std::hash<std::string>{}(value);
}
