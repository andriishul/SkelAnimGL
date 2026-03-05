#pragma once
#include "ILogger.hpp"

#include <fmt/core.h>
#include <memory>

class Logger 
{
public:
	static inline void SetLogger(std::unique_ptr<ILogger>&& logger) { s_Logger = std::move(logger); }

	static inline void Log(Level level, const std::string& str, const std::string& file, int32_t line) { s_Logger->Log(level, str, file, line); }
private:
	static std::unique_ptr<ILogger> s_Logger;
};

#define LOG_INFO(str, ...) Logger::Log(Level::INFO, fmt::format(str, ##__VA_ARGS__), __FILE__, __LINE__)
#define LOG_WARN(str, ...) Logger::Log(Level::WARNING, fmt::format(str, ##__VA_ARGS__), __FILE__, __LINE__)
#define LOG_ERROR(str, ...) \
    do { \
        Logger::Log(Level::ERROR, fmt::format(str, ##__VA_ARGS__), __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } while (0)
#define LOG_DEBUG(str, ...) Logger::Log(Level::DEBUG, fmt::format(str, ##__VA_ARGS__), __FILE__, __LINE__)
