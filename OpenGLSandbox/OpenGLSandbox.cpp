#include "App.h"
#include "Debug.h"

class MyApp : public App
{
protected:
	bool Initialize() override
	{
		Debug::Critical("TEST");
		Debug::Error("TEST");
		Debug::Warn("TEST");
		Debug::Log("TEST");
		return true;
	}
	void Render() override
	{
	}
	void Update() override
	{
		if (Input->IsKeyDown(Keys::Escape))
		{
			Close();
		}
	}
	void Cleanup() override
	{

	}
};

int main(int argc, char** argv)
{
	MyApp().Run();
}