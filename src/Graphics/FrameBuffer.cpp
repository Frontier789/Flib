////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/Vector4.hpp>
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
	FRONTIER_API const FrameBuffer::DepthBuffer FrameBuffer::DepthBuffer::noDepthBuffer = FrameBuffer::DepthBuffer(0,0);

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer() : m_depthBufID(0),
								 m_width(0),
								 m_height(0),
								 m_clearDepth(1),
								 m_depthTestMode(Unused)
	{

	}

	/////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer(FrameBuffer &&move) : m_depthBufID(0),
												   m_width(0),
												   m_height(0),
												   m_clearDepth(1),
												   m_depthTestMode(Unused)
	{
		move.swap(*this);
	}

	/////////////////////////////////////////////////////////////
	FrameBuffer &FrameBuffer::operator=(FrameBuffer &&move)
	{
		return this->swap(move);
	}

	/////////////////////////////////////////////////////////////
	FrameBuffer &FrameBuffer::swap(FrameBuffer &fbo)
	{
		std::swap(m_depthBufID   ,fbo.m_depthBufID   );
		std::swap(m_width        ,fbo.m_width        );
		std::swap(m_height       ,fbo.m_height       );
		std::swap(m_depthTestMode,fbo.m_depthTestMode);
		std::swap(getGlId()      ,fbo.getGlId()      );
		
		return *this;
	}

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer(const Texture **colorAttachments,fm::Size count,const DepthBuffer &depthBuf,fg::DepthTestMode depthMode) : 
		m_depthBufID(0),
		m_width(0),
		m_height(0),
		m_clearDepth(1),
		m_depthTestMode(Unused)
	{
		create(colorAttachments,count,depthBuf,depthMode);
	}

	////////////////////////////////////////////////////////////
	FrameBuffer::FrameBuffer(const Texture &colorAttachment,const DepthBuffer &depthBuf,fg::DepthTestMode depthMode) : 
		m_depthBufID(0),
		m_width(0),
		m_height(0),
		m_clearDepth(1),
		m_depthTestMode(Unused)
	{
		const Texture *ptr = &colorAttachment;
		
		create(&ptr,1,depthBuf,depthMode);
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
	fm::Result FrameBuffer::create(const Texture **colorAttachments,fm::Size count,const DepthBuffer &depthBuf,fg::DepthTestMode depthMode)
	{
		fm::Result res = setColorAttachments(colorAttachments,count);
		
		if (!res) return res;
		
		return setDepthBuffer(depthBuf,depthMode);
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::create(const Texture &colorAttachment,const DepthBuffer &depthBuf,fg::DepthTestMode depthMode)
	{
		const Texture *ptr = &colorAttachment;
		
		return create(&ptr,1,depthBuf,depthMode);
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::setColorAttachments(const Texture **colorAttachments,fm::Size count)
	{
		init();

		if (!colorAttachments || !count)
			return fm::Result();
		
		fm::Result res;
		
		bind();
		C(count)
		{
			const Texture *ptr = *(colorAttachments+i);
			
			if (!ptr) continue;
			
			res = glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, ptr->getTexTarget(), ptr->getGlId(), 0));
			
			if (!res) return res;

			fm::vec2s size = ptr->getSize();

			if (!i || size.w < m_width ) m_width  = size.w;
			if (!i || size.h < m_height) m_height = size.h;
		}
		FrameBuffer::bind(nullptr);
		
		GLenum *drawBuffers = new GLenum[count];
		C(count)
			drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,getGlId());
		res += glCheck(glDrawBuffers(count, drawBuffers));
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
		
		delete[] drawBuffers;
		
		if (!res) return res;
		
		return checkFramebufferStatus("FrameBuffer.setColorAttachments",__FILE__,__LINE__);
	}

	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::setDepthBuffer(const DepthBuffer &depthBuf,fg::DepthTestMode depthMode)
	{
		init();
		
		fm::Result res;
		
		if (depthBuf.dtex)
		{
			ObjectBinder binder(getGlId());
			res += glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBuf.dtex->getTexTarget(), depthBuf.dtex->getGlId(), 0));
		}
		else if (depthBuf.width && depthBuf.height)
		{
			ObjectBinder binder(getGlId());
			if (glIsRenderbuffer(m_depthBufID) == GL_FALSE)
				res += glCheck(glGenRenderbuffers(1, &m_depthBufID));
			res += glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufID));
			res += glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, depthBuf.width, depthBuf.height));
			res += glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufID));
		}
		
		m_depthTestMode = depthMode;
		if (m_depthTestMode == fg::AutoDepthTest) {
			m_depthTestMode = ((depthBuf).dtex == nullptr && (depthBuf.width == 0 || depthBuf.height == 0) ? fg::Unused : fg::LEqual);
		}
		
		if (!res) return res;
		
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
	void FrameBuffer::applyDepthTest(fg::DepthTestMode depthMode)
	{
		if (depthMode != Unused)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		if (depthMode == Less)    glDepthFunc(GL_LESS);
		if (depthMode == LEqual)  glDepthFunc(GL_LEQUAL);
		if (depthMode == GEqual)  glDepthFunc(GL_GEQUAL);
		if (depthMode == Greater) glDepthFunc(GL_GREATER);
		if (depthMode == Always)  glDepthFunc(GL_ALWAYS);
	}
	
	/////////////////////////////////////////////////////////////
	DepthTestMode FrameBuffer::getDepthTest() const
	{
		return m_depthTestMode;
	}
	
	////////////////////////////////////////////////////////////
	fm::Result FrameBuffer::bind(const FrameBuffer *fbo)
	{
		fm::Result res = glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fbo ? fbo->getGlId() : 0));
		
		if (res && fbo) {
			glViewport(0,0,fbo->m_width,fbo->m_height);
			applyDepthTest(fbo->getDepthTest());
		}
		
		return res;
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
	void FrameBuffer::setClearColor(fm::vec4 color)
	{
		m_clearColor = color;
	}

	/////////////////////////////////////////////////////////////
	void FrameBuffer::setClearDepth(float depth)
	{
		m_clearDepth = depth;
	}

	/////////////////////////////////////////////////////////////
	void FrameBuffer::clear(bool colorBuffer,bool depthBuffer,bool stencilBuffer)
	{
		glClearDepth(m_clearDepth);
		glClearColor(m_clearColor.r,m_clearColor.g,m_clearColor.b,m_clearColor.a);
		
		if (colorBuffer || depthBuffer || stencilBuffer)
		{
			bind();
			glClear((colorBuffer ? GL_COLOR_BUFFER_BIT : 0)|(depthBuffer ? GL_DEPTH_BUFFER_BIT : 0)|(stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0));
		}
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
		
		m_depthTestMode = mode;
		
		applyDepthTest(m_depthTestMode);
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
