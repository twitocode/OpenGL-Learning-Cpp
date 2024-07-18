#pragma once
#include "Renderer.h"
#include <GL/glew.h>
#include <vector>

// Could be position, colour, etc
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 4;
		}

		ASSERT(false);
		return 0;
	}
};

/// <summary>
/// Used to store the layout of a vertex buffer
/// A layout could be (1. positions, 2. colours, 3. uv, 4. textures)
/// </summary>
class VertexBufferLayout
{
  private:
	std::vector<VertexBufferElement> _elements;

	/// <summary>
	/// In bytes
	/// </summary>
	unsigned int _stride;

  public:
	VertexBufferLayout() : _stride(0)
	{
	}
	~VertexBufferLayout()
	{
	}

	// If it is anything other than the ones specified then it is going to fail
	template <typename T> void Push(unsigned int count)
	{
		static_assert(false);
	}

	//
	template <> void Push<float>(unsigned int count)
	{
		// Adds a new element to the elements vector.
		_elements.push_back({GL_FLOAT, count, GL_FALSE});

		// The stride will be used to get the total size of the attribute pointer array.
		_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template <> void Push<unsigned int>(unsigned int count)
	{
		_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
		_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template <> void Push<unsigned char>(unsigned int count)
	{
		_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
		_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	/// <summary>
	/// Used to store each element in the attribute layout
	/// elements could be positions, colours, uv, textures
	/// </summary>
	inline const std::vector<VertexBufferElement>& GetElements() const
	{
		return _elements;
	}

	/// <summary>
	/// Used to get the size of the attribute pointer array in bytes
	/// </summary>
	inline const unsigned int GetStride() const
	{
		return _stride;
	}
};