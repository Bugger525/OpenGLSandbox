#include "Shader.h"
#include "Debug.h"
#include <fstream>
#include <sstream>
#include <format>
#include <iostream>
#include <GL/gl3w.h>

Shader::Shader(std::string_view vertexShaderPath, std::string_view fragmentShaderPath)
{
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(vertexShaderPath.data());
		fragmentShaderFile.open(fragmentShaderPath.data());

		std::stringstream vStream;
		std::stringstream fStream;

		vStream << vertexShaderFile.rdbuf();
		fStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexShaderCode = vStream.str();
		fragmentShaderCode = fStream.str();
	}
	catch (std::ifstream::failure e)
	{
		Debug::Log("Failed to read shader codes.", DebugSource::Application, DebugType::Error, DebugSeverity::Medium);
		return;
	}

	auto cVertexShaderCode = vertexShaderCode.c_str();
	auto cFragmentShaderCode = fragmentShaderCode.c_str();

	int success = 0;
	char infoLog[512]{};

	unsigned int vertexShader = NULL;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cVertexShaderCode, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		Debug::Log(
			std::format("Failed to compile vertex shader : {}", infoLog),
			DebugSource::Application,
			DebugType::Error,
			DebugSeverity::High
		);
		return;
	}

	unsigned int fragmentShader = NULL;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cFragmentShaderCode, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		Debug::Log(
			std::format("Failed to compile fragment shader : {}", infoLog),
			DebugSource::Application,
			DebugType::Error,
			DebugSeverity::High
		);
		return;
	}

	m_ShaderProgram = glCreateProgram();

	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);

	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	success = 0;
	if (!success)
	{
		glGetShaderInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		Debug::Log(std::format("Failed to link program : {}", infoLog),
			DebugSource::Application,
			DebugType::Error,
			DebugSeverity::High
		);
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void Shader::Use()
{
	glUseProgram(m_ShaderProgram);
}