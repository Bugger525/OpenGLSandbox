#include "App.h"

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