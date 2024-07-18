#include "VertexBuffer.h"
#include "Renderer.h"

// Data can be anything. void means any,
// the size is the size in bytes of the data being stored
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	// Makes an OpenGL buffer
	GLCall(glGenBuffers(1, &_RendererID));

	// Binds the buffer to an array for data
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _RendererID));

	// Puts the data into the currently bound vao
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

// Deletes the buffer when this object is out of scope
VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _RendererID));
}

void VertexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}