/*
MAKE SURE YOU DEFINE THIS PROPERLY
EITHER IN VISUAL STUDIO PREPROCESSOR OR HEADER FILES
THIS GAVE A MASSIVE HEADACHE

#define GLEW_STATIC
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "vendor/stb_image/stb_image.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		const char* description;
		int code = glfwGetError(&description);

		if (description)
			std::cout << code << "|" << description << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profiles need a vao to work, compat already has one
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		const char* description;
		int code = glfwGetError(&description);

		if (description)
			std::cout << code << "|" << description << std::endl;
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glViewport(0, 0, 800, 800);
	// need to be ran after a valid context has been made ^
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew is not okay" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float vertexData[] = {
		//  X  Y      U     V
		1.0f,  1.0f,  1.0f, 1.0f, // vertex 0
		-1.0f, 1.0f,  0.0f, 1.0f, // vertex 1
		1.0f,  -1.0f, 1.0f, 0.0f, // vertex 2
		-1.0f, -1.0f, 0.0f, 0.0f, // vertex 3
	};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		2, 1, 3, // second triangle
	};

	glEnable(GL_BLEND);

	unsigned int va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	unsigned int vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GL_FLOAT), vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (char*)(2 * sizeof(GL_FLOAT)));

	unsigned int ib;
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	Shader shader("shaders/shader.vert", "shaders/shader.frag");
	shader.Bind();
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	int width, height, bpp;
	unsigned char* pixelBuffer;

	stbi_set_flip_vertically_on_load(true);
	pixelBuffer = stbi_load("C:/Users/Rocket/Desktop/Files/Code/Learning/OpenGLCPP/Testing/res/image.png", &width,
							&height, &bpp, 0);

	if (pixelBuffer == nullptr)
	{
		printf("Not proper texture loaded");
	}

	unsigned int texture;
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader.SetUniform1i("u_Texture", 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	shader.Unbind();

	double lasttime = glfwGetTime();

	glClearColor(1.0, 1.0, 1.0, 1.0);
	while (!glfwWindowShouldClose(window))
	{
		// Limits framerate to 60
		while (glfwGetTime() < lasttime + 1.0 / 60)
		{
			// TODO: Put the thread to sleep, yield, or simply do nothing
		}
		lasttime += 1.0 / 60;

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();
		shader.SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 0.0f);

		// texture bind ------------------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		shader.SetUniform1i("u_Texture", 0);

		glBindVertexArray(va);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

		glDrawElements(GL_TRIANGLES, 6 * sizeof(float), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}