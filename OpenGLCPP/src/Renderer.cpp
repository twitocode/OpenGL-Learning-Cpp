#include "Renderer.h"
#include <string>

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	// Rebind
	shader.Bind();
	va.Bind();
	ib.Bind();

	// 6 is the number of indices
	// nullptr because the indicies buffer is already bound
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
	GLCall(glClearColor(0.25, 0.25, 1, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

bool GLLogCall(const char* function, const char* file, int line)
{
	std::string a = "";

	while (GLenum error = glGetError())
	{
		std::cout << "Opengl error ( " << function << ", " << file << ", line:" << line << ")\n"
				  << "Error Code:" << error << std::endl;
		return false;
	}

	return true;
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
		;
}