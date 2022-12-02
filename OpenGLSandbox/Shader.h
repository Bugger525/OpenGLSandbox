#pragma once

#include <string>

class Shader
{
public:
	Shader(std::string_view vertexShaderPath, std::string_view fragmentShaderPath);

	void Use();
private:
	unsigned int m_ShaderProgram = NULL;
};