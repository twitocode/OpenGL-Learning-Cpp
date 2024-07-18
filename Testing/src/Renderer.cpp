#include "Renderer.h"
#include <string>

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