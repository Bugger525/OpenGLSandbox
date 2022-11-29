#include "App.h"
#include "Input.h"

class MyApp : public App
{
protected:
	Input* m_input = new Input(this);
	bool Initialize() override
	{
		return true;
	}
	void Render() override
	{

	}
	void Update() override
	{
		if (m_input->GetKeyDown(Keys::Escape))
		{
			Close();
		}
	}
	void Cleanup() override
	{
		//delete m_input;
	}
};

int main()
{
	MyApp().Run();
}