/*
MAKE SURE YOU DEFINE THIS PROPERLY
EITHER IN VISUAL STUDIO PREPROCESSOR OR HEADER FILES
THIS GAVE A MASSIVE HEADACHE

#define GLEW_STATIC
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
	GLFWwindow* window;
	const int width = 960;
	const int height = 540;
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
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
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

	// need to be ran after a valid context has been made ^
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew is not okay" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	/*
		OpenGL is a state machine. Put state about the current data into the buffer, draw it,
		Then clear the buffer for the next set of data.
	*/

	float vertexData[] = {
		//  X     Y      Z     U     V
		-50.0f, 50.0f, 0.0f, 0.0f, 0.0f,  // Front 1
		-50.0f, -50.0f, 0.0f, 0.0f, 1.0f, // Front 2
		50.0f, -50.0f, 0.0f, 1.0f, 1.0f,  // Front 3
		50.0f, 50.0f, 0.0f, 1.0f, 0.0f,	  // Front 4
		//------------------------------------------
		-50.0f, 50.0f, 1.0f, 0.0f, 0.0f,  // Back 1
		-50.0f, -50.0f, 1.0f, 0.0f, 1.0f, // Back 2
		50.0f, -50.0f, 1.0f, 1.0f, 1.0f,  // Back 3
		50.0f, 50.0f, 1.0f, 1.0f, 0.0f,	  // Back 4
	};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		2, 0, 3	 // second triangle
	};

	// Blending is for dealing with transparent textures.
	// Target buffer (destination) is where we are drawing a transparent image onto
	// Output is the fragment shader output
	/*
		source (take every colour channel, r, g, b, a) and multiply is by the src
		ex. GL_ONE for source and GL_ZERO for dest
		GL_FUNC_ADD (source * 1) + (dest * 0) = source colour

		--------------------------------------

		Transparent pixel (0)
		ex. GL_SRC_ALPHA for source and GL_ONE_MINUS_SRC_ALPHA for dest
		GL_FUNC_ADD (source = 0) + (dest = 1 - 0[source] = 1)

		R = (r_src * 0) + (r_dest * (1 - 0))
		For all channels

		--------------------------------------

		Partially Transparent vec4(1.0, 1.0, 1.0, 0.5)
		Destination buffer is cleared to magenta vec4(1.0, 0.0, 1.0, 1.0)
		Rendering white square on magenta background

		R = (1.0 * 0.5) + (1.0 * (1 - 0.5)) = 1.0
		G = (1.0 * 0.5) + (0.0 * (1 - 0.5)) = 0.5
		B = (1.0 * 0.5) + (1.0 * (1 - 0.5)) = 1.0
		A = (0.5 * 0.5) + (1.0 * (1 - 0.5)) = 0.75

	*/

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	VertexArray va;

	// 4 vertex * 4 floats per vertex * 4 bytes per float
	VertexBuffer vb(vertexData, 5 * 4 * sizeof(float));
	IndexBuffer ib(indices, 6);

	VertexBufferLayout layout;
	// 2 is the size of each vector position (in this case its vec2)
	layout.Push<float>(3);
	// Textures (vector2)
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	/*
	A project matrix converts an objects vertex positions from world to screen space using (orthographic, perspective)
	World coordinates become normalized (-1 to 1)
	*/
	// Use values that use your aspect ratio (4:3, 16:9, etc)
	// Near and far are z values
	//(2.0 * 2):(1.5 * 2) = 4:3
	// Orthographic does not make object farther smaller, perspective does (like irl)
	// glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

	// 1:1 pixel mapping
	glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
	// To move a camera left, move every object right. (cameras dont actually exist)
	/*
		An identity matrix (glm::mat4(1), sometimes called a unit matrix, is a diagonal matrix with all its diagonal
	   elements equal to 1 , and zeroes everywhere else.
	*/
	glm::mat4 view = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));

	Shader shader("shaders/shader.vert", "shaders/shader.frag");

	// Shader must be bound before settings uniforms
	shader.Bind();
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	Texture* texture = new Texture("res/sun.png");
	// Put a slot in here if you want
	texture->Bind();
	// 0 needs to match the slot
	shader.SetUniform1i("u_Texture", 0);

	// Unbound
	va.Unbind();
	vb.UnBind();
	ib.UnBind();
	// shader.Unbind();

	Renderer renderer;

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	double lasttime = glfwGetTime();

	auto translationA = glm::vec3(200, 200, 0);
	auto translationB = glm::vec3(200, 200, 0);

	glm::vec2 mousePos;

	while (!glfwWindowShouldClose(window))
	{
		// Limits framerate to 60
		while (glfwGetTime() < lasttime + 1.0 / 60)
		{
			// TODO: Put the thread to sleep, yield, or simply do nothing
		}
		lasttime += 1.0 / 60;

		/* Render here */

		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug Info");
		ImGui::SliderFloat3("Model Translation A", &translationA.x, 0, 960);
		ImGui::SliderFloat3("Model Translation B", &translationB.x, 0, 960);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();

		double relativeCursorX = 0.0;
		double relativeCursorY = 0.0;
		glfwGetCursorPos(window, &relativeCursorX, &relativeCursorY);
		mousePos.x = relativeCursorX;
		mousePos.y = relativeCursorY;

		// Shader needs to bound before setting uniforms. Should cache the bind call
		shader.Bind();
		shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		shader.SetUniform1f("iTime", (float)glfwGetTime());

		texture->Bind();
		// shader.SetUniform1i("u_Texture", 0);

		{
			// move every model 200 right and 200 up
			glm::mat4 model = glm::translate(glm::mat4(1), translationA);

			// MVP is the regular acronym but opengl specifcally reverses it (PVM)
			glm::mat4 mvp = proj * view * model;
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(va, ib, shader);
		}

		{
			// move every model 200 right and 200 up
			glm::mat4 model = glm::translate(glm::mat4(1), translationB);

			// MVP is the regular acronym but opengl specifcally reverses it (PVM)
			glm::mat4 mvp = proj * view * model;
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(va, ib, shader);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	delete texture;
	return 0;
}