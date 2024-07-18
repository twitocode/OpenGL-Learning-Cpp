#pragma once
#include "Renderer.h"
#include <string>

/// <summary>
/// Handles informaton about a texture
/// </summary>
class Texture
{
  private:
	unsigned int _RendererID;
	std::string _filePath;
	unsigned char* _localBuffer;

	int _width, _height, _bitsPerPixel;

  public:
	Texture(const std::string& filePath);
	~Texture();

	/// <summary>
	///
	/// </summary>
	/// <param name="slot -">There are usually 32 texture slots on PC (32 textures loaded at once)</param>
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int GetWidth() const
	{
		return _width;
	}

	int GetHeight() const
	{
		return _height;
	}
};