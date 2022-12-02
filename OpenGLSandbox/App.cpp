#include "App.h"
#include "Shader.h"

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
	if (!InitializeCore())
	{
		Debug::Log("Failed to initialize core.", DebugSource::Application, DebugType::Error, DebugSeverity::High);
		return;
	}
	if (!Initialize())
	{
		Debug::Log("Failed to initialize.", DebugSource::Application, DebugType::Error, DebugSeverity::Medium);
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
	auto result = std::format("#version {}{}0", m_GLVersion.first, m_GLVersion.second);
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

	Input = new ::Input(this);

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