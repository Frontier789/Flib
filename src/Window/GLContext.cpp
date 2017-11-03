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
#include <FRONTIER/Window/GLContext.hpp>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/Window/Window.hpp>
#include <FRONTIER/System/util/OS.h>
#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/OpenGL.hpp>

#ifndef FRONTIER_NO_CONTEXT

#ifdef FRONTIER_OS_WINDOWS
	#include <FRONTIER/Window/Wapi/WapiGLContext.hpp>
	#include <FRONTIER/Window/Wapi/WapiWindow.hpp>
	#include "Wapi/WapiGLContext.cpp"
#elif defined(FRONTIER_OS_LINUX) && !defined(FRONTIER_OS_ANDROID)
	#include <FRONTIER/Window/Xlib/XlibGLContext.hpp>
	#include <FRONTIER/Window/Xlib/XlibWindow.hpp>
	#include "Xlib/XlibGLContext.cpp"
#endif

// On Linux 'Always' is a macro and conflicts with fg::Always
#ifdef Always
	#undef Always
#endif

namespace fw
{
	namespace priv
	{
		class ContextHolder
		{
		public:
			fw::GLContext *cont;
			ContextHolder() : cont(0)
			{

			}
			~ContextHolder()
			{
				if (cont)
					delete cont;
			}
		};

		ContextHolder ch;

		#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		std::mutex theSharedContextMutex;
		#endif

		fw::GLContext &getSharedContext()
		{
			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			theSharedContextMutex.lock();
			#endif

			if (!ch.cont)
				ch.cont = new fw::GLContext,
				ch.cont->getOSContext().create(NULL,128,128,fw::GLContext::Settings());

			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			theSharedContextMutex.unlock();
			#endif

			return *ch.cont;
		}
	}

	/////////////////////////////////////////////////////////////
	GLContext::Settings::Settings(unsigned char majorVersion,
								  unsigned char minorVersion,
								  bool compatiblityProfile) : majorVersion(majorVersion),
															  minorVersion(minorVersion),
															  compatiblityProfile(compatiblityProfile)
	{

	}

	/////////////////////////////////////////////////////////////
	void GLContext::Settings::decreaseVersion()
	{
		// set to 0.0 if 1.0
		if (majorVersion == 1 && minorVersion == 0)
		{
			majorVersion = 0,
			minorVersion = 0;
			return;
		}

		// decrease as usual
		if (minorVersion == 0)
			majorVersion--,
			minorVersion = 9;
		else
			minorVersion--;

		// round down the minor part
		if (majorVersion == 1 && minorVersion > 5) minorVersion = 5;
		if (majorVersion == 2 && minorVersion > 1) minorVersion = 1;
		if (majorVersion == 3 && minorVersion > 3) minorVersion = 3;
		if (majorVersion == 4 && minorVersion > 5) minorVersion = 5;
	}

	/////////////////////////////////////////////////////////////
	GLContext::GLContext() : m_context(new priv::GLContext),
							 m_depthTestMode(fg::Unused),
							 m_clearDepth(0)
	{

	}

	/////////////////////////////////////////////////////////////
	GLContext::GLContext(priv::WindowHandle windowHandle,fw::GLContext::Settings settings) : m_context(new priv::GLContext),
																							 m_depthTestMode(fg::Unused),
																							 m_clearDepth(0)
	{
		create(windowHandle,settings);
	}

	/////////////////////////////////////////////////////////////
	GLContext::GLContext(const fm::vec2s &size,fw::GLContext::Settings settings) : m_context(new priv::GLContext),
																				   m_depthTestMode(fg::Unused),
																				   m_clearDepth(0)
	{
		create(size,settings);
	}

	/////////////////////////////////////////////////////////////
	GLContext::~GLContext()
	{
		delete m_context;
	}

	/////////////////////////////////////////////////////////////
	fm::Result GLContext::setActive(bool active)
	{
		fm::Result res = m_context->setActive(active);
		
		if (res && active)
		{
			unsigned int w = 1,h = 1;
			res += m_context->getSize(w,h);
			
			glViewport(0,0,w,h);
		}
		
		return res;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result GLContext::bindDefaultFrameBuffer()
	{
		fm::Result res = glCheck(glBindFramebuffer(GL_FRAMEBUFFER,0));
		
		if (res)
		{
			unsigned int w = 1,h = 1;
			res += m_context->getSize(w,h);
			
			glViewport(0,0,w,h);
		}
		
		return res;
	}

	/////////////////////////////////////////////////////////////
	fm::Result GLContext::swapBuffers()
	{
		return m_context->swapBuffers();
	}

	/////////////////////////////////////////////////////////////
	fm::Result GLContext::create(priv::WindowHandle windowHandle,fw::GLContext::Settings settings)
	{
		return m_context->create((priv::Window::Handle)windowHandle,
								 (priv::GLContext::Handle)priv::getSharedContext().getHandle(),
								  settings);
	}

	/////////////////////////////////////////////////////////////
	fm::Result GLContext::create(const fm::vec2s &size,fw::GLContext::Settings settings)
	{
		return m_context->create((priv::GLContext::Handle)priv::getSharedContext().getHandle(),
								  size.w,size.h,
								  settings);
	}

	/////////////////////////////////////////////////////////////
	fm::Result GLContext::create(fw::GLContext::Settings settings)
	{
		return create(fm::vec2s(1,1),settings);
	}

	/////////////////////////////////////////////////////////////
	fm::Result GLContext::destroy()
	{
		return m_context->destroy();
	}

	/////////////////////////////////////////////////////////////
	bool GLContext::hasThreadGL()
	{
		return priv::GLContext::hasThreadGL();
	}

	/////////////////////////////////////////////////////////////
	priv::GLContext &GLContext::getOSContext()
	{
		return *m_context;
	}

	/////////////////////////////////////////////////////////////
	const priv::GLContext &GLContext::getOSContext() const
	{
		return *m_context;
	}

	/////////////////////////////////////////////////////////////
	GLContext::Handle GLContext::getHandle() const
	{
		return m_context->getHandle();
	}

	/////////////////////////////////////////////////////////////
	GLContext::operator GLContext::Handle() const
	{
		return m_context->getHandle();
	}

	/////////////////////////////////////////////////////////////
	const fw::GLContext::Settings &GLContext::getSettings() const
	{
		return m_context->getSettings();
	}
	
	/////////////////////////////////////////////////////////////
	void GLContext::setClearColor(const fm::vec4 &color)
	{
		m_clearColor = color;
	}

	/////////////////////////////////////////////////////////////
	void GLContext::setClearDepth(float depth)
	{
		m_clearDepth = depth;
	}

	/////////////////////////////////////////////////////////////
	void GLContext::clear(bool colorBuffer,bool depthBuffer,bool stencilBuffer)
	{
		glClearDepth(m_clearDepth);
		glClearColor(m_clearColor.r,m_clearColor.g,m_clearColor.b,m_clearColor.a);
		
		if (colorBuffer || depthBuffer || stencilBuffer)
		{
			bindDefaultFrameBuffer();
			glClear((colorBuffer ? GL_COLOR_BUFFER_BIT : 0)|(depthBuffer ? GL_DEPTH_BUFFER_BIT : 0)|(stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0));
		}
	}

	/////////////////////////////////////////////////////////////
	void GLContext::clear(bool colorBuffer)
	{
		clear(colorBuffer,m_depthTestMode != fg::Unused,false);
	}

	/////////////////////////////////////////////////////////////
	void GLContext::setDepthTest(fg::DepthTestMode mode)
	{
		if (mode != fg::Unused)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		if (mode == fg::Less)    glDepthFunc(GL_LESS);
		if (mode == fg::LEqual)  glDepthFunc(GL_LEQUAL);
		if (mode == fg::GEqual)  glDepthFunc(GL_GEQUAL);
		if (mode == fg::Greater) glDepthFunc(GL_GREATER);
		if (mode == fg::Always)  glDepthFunc(GL_ALWAYS);
		
		m_depthTestMode = mode;
	}

	/////////////////////////////////////////////////////////////
	fg::Image GLContext::capture(const fm::vec2s &pos,const fm::vec2s &size,bool flip,bool frontBuffer)
	{
		fg::Image ret;
		ret.create(size);
		
		glReadBuffer(frontBuffer ? GL_FRONT : GL_BACK);
		glReadPixels(pos.x,pos.y,size.w,size.h,GL_RGBA,GL_UNSIGNED_BYTE,ret.getPtr());
		
		if (flip)
			ret.flipVertically();
			
		return ret;
	}

	/////////////////////////////////////////////////////////////
	fg::Image GLContext::capture(bool flip,bool frontBuffer)
	{
		unsigned int w = 1,h = 1;
		m_context->getSize(w,h);
		
		fg::Image ret;
		ret.create(fm::vec2u(w,h));
		
		glReadBuffer(frontBuffer ? GL_FRONT : GL_BACK);
		glReadPixels(0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,ret.getPtr());
		
		if (flip)
			ret.flipVertically();
			
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	GLContext &GLContext::swap(GLContext &cont)
	{
		std::swap(m_context,cont.m_context);
		std::swap(m_depthTestMode,cont.m_depthTestMode);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	GLContext &GLContext::operator=(GLContext &&cont)
	{
		return this->swap(cont);
	}
}

#endif // FRONTIER_NO_CONTEXT
