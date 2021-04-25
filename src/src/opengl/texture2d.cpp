#include <string>
#include <iostream>
#include <array>
#include <stb_image.h>
#include <opengl/texture2d.h>

void Texture2D::loadFromMemory(const GLsizei width, const GLsizei height, const GLsizei levels, const GLenum internalFormat, const GLenum format, const GLenum type, const void* data) const
{
	assert(m_handler);
	setTexFormat(levels, internalFormat, width, height);
	setTexImageData(width, height, format, type, data);
	initialParameters(levels > 1);
	if (levels > 1) genTexMipMap();
}

void Texture2D::loadFromFile(const std::string& filePath, unsigned int mipmapLevels) const
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
	
	loadFromMemory(width, height, mipmapLevels, internalFormat, format, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}


Texture2D Texture2D::createShadowMap(const int width, const int height, const GLenum internalFormat)
{
	Texture2D shadow;
	shadow.setTexFormat(1, internalFormat, width, height);
	shadow.setTexImageData(width, height, internalFormat, GL_FLOAT, nullptr);
	shadow.setTexFilterParameter(GL_NEAREST, GL_NEAREST);
	shadow.setTexWrapParameter(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	shadow.setTexBorderColor(
		std::array<float, 4>{
			std::numeric_limits<float>::max(), 
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			1
		}
	);
	return shadow;
}

