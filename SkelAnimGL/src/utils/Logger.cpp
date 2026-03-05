#include "Logger.hpp"
#include <utils/_internal/ConsoleLogger.hpp>
std::unique_ptr<ILogger> Logger::s_Logger = std::make_unique<ConsoleLogger>();