#include "Debug.h"
#include <format>
#include <iostream>

void Debug::Critical(std::string_view str, const std::source_location& location)
{
	std::cout << std::format("[\x1b[41mCRITICAL\x1b[0m] {}\nAt {}({}:{}) `{}`\n",
		str,
		location.file_name(),
		location.line(),
		location.column(),
		location.function_name())
		<< std::endl;
}
void Debug::Error(std::string_view str, const std::source_location& location)
{
	std::cout << std::format("[\x1b[91mERROR\x1b[0m] {}\nAt {}({}:{}) `{}`\n",
		str,
		location.file_name(),
		location.line(),
		location.column(),
		location.function_name())
		<< std::endl;
}
void Debug::Warn(std::string_view str, const std::source_location& location)
{
	std::cout << std::format("[\x1b[33mWARN\x1b[0m] {}\nAt {}({}:{}) `{}`\n",
		str,
		location.file_name(),
		location.line(),
		location.column(),
		location.function_name())
		<< std::endl;
}
void Debug::Log(std::string_view str, const std::source_location& location)
{
	std::cout << std::format("[\x1b[94mLOG\x1b[0m] {}\nAt {}({}:{}) `{}`\n",
		str,
		location.file_name(),
		location.line(),
		location.column(),
		location.function_name())
		<< std::endl;
}