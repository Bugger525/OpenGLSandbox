#include "RenderWindow.h"
#include "Debug.h"
#include "Shader.h"
#include <format>
#include <GL/gl3w.h>

int main(int argc, char** argv)
{
	auto renderWindow = RenderWindow(800, 600, "OpenGLSandbox");

	Shader shader("Shaders/shader.vert", "Shaders/shader.frag");

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	GLuint bufferObject = 0;
	glGenBuffers(1, &bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint arrayObject = NULL;
	glGenVertexArrays(1, &arrayObject);
	glBindVertexArray(arrayObject);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (renderWindow.IsOpen())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		glBindVertexArray(arrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		if (renderWindow.Input->IsKeyHeld(Keys::Space))
		{
			Debug::Log("차징");
		}
		if (renderWindow.Input->IsKeyPressed(Keys::Space))
		{
			Debug::Log("평타 발사");
		}
		if (renderWindow.Input->IsKeyReleased(Keys::Space))
		{
			Debug::Log("미사일 발사");
		}

		renderWindow.UpdateFrame();
	}
}