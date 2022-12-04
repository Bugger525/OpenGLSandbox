#pragma once

#include <string>
#include <utility>

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
	void SetWindowTitle(std::string_view title);

	void Close();

	virtual bool Initialize() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Cleanup() = 0;
private:
	bool InitializeCore();
	void CleanupCore();

	std::pair<int, int> m_Size = { 800, 600 };
	std::string m_Title = "App";

	std::pair<int, int> m_GLVersion = { 4, 5 };
	GLFWwindow* m_Data = nullptr;
};