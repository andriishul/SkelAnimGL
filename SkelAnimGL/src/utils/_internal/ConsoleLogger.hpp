#pragma once
#include <utils/Logger.hpp>

class ConsoleLogger : public ILogger
{
public:
	~ConsoleLogger() = default;

	virtual void Log(Level level, const std::string& str, const std::string& file, int32_t line) override;
};