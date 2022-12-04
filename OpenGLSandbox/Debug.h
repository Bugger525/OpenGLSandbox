#pragma once

#include <string>
#include <source_location>

class Debug
{
public:
	Debug() = delete;

	static void Critical(std::string_view str, const std::source_location& location = std::source_location::current());
	static void Error(std::string_view str, const std::source_location& location = std::source_location::current());
	static void Warn(std::string_view str, const std::source_location& location = std::source_location::current());
	static void Log(std::string_view str, const std::source_location& location = std::source_location::current());
};