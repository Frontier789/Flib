////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/macros/OS.h>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/Window/Window.hpp>
#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/System/Mutex.hpp>
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
		fm::Mutex theSharedContextMutex;
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
			theSharedContextMutex.unLock();
			#endif

			return *ch.cont;
		}

		/*
		fw::GLContext theSharedContext;
		class theSharedContextInitializer
		{
		public:
			theSharedContextInitializer(fw::GLContext *ptr)
			{
				ptr->getOSContext().create(NULL,100,100,fw::GLContext::Settings());
			}
		};
		theSharedContextInitializer initit(&theSharedContext);*/
	}

	/////////////////////////////////////////////////////////////
	GLContext::Settings::Settings(unsigned char majorVersion,
								  unsigned char minorVersion,
								  bool compatiblityProfile,
								  unsigned char bitsPerPixel,
								  unsigned char depthBits,
								  unsigned char stencilBits) : majorVersion(majorVersion),
															   minorVersion(minorVersion),
															   compatiblityProfile(compatiblityProfile),
															   bitsPerPixel(bitsPerPixel),
															   depthBits(depthBits),
															   stencilBits(stencilBits)
	{

	}

	/////////////////////////////////////////////////////////////
	void GLContext::Settings::decreaseVersion()
	{
		// set to the highest known if 1.0
		if (majorVersion == 1 && minorVersion == 0)
		{
			majorVersion = 4,
			minorVersion = 5;
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
	GLContext::GLContext() : m_context(new priv::GLContext)
	{

	}

	/////////////////////////////////////////////////////////////
	GLContext::GLContext(priv::WindowHandle windowHandle,fw::GLContext::Settings settings) : m_context(new priv::GLContext)
	{
		create(windowHandle,settings);
	}

	/////////////////////////////////////////////////////////////
	GLContext::GLContext(const fm::vec2s &size,fw::GLContext::Settings settings) : m_context(new priv::GLContext)
	{
		create(size,settings);
	}

	/////////////////////////////////////////////////////////////
	GLContext::~GLContext()
	{
		delete m_context;
	}

	/////////////////////////////////////////////////////////////
	bool GLContext::setActive(bool active)
	{
		return m_context->setActive(active);
	}

	/////////////////////////////////////////////////////////////
	bool GLContext::swapBuffers()
	{
		return m_context->swapBuffers();
	}

	/////////////////////////////////////////////////////////////
	bool GLContext::create(priv::WindowHandle windowHandle,fw::GLContext::Settings settings)
	{
		// forward the call to the implementation
		return m_context->create((priv::Window::Handle)windowHandle,
								 (priv::GLContext::Handle)priv::getSharedContext().getHandle(),
								  settings);
	}

	/////////////////////////////////////////////////////////////
	bool GLContext::create(const fm::vec2s &size,fw::GLContext::Settings settings)
	{
		// forward the call to the implementation
		return m_context->create((priv::GLContext::Handle)priv::getSharedContext().getHandle(),
								  size.w,size.h,
								  settings);
	}

	/////////////////////////////////////////////////////////////
	bool GLContext::create(fw::GLContext::Settings settings)
	{
		return create(fm::vec2s(1,1),settings);
	}

	/////////////////////////////////////////////////////////////
	bool GLContext::destroy()
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
		glClearColor(color.r,color.g,color.b,color.a);
	}

	/////////////////////////////////////////////////////////////
	void GLContext::setClearDepth(float depth)
	{
		glClearDepth(depth);
	}

	/////////////////////////////////////////////////////////////
	void GLContext::clear(bool colorBuffer,bool depthBuffer,bool stencilBuffer)
	{
		if (colorBuffer || depthBuffer || stencilBuffer)
			glClear((colorBuffer ? GL_COLOR_BUFFER_BIT : 0)|(depthBuffer ? GL_DEPTH_BUFFER_BIT : 0)|(stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0));
	}

	/////////////////////////////////////////////////////////////
	void GLContext::setBlend(BlendMode mode)
	{
		if (mode == Overwrite)
			glDisable(GL_BLEND);
		if (mode == Additive)
			glEnable(GL_BLEND),
			glBlendFunc(GL_ONE,GL_ONE);
		if (mode == Alpha)
			glEnable(GL_BLEND),
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	/////////////////////////////////////////////////////////////
	void GLContext::setDepthTest(DepthTestMode mode)
	{
		if (mode != Unused)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		if (mode == Less)    glDepthFunc(GL_LESS);
		if (mode == LEqual)  glDepthFunc(GL_LEQUAL);
		if (mode == GEqual)  glDepthFunc(GL_GEQUAL);
		if (mode == Greater) glDepthFunc(GL_GREATER);
		if (mode == Always)  glDepthFunc(GL_ALWAYS);
	}

	/////////////////////////////////////////////////////////////
	fg::Image GLContext::capture(const fm::vec2s &pos,const fm::vec2s &size,bool flip)
	{
		fg::Image ret;
		ret.create(size);
		glReadPixels(pos.x,pos.y,size.w,size.h,GL_RGBA,GL_UNSIGNED_BYTE,&ret.getPixel(0,0));
		if (flip)
			ret.flipVertically();
		return ret;
	}
}

#endif // FRONTIER_NO_CONTEXT
