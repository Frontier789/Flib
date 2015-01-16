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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Window/GLContext.hpp>
#include <FRONTIER/System/macros/OS.h>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Window/Window.hpp>
#include <FRONTIER/System/Mutex.hpp>

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
		fw::GLContext theSharedContext;
		fm::Mutex sharedContextMutex;
		class theSharedContextInitializer
		{
		public:
			theSharedContextInitializer(fw::GLContext *ptr)
			{
				ptr->m_context->create(NULL,100,100,fw::GLContext::Settings());
			}
		};
		theSharedContextInitializer initit(&theSharedContext);
	}
	/////////////////////////////////////////////////////////////
	GLContext::Settings::Settings(unsigned char bitsPerPixel,
								  unsigned char depthBits,
								  unsigned char stencilBits,
								  unsigned char majorVersion,
								  unsigned char minorVersion,
								  bool compatiblityProfile) : bitsPerPixel(bitsPerPixel),
															  depthBits(depthBits),
															  stencilBits(stencilBits),
															  majorVersion(majorVersion),
															  minorVersion(minorVersion),
															  compatiblityProfile(compatiblityProfile)
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
	{/*
		if (!hasThreadGL())
		{
			// for some strange reason an OpenGL context 
			// is needed to be active for sharing to work properly
			fw::priv::GLContext tmpContext;
			tmpContext.create();
			tmpContext.setActive();
			bool success = m_context->create((priv::Window::Handle)windowHandle,
											 (priv::GLContext::Handle)priv::theSharedContext.getHandle(),
											  settings);
			tmpContext.setActive(false);
			return success;
		}*/
		
		// forward the call to the implementation
		return m_context->create((priv::Window::Handle)windowHandle,
								 (priv::GLContext::Handle)priv::theSharedContext.getHandle(),
								  settings);
	}

	/////////////////////////////////////////////////////////////
	bool GLContext::create(const fm::vec2s &size,fw::GLContext::Settings settings)
	{/*
		bool useShared = false;
		if (!hasThreadGL())
		{
			useShared = true;
			priv::sharedContextMutex.lock();
			priv::theSharedContext.setActive();
		}
*/
		bool success = m_context->create((priv::GLContext::Handle)priv::theSharedContext.getHandle(),
										  size.w,size.h,
										  settings);

	/*	if (useShared)
		{
			priv::theSharedContext.setActive(false);
			priv::sharedContextMutex.unLock();
		}*/

		return success;
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
}

#endif // FRONTIER_NO_CONTEXT
