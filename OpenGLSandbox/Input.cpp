#include "Input.h"

#include <GLFW/glfw3.h>

Input::Input(GLFWwindow* window)
{
	m_Data = window;
}
bool Input::IsKeyDown(Keys key)
{
	return (glfwGetKey(m_Data, static_cast<int>(key)) == GLFW_PRESS);
}
bool Input::IsKeyUp(Keys key)
{
	return (glfwGetKey(m_Data, static_cast<int>(key)) == GLFW_RELEASE);
}