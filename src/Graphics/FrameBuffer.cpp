////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/FrameBuffer.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/Graphics/GLCheck.hpp>
#include <FRONTIER/Graphics/FgLog.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Log.hpp>
#include <FRONTIER/OpenGL.hpp>
class ObjectBinder
{
public:
	int m_oldId;
	int m_newId;
	ObjectBinder(int idToBeBound) : m_oldId(getBoundId()),
									m_newId(idToBeBound)
	{
		if (m_newId != m_oldId)
			glCheck(glBindFramebuffer(GL_FRAMEBUFFER,m_newId));
	}
	~ObjectBinder()
	{
		if (m_newId != m_oldId)
			glCheck(glBindFramebuffer(GL_FRAMEBUFFER,m_oldId));
	}
	static int getBoundId()
	{
		int ret;
		glCheck(glGetIntegerv(GL_FRAMEBUFFER_BINDING,&ret));
		return ret;
	}
};
bool checkFramebufferStatus()
{
	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
	{
		fg::fg_log << "Error: the framebuffer object has a incomplete attachment (invalid size?)" << std::endl;
		return 0;
	}
	if (status == GL_FRAMEBUFFER_UNSUPPORTED)
	{
		fg::fg_log << "Error: FrameBuffers are not supported by implementation of OpenGL." << std::endl;
		return 0;
	}
	return 1;
}
namespace fg
{
	////////////////////////////////////////////////////////////
	FrameBuffer::DepthBuffer::DepthBuffer(const fm::vec2s &size) : width(size.w),
																   height(size.h),
																   dtex(NULL)
	{

	}

	////////////////////////////////////////////////////////////
	FrameBuffer::DepthBuffer::DepthBuffer(const fm::Size &w,const fm::Size &h) : width(w),
																				 height(h),
																				 dtex(NULL)
	{

	}

	////////////////////////////////////////////////////////////
	FrameBuffer::DepthBuffer::DepthBuffer(const DepthTexture &depthTexture) : width(0),
																			  height(0),
																			  dtex(&depthTexture)
	{

	}

	////////////////////////////////////////////////////////////
	const FrameBuffer::DepthBuffer FrameBuffer::DepthBuffer::noDepthBuffer = FrameBuffer::DepthBuffer(0,0);

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer() : m_depthBufID(0),
								 m_width(0),
								 m_height(0)
	{

	}

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer(const Texture *colorAttachments,fm::Size count,const DepthBuffer &depthBuf) : m_depthBufID(0),
																										   m_width(0),
																										   m_height(0)
	{
		create(colorAttachments,count,depthBuf);
	}

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer(const Texture &colorAttachment,const DepthBuffer &depthBuf) : m_depthBufID(0),
																						   m_width(0),
																						   m_height(0)
	{
		create(&colorAttachment,1,depthBuf);
	}

	////////////////////////////////////////////////////////////
	FrameBuffer::~FrameBuffer()
	{
		if (m_depthBufID && glIsRenderbuffer(m_depthBufID) == GL_TRUE)
			glCheck(glDeleteRenderbuffers(1,&m_depthBufID));
		if (getGlId() && glIsFramebuffer(getGlId()) == GL_TRUE)
			glCheck(glDeleteFramebuffers(1,&getGlId()));
	}

	////////////////////////////////////////////////////////////
	bool FrameBuffer::create(const Texture *colorAttachments,fm::Size count,const DepthBuffer &depthBuf)
	{
		return setColorAttachments(colorAttachments,count) && setDepthBuffer(depthBuf);
	}

	////////////////////////////////////////////////////////////
	bool FrameBuffer::create(const Texture &colorAttachment,const DepthBuffer &depthBuf)
	{
		return setColorAttachments(&colorAttachment,1) && setDepthBuffer(depthBuf);
	}

	////////////////////////////////////////////////////////////
	bool FrameBuffer::setColorAttachments(const Texture *colorAttachments,fm::Size count)
	{
		init();

		if (!colorAttachments || !count)
		{
			fg_log << "Error: no attachment is specified in framebuffer constructor" << std::endl;
			return false;
		}

		ObjectBinder binder(getGlId());
		C(count)
		{
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, (colorAttachments+i)->getGlId(), 0));
			
			fm::vec2s size = (colorAttachments+i)->getRealSize();
			
			if (!i || size.w < m_width ) m_width  = size.w;
			if (!i || size.h < m_height) m_height = size.h;
		}

		/*
		GLenum *DrawBuffers;
		DrawBuffers = new GLenum[count];
		C(count)
			*(DrawBuffers+i) = GL_COLOR_ATTACHMENT0+i;
		glCheck(glDrawBuffers(count, DrawBuffers));
		delete[] DrawBuffers;
		*/
		return checkFramebufferStatus();
	}

	bool FrameBuffer::setDepthBuffer(const DepthBuffer &depthBuf)
	{
		if (depthBuf.dtex)
		{
			ObjectBinder binder(getGlId());
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuf.dtex->getGlId(), 0));
		}
		else if (depthBuf.width && depthBuf.height)
		{
			ObjectBinder binder(getGlId());
			if (glIsRenderbuffer(m_depthBufID)==GL_FALSE)
				glCheck(glGenRenderbuffers(1, &m_depthBufID));
			glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufID));
			glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, depthBuf.width, depthBuf.height));
			glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufID));
		}
		return checkFramebufferStatus();
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::init()
	{
		if (glIsFramebuffer(getGlId())==GL_FALSE)
			glCheck(glGenFramebuffers(1, &getGlId()));
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::bind() const
	{
		FrameBuffer::bind(this);
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::bind(const FrameBuffer *fbo)
	{
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fbo ? fbo->getGlId() : 0));
	}

	////////////////////////////////////////////////////////////
	bool FrameBuffer::isAvailable()
	{
		GLint testBound;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING,&testBound);
		return glGetError()==GL_NO_ERROR;
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::bind(const FrameBuffer &fbo)
	{
		bind(&fbo);
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::setViewport(const fm::rect2s &viewport)
	{
		glViewport(viewport.pos.x,viewport.pos.y,viewport.size.w,viewport.size.h);
	}

	////////////////////////////////////////////////////////////
	const fm::vec2s &FrameBuffer::getSize() const
	{
		return *((const fm::vec2s*)&m_width);
	}
}
