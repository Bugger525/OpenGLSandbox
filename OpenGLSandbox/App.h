#pragma once

#include <string>
#include <utility>
#include "Debug.h"

struct GLFWwindow;

class App
{
public:
	App();

	GLFWwindow* GetData() const;

	void Run();
protected:
	const std::pair<int, int>& GetWindowSize() const;
	std::string GetWindowTitle() const;

	std::string GetGLVersionString() const;
	const std::pair<int, int>& GetGLVersionPair() const;

	void SetWindowSize(int width, int height);
	void SetWindowWidth(int width);
	void SetWindowHeight(int height);
	void SetWindowTitle(const std::string_view& title);

	void Close();

	virtual bool Initialize() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Cleanup() = 0;
private:
	DebugSource InitializeCore();
	void CleanupCore();

	std::pair<int, int> mSize = { 800, 600 };
	std::string mTitle = "App";

	std::pair<int, int> mGLVersion = { 3, 3 };
	GLFWwindow* mData = nullptr;
};