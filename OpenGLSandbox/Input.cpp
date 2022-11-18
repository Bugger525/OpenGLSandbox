
#include "Input.h"
#include "App.h"

#include <GLFW/glfw3.h>

Input::Input(App* window)
{
	mData = window;
}
bool Input::GetKeyDown(Keys key)
{
	return (glfwGetKey(mData->GetData(), static_cast<int>(key)) == GLFW_PRESS);
}
bool Input::GetKeyUp(Keys key)
{
	return (glfwGetKey(mData->GetData(), static_cast<int>(key)) == GLFW_RELEASE);
}