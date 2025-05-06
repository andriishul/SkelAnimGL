#pragma once
#include <string>

enum class Level
{
	INFO,
	ERROR,
	WARNING,
	DEBUG
};

inline std::string LevelToString(Level l) 
{
	return l == Level::INFO ? "INFO" :
		l == Level::ERROR ? "ERROR" :
		l == Level::WARNING ? "WARNING" :
		l == Level::INFO ? "DEBUG" : "UNKNOWN";
}

class ILogger
{
public:
	virtual ~ILogger() = default;

	virtual void Log(Level level, const std::string& str, const std::string& file, int32_t line) = 0;
};


