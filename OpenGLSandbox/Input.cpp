#include "Input.h"

#include <GLFW/glfw3.h>

Input::Input(GLFWwindow* window)
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, KeyCallback);
}
bool Input::IsKeyHeld(Keys key) const
{
	int code = static_cast<int>(key);
	return m_Keys.at(code).Current && m_Keys.at(code).Previous;
}
bool Input::IsKeyPressed(Keys key) const
{
	int code = static_cast<int>(key);
	return m_Keys.at(code).Current && !m_Keys.at(code).Previous;
}
bool Input::IsKeyReleased(Keys key) const
{
	int code = static_cast<int>(key);
	return !m_Keys.at(code).Current && m_Keys.at(code).Previous;
}
void Input::KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	auto& self = *static_cast<Input*>(glfwGetWindowUserPointer(window));

	self.m_Keys[key].Previous = self.m_Keys[key].Current;
	self.m_Keys[key].Current = action != GLFW_RELEASE;

	if (action == GLFW_REPEAT)
	{
		self.m_Keys[key].Previous = true;
	}
}