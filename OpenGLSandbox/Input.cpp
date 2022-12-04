#include "Input.h"

#include <GLFW/glfw3.h>

GLFWwindow* Data = nullptr;

void Input::SetWindow(GLFWwindow* window)
{
	Data = window;
}
bool Input::IsKeyDown(Keys key)
{
	return (glfwGetKey(Data, static_cast<int>(key)) == GLFW_PRESS);
}
bool Input::IsKeyUp(Keys key)
{
	return (glfwGetKey(Data, static_cast<int>(key)) == GLFW_RELEASE);
}