#include "App.h"
#include "Input.h"
#include "Shader.h"
#include "Debug.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <format>

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

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         Debug::Critical(std::format("({}) {}", id, message)); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       Debug::Error(std::format("({}) {}", id, message));    break;
	case GL_DEBUG_SEVERITY_LOW:          Debug::Warn(std::format("({}) {}", id, message));     break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: Debug::Log(std::format("({}) {}", id, message));      break;
	}
}

App::App() { }
void App::Run()
{
	if (!InitializeCore())
	{
		Debug::Critical("Failed to initialize core.");
		return;
	}
	if (!Initialize())
	{
		Debug::Error("Failed to initialize.");
		return;
	}

	Shader shader("Shaders/shader.vert", "Shaders/shader.frag");

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	GLuint bufferObject = 0;
	glGenBuffers(1, &bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint arrayObject = NULL;
	glGenVertexArrays(1, &arrayObject);
	glBindVertexArray(arrayObject);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(m_Data))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		glBindVertexArray(arrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(m_Data);
		Update();
		Render();
	}
	Cleanup();
	CleanupCore();
}
const std::pair<int, int>& App::GetWindowSize() const
{
	return m_Size;
}
std::string App::GetWindowTitle() const
{
	return m_Title;
}
std::string App::GetGLVersionString() const
{
	std::string result = std::format("#version {}{}0", m_GLVersion.first, m_GLVersion.second);
	return result;
}
const std::pair<int, int>& App::GetGLVersionPair() const
{
	return m_GLVersion;
}
GLFWwindow* App::GetData() const
{
	return m_Data;
}
void App::SetWindowSize(int width, int height)
{
	m_Size = { width, height };
	glfwSetWindowSize(m_Data, m_Size.first, m_Size.second);
}
void App::SetWindowWidth(int width)
{
	m_Size.first = width;
	glfwSetWindowSize(m_Data, m_Size.first, m_Size.second);
}
void App::SetWindowHeight(int height)
{
	m_Size.second = height;
	glfwSetWindowSize(m_Data, m_Size.first, m_Size.second);
}
void App::SetWindowTitle(std::string_view title)
{
	m_Title = title;

	glfwSetWindowTitle(m_Data, m_Title.c_str());
}
void App::Close()
{
	glfwSetWindowShouldClose(m_Data, true);
}
bool App::InitializeCore()
{
	if (!glfwInit())
	{
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_GLVersion.first);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_GLVersion.second);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_Data = glfwCreateWindow(m_Size.first, m_Size.second, m_Title.c_str(), nullptr, nullptr);
	if (m_Data == nullptr)
	{
		return false;
	}
	glfwMakeContextCurrent(m_Data);

	if (gl3wInit() != GL3W_OK)
	{
		return false;
	}

	Input::SetWindow(m_Data);

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

	glfwSetFramebufferSizeCallback(m_Data, glFrameBufferSizeCallback);

	return true;
}
void App::CleanupCore()
{
	glfwDestroyWindow(m_Data);
	glfwTerminate();
}