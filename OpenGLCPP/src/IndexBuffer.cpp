#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count) : _Count(count)
{
	// Depending on your platform, these types may be a different size (should never happen tho)
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererID));
}

void IndexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}