#pragma once

class IndexBuffer
{
  private:
	unsigned int _RendererID;

	// Count and size are not the same
	// Count = 6 elements
	// Size = 6 elements * 4 bytes per int = 24 bytes
	unsigned int _Count;

  public:
	IndexBuffer(unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	// Inline literally puts the code in here to where it was called
	inline unsigned int GetCount() const
	{
		return _Count;
	}
};