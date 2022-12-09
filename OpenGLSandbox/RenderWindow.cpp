#include "RenderWindow.h"
#include "Debug.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <format>

constexpr int OpenGLVersionMajor = 4;
constexpr int OpenGLVersionMinor = 5;

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#ifdef __cplusplus
}
#endif

static void glFrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         Debug::Critical(std::format("({}) {}", id, message)); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       Debug::Error(std::format("({}) {}", id, message));    break;
	case GL_DEBUG_SEVERITY_LOW:          Debug::Warn(std::format("({}) {}", id, message));     break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: Debug::Log(std::format("({}) {}", id, message));      break;
	}
}

RenderWindow::RenderWindow() { }
RenderWindow::RenderWindow(int width, int height, std::string_view title) : m_Width(width), m_Height(height), m_Title(title)
{
	if (!glfwInit())
	{
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_Data = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	if (m_Data == nullptr)
	{
		return;
	}
	glfwMakeContextCurrent(m_Data);

	if (gl3wInit() != GL3W_OK)
	{
		return;
	}

	Input = new ::Input(m_Data);

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}
void RenderWindow::Run()
{
	glfwDestroyWindow(m_Data);
	glfwTerminate();
}
int RenderWindow::GetWidth() const
{
	return m_Width;
}
int RenderWindow::GetHeight() const
{
	return m_Height;
}
std::string RenderWindow::GetTitle() const
{
	return m_Title;
}
GLFWwindow* RenderWindow::GetData() const
{
	return m_Data;
}
void RenderWindow::SetWidth(int width)
{
	m_Width = width;
	glfwSetWindowSize(m_Data, m_Width, m_Height);
}
void RenderWindow::SetHeight(int height)
{
	m_Height = height;
	glfwSetWindowSize(m_Data, m_Width, m_Height);
}
void RenderWindow::SetSize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	glfwSetWindowSize(m_Data, m_Width, m_Height);
}
void RenderWindow::SetTitle(std::string_view title)
{
	m_Title = title;
	glfwSetWindowTitle(m_Data, m_Title.c_str());
}
void RenderWindow::Close()
{
	glfwSetWindowShouldClose(m_Data, GL_TRUE);
}
bool RenderWindow::IsOpen() const
{
	return !glfwWindowShouldClose(m_Data);
}
void RenderWindow::UpdateFrame()
{
	glfwSwapBuffers(m_Data);
	glfwPollEvents();
}