#include "Debug.h"
#include <GL/gl3w.h>

void Debug::Log(std::string_view str)
{
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, -1, str.data());
}
void Debug::Log(std::string_view str, DebugSource source, DebugType type, DebugSeverity severity)
{
	glDebugMessageInsert(static_cast<GLenum>(source), static_cast<GLenum>(type), 0, static_cast<GLenum>(severity), -1, str.data());
}