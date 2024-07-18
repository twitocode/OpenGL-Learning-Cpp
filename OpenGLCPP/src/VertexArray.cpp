#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include <vector>

VertexArray::VertexArray()
{
	// VBOs get bound to the VAO when they specify their attribute pointers
	GLCall(glGenVertexArrays(1, &_RendererID));
}

VertexArray::~VertexArray()
{
	// Deletes all data in the VAO as well as the VAO itself
	GLCall(glDeleteVertexArrays(1, &_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	// Binds the current VAO
	Bind();

	// binds the vertex buffer given to be manipulated
	vb.Bind();

	// Stores each element in the buffer
	// This will most likely have 1 element.
	std::vector<VertexBufferElement> elements = layout.GetElements();

	// This the offset for the current vertex element in the loop
	// Position = 0, colour = 3, uvData = 7
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		VertexBufferElement element = elements[i];

		// Adds the vertex buffer element to the buffer
		// The stride is the size of all of a vertex's data in bytes. (position, colour), (position, colour). Each is 6
		// * 4 bytes (position = 3 floats * sizeof(float))
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(),
									 (const void*)offset));

		// This enables the attribute
		GLCall(glEnableVertexAttribArray(i));

		offset += element.count * sizeof(float);
	}

	vb.UnBind();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}