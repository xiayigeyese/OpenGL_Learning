#pragma once

#include <cassert>
#include <iostream>
#include <glad/glad.h>

#include "texture2d.h"
#include "renderbuffer.h"

class FrameBuffer
{
public:
	FrameBuffer() :m_handler(0) 
	{
		glCreateFramebuffers(1, &m_handler);
	}

	FrameBuffer(const FrameBuffer&) = delete;

	FrameBuffer(FrameBuffer&& framebuffer) noexcept
		:m_handler(framebuffer.m_handler)
	{
		framebuffer.m_handler = 0;
	}

	FrameBuffer& operator=(const FrameBuffer&) = delete;

	FrameBuffer& operator=(FrameBuffer&& framebuffer) noexcept
	{
		destory();
		m_handler = framebuffer.m_handler;
		framebuffer.m_handler = 0;
		return *this;
	}

	~FrameBuffer()
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
		assert(isComplete());
		glBindFramebuffer(GL_FRAMEBUFFER, m_handler);
	}

	void unBind() 
	{
		assert(m_handler);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void attachTexture2D(GLenum attachment, Texture2D& texture)
	{
		assert(m_handler);
		glNamedFramebufferTexture(m_handler, attachment, texture.getHandler(), 0);
	}

	void attachRenderBuffer(GLenum attachment, Renderbuffer& renderbuffer)
	{
		assert(m_handler);
		glNamedFramebufferRenderbuffer(m_handler, attachment, GL_RENDERBUFFER, renderbuffer.getHandler());
	}

	bool isComplete()
	{
		assert(m_handler);
		if (glCheckNamedFramebufferStatus(m_handler, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "error framebuffer is not complete!" << std::endl;
			return false;
		}
		return true;
	}

	GLuint getHandler() const
	{
		return m_handler;
	}


private:
	GLuint m_handler;
};