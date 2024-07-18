#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	: _RendererID(0), _filePath(filePath), _localBuffer(nullptr), _width(0), _height(0), _bitsPerPixel(0)
{
	// image needs to be flipped because images in opengl are loaded from bottom left not top left
	stbi_set_flip_vertically_on_load(true);
	_localBuffer = stbi_load(filePath.c_str(), &_width, &_height, &_bitsPerPixel, STBI_rgb_alpha);

	if (_localBuffer == nullptr)
	{
		std::cout << stbi_failure_reason() << std::endl;
		ASSERT(false);
	}

	GLCall(glGenTextures(1, &_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, _RendererID));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer));

	// configure sampler
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	// GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colors));
	//  Pixels can be void pointer if you are not providing any data, will still allocate space on gpu

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(_localBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, _RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}