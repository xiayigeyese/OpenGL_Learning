#include <string>
#include <iostream>

#include <stb_image.h>
#include <opengl/texture2d.h>

void Texture2D::loadFromMemory(const GLsizei width, const GLsizei height, const GLenum internalFormat, const GLenum format, const GLenum type, const void* data, bool genMipMap) const
{
	assert(m_handler);
	setTexFormat(internalFormat, width, height);
	setTexImageData(width, height, format, type, data);
	initialParameters(genMipMap);
	if (genMipMap) genTexMipMap();
}

void Texture2D::loadFromFile(const std::string& filePath, bool genMipMap) const
{
	assert(m_handler);
	int width, height, nrChannels;
	auto* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	if (data == nullptr)
	{
		throw std::runtime_error("Texture2D: load texture data failure");
	}
	GLenum internalFormat, format;
	if (nrChannels == 1)
	{
		internalFormat = GL_R8;
		format = GL_RED;
	}
	else if (nrChannels == 3)
	{
		internalFormat = GL_RGB8;
		format = GL_RGB;
	}
	else
	{
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
	}
	
	loadFromMemory(width, height, internalFormat, format, GL_UNSIGNED_BYTE, data, genMipMap);
	stbi_image_free(data);
}
