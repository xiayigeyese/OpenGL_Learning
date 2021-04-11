#include <string>
#include <iostream>

#include <stb_image.h>
#include <opengl/texture2d.h>

void Texture2D::loadFromMemory(const GLsizei width, const GLsizei height, const GLenum internalFormat, const GLenum format, const GLenum type, const void* data, bool genMipMap) const
{
	assert(m_handler);
	if (data == nullptr) return;
	setTexFormat(internalFormat, width, height);
	setTexImageData(width, height, format, type, data);
	initialParameters(genMipMap);
	if (genMipMap) genTexMipMap();
}

void Texture2D::loadFromFile(const std::string& filePath, const GLenum internalFormat, const GLenum format, const GLenum type, bool genMipMap) const
{
	assert(m_handler);
	int width, height, nrChannels;
	auto* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	if (data == nullptr)
	{
		std::cout << "Texture2D: load texture data failure" << std::endl;
		return;
	}
	loadFromMemory(width, height, internalFormat, format, type, data);
	stbi_image_free(data);
}
