#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout;

/// <summary>
/// A container for all currently bound buffer data (positions, colour, etc)
/// </summary>
class VertexArray
{
  private:
	/// <summary>
	/// Used to identify this VAO
	/// </summary>
	unsigned int _RendererID;

  public:
	VertexArray();
	~VertexArray();

	/// <summary>
	/// Binds a buffer to this VAO
	/// </summary>
	/// <param name="vb -">The buffer to bind</param>
	/// <param name="layout -">The layout of the buffer being added</param>
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};