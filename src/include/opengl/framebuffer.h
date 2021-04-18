#pragma once

#include <cassert>
#include <iostream>
#include <glad/glad.h>

#include "texture2d.h"
#include "renderbuffer.h"

class Framebuffer
{
public:
	Framebuffer() :m_handler(0) 
	{
		glCreateFramebuffers(1, &m_handler);
	}

	Framebuffer(const Framebuffer&) = delete;

	Framebuffer(Framebuffer&& framebuffer) noexcept
		:m_handler(framebuffer.m_handler)
	{
		framebuffer.m_handler = 0;
	}

	Framebuffer& operator=(const Framebuffer&) = delete;

	Framebuffer& operator=(Framebuffer&& framebuffer) noexcept
	{
		destory();
		m_handler = framebuffer.m_handler;
		framebuffer.m_handler = 0;
		return *this;
	}

	~Framebuffer()
	{
		destory();
	}

	void destory() 
	{
		if (m_handler) {
			glDeleteFramebuffers(1, &m_handler);
			m_handler = 0;
		}
	}

	void reInitHandler()
	{
		destory();
		glCreateFramebuffers(1, &m_handler);
	}

	// check complete, and bind framebuffer, then use it
	void bind()
	{
		assert(m_handler);
		glBindFramebuffer(GL_FRAMEBUFFER, m_handler);
	}

	void unBind() 
	{
		assert(m_handler);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void attachTexture2D(const GLenum attachment, const Texture2D& texture)
	{
		assert(m_handler);
		glNamedFramebufferTexture(m_handler, attachment, texture.getHandler(), 0);
	}

	void attachRenderBuffer(const GLenum attachment, const Renderbuffer& renderbuffer) const
	{
		assert(m_handler);
		glNamedFramebufferRenderbuffer(m_handler, attachment, GL_RENDERBUFFER, renderbuffer.getHandler());
	}

	void setColorBufferToDraw(GLenum buffer) const
	{
		assert(m_handler);
		glNamedFramebufferDrawBuffer(m_handler, buffer);
	}

	void setColorBufferToRead(GLenum buffer) const
	{
		assert(m_handler);
		glNamedFramebufferReadBuffer(m_handler, buffer);
	}

	bool isComplete() const
	{
		assert(m_handler);
		if (glCheckNamedFramebufferStatus(m_handler, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "error framebuffer is not complete!" << std::endl;
			return false;
		}
		return true;
	}

	[[nodiscard]] GLuint getHandler() const
	{
		return m_handler;
	}


private:
	GLuint m_handler;
};