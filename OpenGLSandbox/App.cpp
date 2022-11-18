#include "App.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <format>

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#ifdef __cplusplus
}
#endif

void glFrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	std::cout << "---------------\n";
	std::cout << "Debug message (" << id << "): " << message << "\n";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << "\n";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	}std::cout << "\n";

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: High"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: Medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: Low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: Notification"; break;
	} std::cout << std::endl;
}


App::App() { }
void App::Run()
{
	auto result = InitializeCore();
	if (result != DebugSource::Other)
	{
		Debug::Log("Failed to initialize core.", result, DebugType::Error, DebugSeverity::High);
		return;
	}
	if (!Initialize())
	{
		Debug::Log("Failed to initialize.", DebugSource::Application, DebugType::Error, DebugSeverity::Medium);
		return;
	}
	while (!glfwWindowShouldClose(mData))
	{
		glfwPollEvents();
		Update();
		Render();
	}
	Cleanup();
	CleanupCore();
}
const std::pair<int, int>& App::GetWindowSize() const
{
	return mSize;
}
std::string App::GetWindowTitle() const
{
	return mTitle;
}
std::string App::GetGLVersionString() const
{
	auto result = std::format("#version {}{}0", mGLVersion.first, mGLVersion.second);
	return result;
}
const std::pair<int, int>& App::GetGLVersionPair() const
{
	return mGLVersion;
}
GLFWwindow* App::GetData() const
{
	return mData;
}
void App::SetWindowSize(int width, int height)
{
	mSize = { width, height };
	glfwSetWindowSize(mData, mSize.first, mSize.second);
}
void App::SetWindowWidth(int width)
{
	mSize.first = width;
	glfwSetWindowSize(mData, mSize.first, mSize.second);

}
void App::SetWindowHeight(int height)
{
	mSize.second = height;
	glfwSetWindowSize(mData, mSize.first, mSize.second);

}
void App::SetWindowTitle(const std::string_view& title)
{
	mTitle = title;

	glfwSetWindowTitle(mData, mTitle.c_str());

}
void App::Close()
{
	glfwSetWindowShouldClose(mData, true);
}
DebugSource App::InitializeCore()
{
	if (!glfwInit())
	{
		return DebugSource::ThirdParty;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mGLVersion.first);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mGLVersion.second);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	mData = glfwCreateWindow(mSize.first, mSize.second, mTitle.c_str(), nullptr, nullptr);
	if (mData == nullptr)
	{
		return DebugSource::WindowSystem;
	}
	glfwMakeContextCurrent(mData);

	if (gl3wInit() != GL3W_OK)
	{
		return DebugSource::API;
	}

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	glfwSetFramebufferSizeCallback(mData, glFrameBufferSizeCallback);

	return DebugSource::Other;
}
void App::CleanupCore()
{
	glfwDestroyWindow(mData);
	glfwTerminate();
}