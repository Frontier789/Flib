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
#include <FRONTIER/Window/Xlib/XlibGLContext.hpp>
#include <X11/cursorfont.h>
#include <string>

#define GLX_CONTEXT_MAJOR_VERSION_ARB 			  0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 			  0x2092
#define GLX_CONTEXT_PROFILE_MASK_ARB  			  0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

namespace fw
{
	namespace priv
	{
		Display *globDisp = 0;
		unsigned int globDispN = 0;
		
		#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		fm::Mutex globDispMutex;
		#endif

		Display *getGlobDisp()
		{
			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			globDispMutex.lock();
			#endif
			
			if (globDispN > 0) globDispN++;
			else
			{
				globDisp = XOpenDisplay(NULL);
				globDispN = 1;
			}
			
			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			globDispMutex.unLock();
			#endif
			
			return globDisp;
		}

		void freeGlobDisp(Display *disp)
		{
			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			globDispMutex.lock();
			#endif
			
			if (globDispN > 0 && globDisp == disp) 
			{
				globDispN--;
				if (globDispN == 0)
				{
					XCloseDisplay(globDisp);
					globDisp = 0;
				}
			}
			
			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			globDispMutex.unLock();
			#endif
		}
		
		
		
		#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		fm::Mutex globXwinMutex;
		#endif
		
		std::deque< ::Window> xwins;
		
		void regXWin(::Window win)
		{
			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			priv::globXwinMutex.lock();
			#endif
			
			xwins.push_back(win);
			
			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			priv::globXwinMutex.unLock();
			#endif
		}
		
		void unregXWin(::Window win)
		{
			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			priv::globXwinMutex.lock();
			#endif
			
			C(xwins.size())
				if (xwins[i] == win)
				{
					xwins[i] = xwins.back();
					xwins.pop_back();
					break;
				}
			
			#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
			priv::globXwinMutex.unLock();
			#endif
		}
		
		class CurHolder
		{
		public:
			::Cursor curs[(fm::Size)Mouse::CursorCount];
			::Display *disp;
			
			CurHolder() : disp(0)
			{
				C(sizeof(curs)/sizeof(*curs))
					curs[i] = None;
			}
			
			~CurHolder()
			{
				if (disp)
				{
					C(sizeof(curs)/sizeof(*curs))	
						if (curs[i])
							XFreeCursor(disp,curs[i]);
						
					fw::priv::freeGlobDisp(disp);
				}
			}
			
			::Cursor fetch(Mouse::Cursor cursor)
			{
				if (!disp)
				{
					if (globDispN > 0) globDispN++;
					else
					{
						globDisp = XOpenDisplay(NULL);
						globDispN = 1;
					}
					
					disp = globDisp;
				}
				
				fm::Size index = (int)cursor;
				
				if (index >= (fm::Size)Mouse::CursorCount) index = 0;
				
				::Cursor &cur = curs[index];
				
				if (!cur)
				{
					int data[(fm::Size)Mouse::CursorCount] = {XC_left_ptr,XC_xterm,XC_hand2,XC_X_cursor,XC_watch};
				
					cur = XCreateFontCursor(disp,data[index]);
				}
				
				return cur;
			}
		};
		
		CurHolder curHolder;
	}
	
	void Mouse::setCursor(Mouse::Cursor cursor)
	{
		#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		priv::globXwinMutex.lock();
		priv::globDispMutex.lock();
		#endif
		
		// priv::globCursor = cursor;
		
		if (priv::globDisp)
			C(priv::xwins.size())
				XDefineCursor(priv::globDisp,priv::xwins[i],priv::curHolder.fetch(cursor));
		
		
		#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		priv::globDispMutex.unLock();
		priv::globXwinMutex.unLock();
		#endif
	}
		
	namespace Xlib
	{
		bool GLContext::createContext(GLXFBConfig config,::GLXContext sharedContext)
		{
			// chack for OpenGL support
			int buf;
			if (!glXQueryExtension(m_disp,&buf,&buf)) 
				return false;
			

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
					return false;
				
				
				// set an empty error handler as glXCreateContextAttribsARB will often cause GLXBadFBConfig and BadMatch
				int (*oldHandler)(Display*,XErrorEvent*) = XSetErrorHandler(errorHandler);


				while (!m_handle && m_settings.majorVersion > 0)
				{
					// attempt to create 
					m_handle = glXCreateContextAttribsARB(m_disp,config,sharedContext,True,attributes);

					if (m_handle)
					{
						XSetErrorHandler(oldHandler);
						return true;
					}

					m_settings.decreaseVersion();

					attributes[1] = m_settings.majorVersion;
					attributes[3] = m_settings.minorVersion;
				}
				
				XFlush(m_disp);

				// set back the old handler
				XSetErrorHandler(oldHandler);
			}

			// create the handle
			m_handle = glXCreateNewContext(m_disp,config,GLX_RGBA_TYPE,sharedContext,True);

			m_settings.majorVersion = 2,
			m_settings.minorVersion = 1;

			if (!m_handle)
				return false;

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
			m_win = window;

			// connect to X
			m_disp = priv::getGlobDisp();
			
			if (!m_disp)
				return false;

			// choose best configuration available
			GLXFBConfig config;
			
			if (!priv::getFBConfig(m_disp,&config,true))
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
			m_disp = priv::getGlobDisp();
			
			if (!m_disp)
				return false;

			// choose best configuration available
			GLXFBConfig config;
			
			if (!priv::getFBConfig(m_disp,&config,true))
				return false;


			int attributes[] = {GLX_PBUFFER_WIDTH,      (int)width,  //< The requested width
								GLX_PBUFFER_HEIGHT,     (int)height, //< The requested height
								GLX_LARGEST_PBUFFER,    True,   //< If resource is not enough for width x height, allocate the biggest available
								GLX_PRESERVED_CONTENTS, True,   //< Upon resource conflict the data shall be preserved
								None};

			// create a offscreen pixel buffer
			m_pbuf = glXCreatePbuffer(m_disp,config,attributes);

			if (!m_pbuf)
				return false;

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
				priv::freeGlobDisp(m_disp);
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



