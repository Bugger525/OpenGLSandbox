#pragma once

#include <string>

enum class DebugSource : unsigned int
{
	ThirdParty = 0x8249,
	Application = 0x824A,
};
enum class DebugType : unsigned int
{
	Error = 0x824C,
	DeprecatedBehavior = 0x824D,
	UndefinedBehavior = 0x824E,
	Portability = 0x824F,
	Performance = 0x8250,
	Marker = 0x8268,
	PushGroup = 0x8269,
	PopGroup = 0x826A,
	Other = 0x8251
};
enum class DebugSeverity : unsigned int
{
	High = 0x9146,
	Medium = 0x9147,
	Low = 0x9148,
	Notification = 0x826B
};
class Debug
{
public:
	static void Log(std::string_view str);
	static void Log(std::string_view str, DebugSource source, DebugType type, DebugSeverity severity);
};