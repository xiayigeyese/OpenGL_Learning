#pragma once

#include <cassert>
#include <glad/glad.h>

class Renderbuffer 
{
public:
	Renderbuffer() :m_handler(0)
	{
		glCreateRenderbuffers(1, &m_handler);
	}

	Renderbuffer(const Renderbuffer&) = delete;
	Renderbuffer(Renderbuffer&& renderbuffer) noexcept
		:m_handler(renderbuffer.m_handler)
	{
		renderbuffer.m_handler = 0;
	}

	Renderbuffer& operator=(const Renderbuffer&) = delete;
	Renderbuffer& operator=(Renderbuffer&& renderbuffer) noexcept
	{
		m_handler = renderbuffer.m_handler;
		renderbuffer.m_handler = 0;
		return *this;
	}

	~Renderbuffer()
	{
		destroy();
	}

	void destroy()
	{
		if (m_handler)
		{
			glDeleteRenderbuffers(1, &m_handler);
			m_handler = 0;
		}
	}

	void reInitHandler()
	{
		destroy();
		glCreateRenderbuffers(1, &m_handler);
	}

	void allocateStorage(GLenum format, unsigned int width, unsigned int height)
	{
		assert(m_handler);
		glNamedRenderbufferStorage(m_handler, format, width, height);
	}

	void bind()
	{
		assert(m_handler);
		glBindFramebuffer(GL_RENDERBUFFER, m_handler);
	}

	void unBind()
	{
		assert(m_handler);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	GLuint getHandler() const 
	{
		return m_handler;
	}

private:
	GLuint m_handler;
};