////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/GL/GL_SO_LOADER.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/C.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <FRONTIER/GL/GL_CHECK.hpp>
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
			glBindFramebuffer(GL_FRAMEBUFFER,m_newId);
	}
	~ObjectBinder()
	{
		if (m_newId != m_oldId)
			glBindFramebuffer(GL_FRAMEBUFFER,m_oldId);
	}
	static int getBoundId()
	{
		int ret;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING,&ret);
		return ret;
	}
};

fm::Result checkFramebufferStatus(const std::string &func,const std::string &file,fm::Size line)
{
	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
		return fm::Result("GLError",fm::Result::OPFailed,"FrameBufferError",func,file,line,"wrong size");
	
	if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
		return fm::Result("GLError",fm::Result::OPFailed,"FrameBufferError",func,file,line,"no color attachment");
	
	if (status == GL_FRAMEBUFFER_UNSUPPORTED)
		return fm::Result("GLError",fm::Result::OPFailed,"FrameBufferError",func,file,line,"FBO not supported by GL");
	
	return fm::Result();
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
								 m_height(0),
								 m_depthTestMode(Unused)
	{

	}

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer(const Texture **colorAttachments,fm::Size count,const DepthBuffer &depthBuf) : m_depthBufID(0),
																											m_width(0),
																										    m_height(0),
																											m_depthTestMode(Unused)
	{
		create(colorAttachments,count,depthBuf);
	}

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer(const Texture &colorAttachment,const DepthBuffer &depthBuf) : m_depthBufID(0),
																						   m_width(0),
																						   m_height(0),
																						   m_depthTestMode(Unused)
	{
		const Texture *ptr = &colorAttachment;
		
		create(&ptr,1,depthBuf);
	}

	////////////////////////////////////////////////////////////
	FrameBuffer::~FrameBuffer()
	{
		if (m_depthBufID && glIsRenderbuffer(m_depthBufID) == GL_TRUE)
			glDeleteRenderbuffers(1,&m_depthBufID);
		
		if (getGlId() && glIsFramebuffer(getGlId()) == GL_TRUE)
			glDeleteFramebuffers(1,&getGlId());
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::create(const Texture **colorAttachments,fm::Size count,const DepthBuffer &depthBuf)
	{
		fm::Result err;
		
		if ((err = setColorAttachments(colorAttachments,count))) return err;
		
		return setDepthBuffer(depthBuf);
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::create(const Texture &colorAttachment,const DepthBuffer &depthBuf)
	{
		const Texture *ptr = &colorAttachment;
		
		return create(&ptr,1,depthBuf);
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::setColorAttachments(const Texture **colorAttachments,fm::Size count)
	{
		init();

		if (!colorAttachments || !count)
			return fm::Result();
		
		fm::Result err;
		
		ObjectBinder binder(getGlId());
		C(count)
		{
			const Texture *ptr = *(colorAttachments+i);
			
			if ((err = glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, ptr->getTexTarget(), ptr->getGlId(), 0))))
				return err;

			fm::vec2s size = ptr->getRealSize();

			if (!i || size.w < m_width ) m_width  = size.w;
			if (!i || size.h < m_height) m_height = size.h;
		}
		
		if (::priv::so_loader.getProcAddr("glDrawBuffers") != nullptr)
		{
			GLenum *drawBuffers = new GLenum[count];
			C(count)
				drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
			err += glCheck(glDrawBuffers(count, drawBuffers));
			delete[] drawBuffers;
			
			if (err) return err;
		}
		
		
		return checkFramebufferStatus("FrameBuffer.setColorAttachments",__FILE__,__LINE__);
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::setDepthBuffer(const DepthBuffer &depthBuf)
	{
		init();
		
		fm::Result err;

		if (depthBuf.dtex)
		{
			ObjectBinder binder(getGlId());
			err += glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBuf.dtex->getTexTarget(), depthBuf.dtex->getGlId(), 0));
		}
		else if (depthBuf.width && depthBuf.height)
		{
			ObjectBinder binder(getGlId());
			if (glIsRenderbuffer(m_depthBufID)==GL_FALSE)
				err += glCheck(glGenRenderbuffers(1, &m_depthBufID));
			err += glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufID));
			err += glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, depthBuf.width, depthBuf.height));
			err += glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufID));
		}
		
		if (err) return err;
		
		return checkFramebufferStatus("FrameBuffer.setDepthBuffer",__FILE__,__LINE__);
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::init()
	{
		if (glIsFramebuffer(getGlId()) == GL_FALSE)
			glGenFramebuffers(1, &getGlId());
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::bind() const
	{
		return FrameBuffer::bind(this);
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::bind(const FrameBuffer *fbo)
	{
		fm::Result err = glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fbo ? fbo->getGlId() : 0));
		
		if (!err && fbo)
		{
			glViewport(0,0,fbo->m_width,fbo->m_height);
		}
		
		return err;
	}

	////////////////////////////////////////////////////////////
	bool FrameBuffer::isAvailable()
	{
		return ::priv::so_loader.getProcAddr("glGenFramebuffers") != nullptr;
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::bind(const FrameBuffer &fbo)
	{
		return bind(&fbo);
	}

	/////////////////////////////////////////////////////////////
	void FrameBuffer::clear(bool colorBuffer,bool depthBuffer,bool stencilBuffer)
	{
		if (colorBuffer || depthBuffer || stencilBuffer)
			glClear((colorBuffer ? GL_COLOR_BUFFER_BIT : 0)|(depthBuffer ? GL_DEPTH_BUFFER_BIT : 0)|(stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0));
	}

	/////////////////////////////////////////////////////////////
	void FrameBuffer::clear(bool colorBuffer)
	{
		clear(colorBuffer,m_depthTestMode != Unused,false);
	}

	/////////////////////////////////////////////////////////////
	void FrameBuffer::setDepthTest(DepthTestMode mode)
	{
		bind();
		
		if (mode != Unused)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		if (mode == Less)    glDepthFunc(GL_LESS);
		if (mode == LEqual)  glDepthFunc(GL_LEQUAL);
		if (mode == GEqual)  glDepthFunc(GL_GEQUAL);
		if (mode == Greater) glDepthFunc(GL_GREATER);
		if (mode == Always)  glDepthFunc(GL_ALWAYS);
		
		m_depthTestMode = mode;
	}

	////////////////////////////////////////////////////////////
	void FrameBuffer::setViewport(const fm::rect2s &viewport)
	{
		glViewport(viewport.pos.x,viewport.pos.y,viewport.size.w,viewport.size.h);
	}

	////////////////////////////////////////////////////////////
	fm::rect2s FrameBuffer::getViewport()
	{
		GLint vp[4]; 
		glGetIntegerv(GL_VIEWPORT, vp);
		
		return fm::rect2s(fm::vec2s(vp[0],vp[1]),fm::vec2s(vp[2],vp[3]));
	}

	////////////////////////////////////////////////////////////
	const fm::vec2s &FrameBuffer::getSize() const
	{
		return *((const fm::vec2s*)&m_width);
	}
}
