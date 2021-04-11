#pragma once

#include <cassert>

#include <glad/glad.h>

class Texture
{
public:
	Texture():m_handler(0){}

	Texture(const Texture&) = delete;

	Texture(Texture&& texture) noexcept
		:m_handler(texture.m_handler)
	{
		texture.m_handler = 0;
	}

	Texture& operator=(Texture&) = delete;

	Texture& operator=(Texture&& texture) noexcept
	{
		destroy();
		m_handler = texture.m_handler;
		texture.m_handler = 0;
		return *this;
	}

	~Texture()
	{
		destroy();
	}

	void destroy()
	{
		if (m_handler)
		{
			glDeleteTextures(1, &m_handler);
			m_handler = 0;
		}
	}

	void setTexParameteri(GLenum paramName, const GLint param) const
	{
		assert(m_handler);
		glTextureParameteri(m_handler, paramName, param);
	}

	void setTexFilterParameter(const GLint minFilter, const GLint magFilter) const
	{
		assert(m_handler);
		glTextureParameteri(m_handler, GL_TEXTURE_MIN_FILTER, minFilter);
		glTextureParameteri(m_handler, GL_TEXTURE_MAG_FILTER, magFilter);
	}

	void setTexFormat(GLenum internalFormat, GLsizei width, GLsizei height) const
	{
		assert(m_handler);
		glTextureStorage2D(m_handler, 1, internalFormat, width, height);
	}

	void genTexMipMap() const
	{
		assert(m_handler);
		glGenerateTextureMipmap(m_handler);
	}

	[[nodiscard]] GLuint getHandler() const
	{
		return m_handler;
	}

	void bindTexUnit(GLuint unit = 0) const
	{
		assert(m_handler);
		glBindTextureUnit(0, m_handler);
	}

protected:
	GLuint m_handler;
};