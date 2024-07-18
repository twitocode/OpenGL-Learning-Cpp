#pragma once
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
  private:
	unsigned int _RendererID;
	std::string _vertexFilePath;
	std::string _fragFilePath;
	// Caching uniforms
	std::unordered_map<std::string, int> _uniformLocationCache;

  public:
	Shader(const std::string& vertexFilePath, const std::string& fragFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform2f(const std::string& name, float v0, float v1);

	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);

  private:
	unsigned int GetUniformLocatiom(const std::string& location);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragShader);
};