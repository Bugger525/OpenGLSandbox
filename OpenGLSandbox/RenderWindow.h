#pragma once

#include <string>
#include "Input.h"

struct GLFWwindow;

class RenderWindow
{
public:
	RenderWindow();
	RenderWindow(int width, int height, std::string_view title);

	void Run();

	int GetWidth() const;
	int GetHeight() const;
	std::string GetTitle() const;
	GLFWwindow* GetData() const;

	void SetWidth(int width);
	void SetHeight(int height);
	void SetSize(int width, int height);
	void SetTitle(std::string_view title);

	void Close();
	bool IsOpen() const;

	void UpdateFrame();

	Input* Input = nullptr;
private:
	int m_Width = 800, m_Height = 600;
	std::string m_Title = "RenderWindow";

	GLFWwindow* m_Data = nullptr;
};