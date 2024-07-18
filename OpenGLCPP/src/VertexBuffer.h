#pragma once

/// <summary>
/// This class is used for storing Vertex buffer data in OpenGL
/// </summary>
class VertexBuffer
{
  private:
	// This id buffer is used to identify the buffer we just created, could be a shader, texture, etc
	unsigned int _RendererID;

  public:
	// Void means that we can put anything
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};