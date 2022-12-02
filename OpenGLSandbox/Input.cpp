#include "Input.h"
#include "App.h"

#include <GLFW/glfw3.h>

Input::Input(App* window)
{
	m_Data = window;
}
Input::~Input()
{
	delete m_Data;
}
bool Input::IsKeyDown(Keys key)
{
	return (glfwGetKey(m_Data->GetData(), static_cast<int>(key)) == GLFW_PRESS);
}
bool Input::IsKeyUp(Keys key)
{
	return (glfwGetKey(m_Data->GetData(), static_cast<int>(key)) == GLFW_RELEASE);
}