
#include "Input.h"
#include "App.h"

#include <GLFW/glfw3.h>

Input::Input(App* window)
{
	m_data = window;
}
Input::~Input()
{
	delete m_data;
}
bool Input::GetKeyDown(Keys key)
{
	return (glfwGetKey(m_data->GetData(), static_cast<int>(key)) == GLFW_PRESS);
}
bool Input::GetKeyUp(Keys key)
{
	return (glfwGetKey(m_data->GetData(), static_cast<int>(key)) == GLFW_RELEASE);
}