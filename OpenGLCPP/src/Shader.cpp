#include "Shader.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const std::string& vertexFilePath, const std::string& fragFilePath)
	: _vertexFilePath(vertexFilePath), _fragFilePath(fragFilePath)
{
	// When debugging, it automatically uses the project directory
	ShaderProgramSource source = ParseShader(vertexFilePath, fragFilePath);
	_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragShader)
{
	// Shaders are programs that run on the gGPU
	GLCall(unsigned int program = glCreateProgram());
	GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
	GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader));

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// we dont need the shaders anymore, we have the program now
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();

	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		// Probably a better way to do this
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Error compling " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

ShaderProgramSource Shader::ParseShader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	std::ifstream vertexStream(vertexFilePath);
	std::ifstream fragmentStream(fragmentFilePath);

	std::stringstream vertStrStream;
	std::stringstream fragStrStream;

	std::string line;

	while (getline(vertexStream, line))
	{
		vertStrStream << line << "\n";
	}

	while (getline(fragmentStream, line))
	{
		fragStrStream << line << "\n";
	}

	return {vertStrStream.str(), fragStrStream.str()};
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocatiom(name), v0, v1, v2, v3));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocatiom(name), v0, v1));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocatiom(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocatiom(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
	// Transpose = col by default with glm. Basically traverse it by column or by row
	// value is the pointer to the first array in the matrix
	GLCall(glUniformMatrix4fv(GetUniformLocatiom(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::GetUniformLocatiom(const std::string& name)
{
	if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
	{
		return _uniformLocationCache[name];
	}

	// Calling this function is pretty expensive
	GLCall(int location = glGetUniformLocation(_RendererID, name.c_str()));

	// We have a uniform but not using it
	if (location == -1)
		std::cout << "Warning uniform " << name << " is defined but not used in a shader" << std::endl;
	else
		_uniformLocationCache[name] = location;

	return location;
}