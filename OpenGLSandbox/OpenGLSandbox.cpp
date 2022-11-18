#include "App.h"
#include "Input.h"

class MyApp : public App
{
protected:
	bool Initialize() override
	{
		return true;
	}
	void Render() override
	{

	}
	void Update() override
	{

	}
	void Cleanup() override
	{
	}
};

int main()
{
	MyApp().Run();
}