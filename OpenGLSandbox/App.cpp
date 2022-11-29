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
	auto vertexShaderCode = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";
	auto fragmentShaderCode = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)";
	GLuint vertexShader = NULL;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = NULL;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = NULL;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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

	while (!glfwWindowShouldClose(m_data))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(arrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(m_data);
		Update();
		Render();
	}
	Cleanup();
	CleanupCore();
}
const std::pair<int, int>& App::GetWindowSize() const
{
	return m_size;
}
std::string App::GetWindowTitle() const
{
	return m_title;
}
std::string App::GetGLVersionString() const
{
	auto result = std::format("#version {}{}0", m_glVersion.first, m_glVersion.second);
	return result;
}
const std::pair<int, int>& App::GetGLVersionPair() const
{
	return m_glVersion;
}
GLFWwindow* App::GetData() const
{
	return m_data;
}
void App::SetWindowSize(int width, int height)
{
	m_size = { width, height };
	glfwSetWindowSize(m_data, m_size.first, m_size.second);
}
void App::SetWindowWidth(int width)
{
	m_size.first = width;
	glfwSetWindowSize(m_data, m_size.first, m_size.second);
}
void App::SetWindowHeight(int height)
{
	m_size.second = height;
	glfwSetWindowSize(m_data, m_size.first, m_size.second);
}
void App::SetWindowTitle(const std::string_view& title)
{
	m_title = title;

	glfwSetWindowTitle(m_data, m_title.c_str());
}
void App::Close()
{
	glfwSetWindowShouldClose(m_data, true);
}
DebugSource App::InitializeCore()
{
	if (!glfwInit())
	{
		return DebugSource::ThirdParty;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_glVersion.first);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_glVersion.second);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_data = glfwCreateWindow(m_size.first, m_size.second, m_title.c_str(), nullptr, nullptr);
	if (m_data == nullptr)
	{
		return DebugSource::WindowSystem;
	}
	glfwMakeContextCurrent(m_data);

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

	glfwSetFramebufferSizeCallback(m_data, glFrameBufferSizeCallback);

	return DebugSource::Other;
}
void App::CleanupCore()
{
	glfwDestroyWindow(m_data);
	glfwTerminate();
}