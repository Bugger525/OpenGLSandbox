#pragma once

#include <string>
#include <format>
#include <source_location>

#define DEBUG_CRITICAL(fmt, ...) Debug::Critical(std::format(fmt, ##__VA_ARGS__));
#define DEBUG_ERROR(fmt, ...) Debug::Error(std::format(fmt, ##__VA_ARGS__));
#define DEBUG_WARN(fmt, ...) Debug::Warn(std::format(fmt, ##__VA_ARGS__));
#define DEBUG_INFO(fmt, ...) Debug::Info(std::format(fmt, ##__VA_ARGS__));

class Debug
{
public:
	Debug() = delete;

	static void Critical(std::string_view str, const std::source_location& location = std::source_location::current());
	static void Error(std::string_view str, const std::source_location& location = std::source_location::current());
	static void Warn(std::string_view str, const std::source_location& location = std::source_location::current());
	static void Info(std::string_view str, const std::source_location& location = std::source_location::current());
};