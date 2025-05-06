#include "ConsoleLogger.hpp"
#include <fmt/color.h>
void ConsoleLogger::Log(Level level, const std::string& str, const std::string& file, int32_t line)
{
	std::string full_str = ": {}\nfile: {}\tline: {}\n";

    fmt::color color;

    switch (level) 
    {
    case Level::INFO:
        color = fmt::color::green;  
        break;
    case Level::ERROR:
        color = fmt::color::red;          
        break;
    case Level::WARNING:
        color = fmt::color::yellow;      
        break;
    case Level::DEBUG:
        color = fmt::color::cyan;       
        break;
    default:
        color = fmt::color::white;
        break;
    }

    fmt::print("[");
	fmt::print(fmt::fg(color), LevelToString(level));
    fmt::print("]");

    fmt::print(full_str, str, file, line);
}
