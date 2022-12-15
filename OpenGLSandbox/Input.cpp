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

	auto it = m_Keys.find(code);
	if (it != m_Keys.end())
	{
		auto& state = m_Keys.at(code);
		return state.Current && state.Previous;
	}
	else
		return false;
}
bool Input::IsKeyPressed(Keys key) const
{
	int code = static_cast<int>(key);

	auto it = m_Keys.find(code);
	if (it != m_Keys.end())
	{
		auto& state = m_Keys.at(code);
		return state.Current && !state.Previous;
	}
	else
		return false;
}
bool Input::IsKeyReleased(Keys key) const
{
	int code = static_cast<int>(key);

	auto it = m_Keys.find(code);
	if (it != m_Keys.end())
	{
		auto& state = m_Keys.at(code);
		return !state.Current && state.Previous;
	}
	else
		return false;
}
void Input::KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	auto& self = *static_cast<Input*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_REPEAT)
	{
		self.m_Keys[key].Current = true;
		self.m_Keys[key].Previous = true;
		return;
	}

	self.m_Keys[key].Previous = self.m_Keys[key].Current;
	self.m_Keys[key].Current = action != GLFW_RELEASE;
}