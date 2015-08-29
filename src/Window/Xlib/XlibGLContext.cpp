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
#include <FRONTIER/Window/Xlib/XlibGLContext.hpp>
#include <FRONTIER/Window/FwLog.hpp>
#include <string>

#define GLX_CONTEXT_MAJOR_VERSION_ARB 			  0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 			  0x2092
#define GLX_CONTEXT_PROFILE_MASK_ARB  			  0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

namespace fw
{
	namespace Xlib
	{
		GLXFBConfig GLContext::createConfig()
		{
			int attributes[] = {GLX_DOUBLEBUFFER,  True, //< need double buffer
								GLX_RED_SIZE,      m_settings.bitsPerPixel/4,
								GLX_GREEN_SIZE,    m_settings.bitsPerPixel/4,
								GLX_BLUE_SIZE,     m_settings.bitsPerPixel/4,
								GLX_ALPHA_SIZE,    m_settings.bitsPerPixel/4,
								GLX_DEPTH_SIZE,    m_settings.depthBits,
								GLX_STENCIL_SIZE,  m_settings.stencilBits,
								GLX_DRAWABLE_TYPE, m_win ? GLX_WINDOW_BIT : GLX_PIXMAP_BIT,
								None};

			int count;
			// get matching context settings
			GLXFBConfig *fbConfigs = glXChooseFBConfig(m_disp,DefaultScreen(m_disp),attributes,&count);

			if (!fbConfigs)
			{
				// disable double-buffer and retry
				attributes[1] = False; 
				fbConfigs = glXChooseFBConfig(m_disp,DefaultScreen(m_disp),attributes,&count);

				if (!fbConfigs)
				{
					fw_log << "no matching context found" << std::endl;
					return None;
				}
			}

			GLXFBConfig config = fbConfigs[0];
			XFree(fbConfigs);
			return config;
		}

		bool GLContext::createContext(GLXFBConfig config,::GLXContext sharedContext)
		{
			// chack for OpenGL support
			int buf;
			if (!glXQueryExtension(m_disp,&buf,&buf)) 
			{
				fw_log << "OpenGL not supported by X server" << std::endl;
				return false;
			}

			// if GLX_ARB_create_context is present that means OpenGL 3.0+ is available
			std::string extensions =  glXQueryExtensionsString(m_disp, DefaultScreen(m_disp));
			if (extensions.find("GLX_ARB_create_context") != std::string::npos)
			{
				typedef GLXContext (*procType)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
				// retrieve glXCreateContextAttribsARB
				procType glXCreateContextAttribsARB = (procType)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

				int attributes[] = {GLX_CONTEXT_MAJOR_VERSION_ARB, (int)m_settings.majorVersion,
									GLX_CONTEXT_MINOR_VERSION_ARB, (int)m_settings.minorVersion,
									GLX_CONTEXT_PROFILE_MASK_ARB,  m_settings.compatiblityProfile ? GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
									None};

				if (!glXCreateContextAttribsARB)
				{
					fw_log << "glXCreateContextAttribsARB is NULL" << std::endl;
					return false;
				}
				
				// set an empty error handler as glXCreateContextAttribsARB will often cause GLXBadFBConfig and BadMatch
				int (*oldHandler)(Display*,XErrorEvent*) = XSetErrorHandler(errorHandler);


				while (!m_handle && m_settings.majorVersion > 0)
				{
					// attempt to create 
					m_handle = glXCreateContextAttribsARB(m_disp,config,sharedContext,True,attributes);

					if (m_handle)
					{/*
						fw_log << attributes[5] << " _kecske" << std::endl;
						int profile;
						glGetIntegerv(GL_CONTEXT_PROFILE_MASK,&profile);
						fw_log << profile << std::endl;*/
						return true;
					}

					m_settings.decreaseVersion();

					attributes[1] = m_settings.majorVersion;
					attributes[3] = m_settings.minorVersion;
				}

				// set back the old handler
				XSetErrorHandler(oldHandler);
			}

			// create the handle
			m_handle = glXCreateNewContext(m_disp,config,GLX_RGBA_TYPE,sharedContext,True);

			m_settings.majorVersion = 2,
			m_settings.minorVersion = 9;

			if (!m_handle)
			{
				fw_log << "glXCreateNewContext failed" << std::endl;
				return false;
			}

			return true;
		}

		/////////////////////////////////////////////////////////////
		int GLContext::errorHandler(Display *disp,XErrorEvent *xerrev)
		{
			// unused
			(void)disp;
			(void)xerrev;
			
			return 0;
		}

		/////////////////////////////////////////////////////////////
		GLContext::GLContext() : m_win(None),
								 m_disp(NULL),
								 m_pbuf(None),
								 m_handle(NULL)
		{

		}

		/////////////////////////////////////////////////////////////
		GLContext::~GLContext()
		{
			destroy();
		}
					
		/////////////////////////////////////////////////////////////
		bool GLContext::setActive(bool active)
		{
			if (m_handle)
			{
				if (active)
				{
					if (m_pbuf)
						glXMakeCurrent(m_disp,m_pbuf,m_handle);
					else
						glXMakeCurrent(m_disp,m_win,m_handle);			
				}
				else
					glXMakeCurrent(m_disp,None,NULL);

				return true;
			}
			return false;
		}
					
		/////////////////////////////////////////////////////////////
		bool GLContext::swapBuffers()
		{
			if (m_handle)
			{
				if (m_pbuf)
					glXSwapBuffers(m_disp,m_pbuf);
				else
					glXSwapBuffers(m_disp,m_win);
				
				return true;
			}
			return false;
		}
					
		/////////////////////////////////////////////////////////////
		bool GLContext::create(::Window window,::GLXContext sharedContext,fw::GLContext::Settings settings)
		{
			// start by cleaning
			destroy();

			// copy settings
			m_settings = settings;

			// copy window
			m_win  = window;

			// connect to X
			m_disp = XOpenDisplay(NULL);

			if (!m_disp)
			{
				fw_log << "XOpenDisplay failed" << std::endl;
				return false;
			}

			// choose best configuration available
			GLXFBConfig config = createConfig();
			
			if (!config)
				return false;

			return createContext(config,sharedContext);
		}
					
		/////////////////////////////////////////////////////////////
		bool GLContext::create(::GLXContext sharedContext,unsigned int width,unsigned int height,fw::GLContext::Settings settings)
		{
			// start by cleaning
			destroy();

			// copy settings
			m_settings = settings;

			// open our own Display
			m_disp = XOpenDisplay(NULL);

			if (!m_disp)
			{
				fw_log << "XOpenDisplay failed" << std::endl;
				return false;
			}

			// choose best configuration available
			GLXFBConfig config = createConfig();

			if (!config)
				return false;

			int attributes[] = {GLX_PBUFFER_WIDTH,      (int)width,  //< The requested width
								GLX_PBUFFER_HEIGHT,     (int)height, //< The requested height
								GLX_LARGEST_PBUFFER,    True,   //< If resource is not enough for width x height, allocate the biggest available
								GLX_PRESERVED_CONTENTS, True,   //< Upon resource conflict the data shall be preserved
								None};

			// create a offscreen pixel buffer
			m_pbuf = glXCreatePbuffer(m_disp,config,attributes);

			if (!m_pbuf)
			{
				fw_log << "glXCreatePbuffer failed" << std::endl;
				return false;
			}

			return createContext(config,sharedContext);
		}
					
		/////////////////////////////////////////////////////////////
		bool GLContext::destroy()
		{
			// delete the context
			if (m_handle)
			{
				if (glXGetCurrentContext() == m_handle)
					glXMakeCurrent(m_disp,None,NULL);

				glXDestroyContext(m_disp,m_handle);
				m_handle = NULL;
			}

			// delete pixel buffer
			if (m_pbuf)
			{
				glXDestroyPbuffer(m_disp,m_pbuf);
				m_pbuf = None;
			}

			// close the connection with X
			if (m_disp)
			{
				XCloseDisplay(m_disp);
				m_disp = NULL;
			}

			// forget the window
			m_win = None;

			return true;
		}

		/////////////////////////////////////////////////////////////
		bool GLContext::hasThreadGL()
		{
			return glXGetCurrentContext();
		}


		/////////////////////////////////////////////////////////////
		GLContext::Handle GLContext::getHandle() const
		{
			return m_handle;
		}
					
		/////////////////////////////////////////////////////////////
		GLContext::operator GLContext::Handle() const
		{
			return m_handle;
		}
					
		/////////////////////////////////////////////////////////////
		const fw::GLContext::Settings &GLContext::getSettings() const
		{
			return m_settings;
		}
	}
}


