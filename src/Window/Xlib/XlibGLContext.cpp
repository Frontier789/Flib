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
#include <FRONTIER/Window/Xlib/XlibGLContext.hpp>
#include <FRONTIER/System/Delegate.hpp>
#include <X11/cursorfont.h>
#include <mutex>
#include <string>

#define GLX_CONTEXT_MAJOR_VERSION_ARB 			  0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 			  0x2092
#define GLX_CONTEXT_PROFILE_MASK_ARB  			  0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
	#define FRONTIER_LOCK_MUTEX(m) m.lock()
	#define FRONTIER_UNLOCK_MUTEX(m) m.unlock()
#else
	#define FRONTIER_LOCK_MUTEX(m) ((void)0)
	#define FRONTIER_UNLOCK_MUTEX(m) ((void)0)
#endif

namespace fw
{
	namespace priv
	{
		Display *globDisp = 0;
		unsigned int globDispN = 0;
		
		#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		std::mutex globDispMutex;
		#endif

		Display *getGlobDisp()
		{
			FRONTIER_LOCK_MUTEX(globDispMutex);
			
			if (globDispN > 0) globDispN++;
			else
			{
				globDisp = XOpenDisplay(NULL);
				globDispN = 1;
			}
			
			FRONTIER_UNLOCK_MUTEX(globDispMutex);
			
			return globDisp;
		}

		void freeGlobDisp(Display *disp)
		{
			FRONTIER_LOCK_MUTEX(globDispMutex);
			
			if (globDispN > 0 && globDisp == disp) 
			{
				globDispN--;
				if (globDispN == 0)
				{
					XCloseDisplay(globDisp);
					globDisp = 0;
				}
			}
			
			FRONTIER_UNLOCK_MUTEX(globDispMutex);
		}
		
		
		
		#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		std::mutex globXwinMutex;
		#endif
		
		std::deque< ::Window> xwins;
		std::deque< ::Cursor> xCurrCursors;
		
		void regXWin(::Window win)
		{
			FRONTIER_LOCK_MUTEX(priv::globXwinMutex);
			
			xwins.push_back(win);
			xCurrCursors.push_back(None);
			
			FRONTIER_UNLOCK_MUTEX(priv::globXwinMutex);
		}
		
		void unregXWin(::Window win)
		{
			FRONTIER_LOCK_MUTEX(priv::globXwinMutex);
			
			C(xwins.size())
				if (xwins[i] == win)
				{
					xwins[i] = xwins.back();
					xwins.pop_back();
					
					xCurrCursors[i] = xCurrCursors.back();
					xCurrCursors.pop_back();
					
					break;
				}
			
			FRONTIER_UNLOCK_MUTEX(priv::globXwinMutex);
		}
		
		class CurHolder
		{
		public:
			::Cursor curs[(fm::Size)Mouse::CursorCount];
			::Display *disp;
			
			CurHolder() : disp(0)
			{
				C(sizeof(curs)/sizeof(*curs))
					curs[i] = 0;
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
					disp = fw::priv::getGlobDisp();

				FRONTIER_LOCK_MUTEX(priv::globDispMutex);
				
				fm::Size index = (int)cursor;
				
				if (index >= (fm::Size)Mouse::CursorCount) index = 0;
				
				::Cursor &cur = curs[index];
				
				if (!cur)
				{
					int data[(fm::Size)Mouse::CursorCount] = 
					{
						XC_left_ptr,
						XC_xterm,
						XC_hand2,
						XC_X_cursor,
						XC_watch,
						XC_top_side,
						XC_left_side,
						XC_bottom_side,
						XC_right_side,
						XC_top_right_corner,
						XC_top_left_corner,
						XC_bottom_left_corner,
						XC_bottom_right_corner
					};
				
					cur = XCreateFontCursor(disp,data[index]);
				}

				FRONTIER_UNLOCK_MUTEX(priv::globDispMutex);
				
				return cur;
			}
		};
		
		CurHolder curHolder;
	}

	void setCursorXlib(::Cursor cursor)
	{
		FRONTIER_LOCK_MUTEX(priv::globXwinMutex);
		FRONTIER_LOCK_MUTEX(priv::globDispMutex);
		
		if (priv::globDisp)
			C(priv::xwins.size())
				if (priv::xCurrCursors[i] != cursor)
				{
					priv::xCurrCursors[i] = cursor;
					
					XDefineCursor(priv::globDisp,priv::xwins[i],cursor);
				}
		
		
		FRONTIER_UNLOCK_MUTEX(priv::globDispMutex);
		FRONTIER_UNLOCK_MUTEX(priv::globXwinMutex);
	}
	
	void Mouse::setCursor(Mouse::Cursor cursor)
	{
		setCursorXlib(priv::curHolder.fetch(cursor));
	}
	
	
	namespace Xlib
	{
		struct MouseCursorData
		{
			::Cursor cursor;
			::Display *display;
		};
		
		Cursor imageToCursor(const fg::Image &image,
							 fm::vec2s clickSpot,
							 fm::Uint8 transparencyLimit,
							 ::Display *disp);
		
		void freeMouseCursorData(void *&dataPtr)
		{
			if (dataPtr)
			{
				MouseCursorData &data = *(MouseCursorData*)dataPtr;
				XFreeCursor(data.display,data.cursor),
				fw::priv::freeGlobDisp(data.display);
				
				delete (MouseCursorData*)dataPtr;
				dataPtr = nullptr;
			}
		}
		
		
	}
	
	/////////////////////////////////////////////////////////////
	MouseCursor::~MouseCursor()
	{
		Xlib::freeMouseCursorData(m_impl);
	}
	
	
	/////////////////////////////////////////////////////////////
	void MouseCursor::loadFromImage(const fg::Image &img,
									fm::vec2s clickSpot,
									fm::Uint8 transparencyLimit)
	{
		Xlib::freeMouseCursorData(m_impl);
		
		Xlib::MouseCursorData *ptr = new Xlib::MouseCursorData;
		ptr->display = fw::priv::getGlobDisp();
		ptr->cursor  = Xlib::imageToCursor(img,clickSpot,transparencyLimit,ptr->display);
		
		m_impl = ptr;
	}
	
	/////////////////////////////////////////////////////////////
	void MouseCursor::setAsCurrent() const
	{
		if (m_impl)
			setCursorXlib( ((Xlib::MouseCursorData *)m_impl)->cursor );
	}
	
	namespace Xlib
	{
		Cursor imageToCursor(const fg::Image &image,
							 fm::vec2s clickSpot,
							 fm::Uint8 transparencyLimit,
							 ::Display *disp)
		{
			fm::vec2s s = image.getSize();
			fm::Size scanline = s.w / 8 + (s.w % 8 ? 1 : 0);

			fm::Size freq[256] = {0};
			fm::Size sum = 0;
			fm::Size inCount = 0;

			image.forEach([&](fm::vec2s /*p*/,fg::Color &c) {
				if (c.a >= transparencyLimit)
				{
					fm::Size grey = 0.2126*c.r + 0.7152*c.g + 0.0722*c.b;
					freq[grey]++;
					inCount++;
					sum += grey;
				}
			});

			float avg = sum / float(inCount); 
			fm::Size sumSmall = 0, sumBig = 0;
			fm::Size inSmall = 0,  inBig  = 0;

			C(avg)
			{
				sumSmall += i * freq[i];
				inSmall  += freq[i];
			}

			sumBig = sum - sumSmall;
			inBig  = inCount - inSmall;

			XColor foreground, background;

			foreground.pixel = 0;
			foreground.red = 65535 / 255 * float(sumBig) / inBig;
			foreground.green = 65535 / 255 * float(sumBig) / inBig;
			foreground.blue = 65535 / 255 * float(sumBig) / inBig;
			foreground.flags = DoRed | DoGreen | DoBlue;

			background.pixel = 0;
			background.red = 65535 / 255 * float(sumSmall) / inSmall;
			background.green = 65535 / 255 * float(sumSmall) / inSmall;
			background.blue = 65535 / 255 * float(sumSmall) / inSmall;
			background.flags = DoRed | DoGreen | DoBlue;

			char *cur_bits  = new char[scanline * s.h];
			char *mask_bits = new char[scanline * s.h];

			C(scanline * s.h) cur_bits[i] = 0, mask_bits[i] = 0;

			image.forEach([&](fm::vec2s p,fg::Color &c) {
				
				fm::Size byteOffset = p.y * scanline + p.x / 8;
				fm::Size bitOffset  = p.x % 8;

				if (c.a >= transparencyLimit)
				{
					mask_bits[byteOffset] |= 1 << bitOffset;
					
					fm::Size grey = 0.2126*c.r + 0.7152*c.g + 0.0722*c.b;

					if (grey > avg)
					{
						cur_bits[byteOffset] |= 1 << bitOffset;
					}
				}
			});


			::Window rwin = DefaultRootWindow(disp);
				
			Pixmap source = XCreateBitmapFromData(disp, rwin,
					   cur_bits, s.w, s.h);
			Pixmap mask = XCreateBitmapFromData(disp, rwin,
					   mask_bits, s.w, s.h);
			::Cursor cursor = XCreatePixmapCursor(disp, source, mask,
					   &foreground, &background,
					   clickSpot.x, clickSpot.y);

			XFreePixmap(disp, source);
			XFreePixmap(disp, mask);

			return cursor;
		}
		
		fm::Result GLContext::createContext(GLXFBConfig config,::GLXContext sharedContext)
		{
			// chack for OpenGL support
			int buf;
			if (!glXQueryExtension(m_disp,&buf,&buf)) 
				return fm::Result("XError",fm::Result::OPFailed,"GLXNotSupported","glXQueryExtension",__FILE__,__LINE__);
			

			// if GLX_ARB_create_context is present that means OpenGL 3.0+ is available
			std::string extensions = glXQueryExtensionsString(m_disp, DefaultScreen(m_disp));
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
					return fm::Result("XError",fm::Result::OPFailed,"UnkownXError","glXCreateContextAttribsARB",__FILE__,__LINE__);
				
				
				// set an empty error handler as glXCreateContextAttribsARB will often cause GLXBadFBConfig and BadMatch
				int (*oldHandler)(Display*,XErrorEvent*) = XSetErrorHandler(errorHandler);


				while (!m_handle && m_settings.majorVersion > 0)
				{
					// attempt to create 
					m_handle = glXCreateContextAttribsARB(m_disp,config,sharedContext,True,attributes);

					if (m_handle)
					{
						XSetErrorHandler(oldHandler);
						return fm::Result();
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
				return fm::Result("XError",fm::Result::OPFailed,"UnkownXError","glXCreateNewContext",__FILE__,__LINE__);

			return fm::Result();
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
		fm::Result GLContext::setActive(bool active)
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

				return fm::Result();
			}
			return fm::Result();
		}
		
		/////////////////////////////////////////////////////////////
		fm::Result GLContext::getSize(unsigned int &w,unsigned int &h)
		{
			if (m_win)
			{
				int x,y;
				unsigned int border,depth;
				
				::Window rootWin = None;

				// simply retrieve the data
				XGetGeometry(m_disp,m_win,&rootWin,&x,&y,&w,&h,&border,&depth);
			}
			else if (m_pbuf)
			{
				int x,y;
				unsigned int border,depth;
				
				::Window rootWin = None;
				
				// simply retrieve the data
				XGetGeometry(m_disp,m_pbuf,&rootWin,&x,&y,&w,&h,&border,&depth);
			}
			
			return fm::Result();
		}
					
		/////////////////////////////////////////////////////////////
		fm::Result GLContext::swapBuffers()
		{
			if (m_handle)
			{
				if (m_pbuf)
					glXSwapBuffers(m_disp,m_pbuf);
				else
					glXSwapBuffers(m_disp,m_win);
				
				return fm::Result();
			}
			return fm::Result();
		}
					
		/////////////////////////////////////////////////////////////
		fm::Result GLContext::create(::Window window,::GLXContext sharedContext,fw::GLContext::Settings settings)
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
				return fm::Result("XError",fm::Result::OPFailed,"NoDisplay","getGlobDisp",__FILE__,__LINE__);

			// choose best configuration available
			GLXFBConfig config;
			
			fm::Result res = priv::getFBConfig(m_disp,&config,true);
			if (!res) return res;

			return createContext(config,sharedContext);
		}
					
		/////////////////////////////////////////////////////////////
		fm::Result GLContext::create(::GLXContext sharedContext,unsigned int width,unsigned int height,fw::GLContext::Settings settings)
		{
			// start by cleaning
			destroy();

			// copy settings
			m_settings = settings;

			// open our own Display
			m_disp = priv::getGlobDisp();
			
			if (!m_disp)
				return fm::Result("XError",fm::Result::OPFailed,"NoDisplay","getGlobDisp",__FILE__,__LINE__);

			// choose best configuration available
			GLXFBConfig config;
			
			fm::Result res = priv::getFBConfig(m_disp,&config,true);			
			if (!res) return res;

			int attributes[] = {GLX_PBUFFER_WIDTH,      (int)width,  //< The requested width
								GLX_PBUFFER_HEIGHT,     (int)height, //< The requested height
								GLX_LARGEST_PBUFFER,    True,   //< If resource is not enough for width x height, allocate the biggest available
								GLX_PRESERVED_CONTENTS, True,   //< Upon resource conflict the data shall be preserved
								None};

			// create a offscreen pixel buffer
			m_pbuf = glXCreatePbuffer(m_disp,config,attributes);

			if (!m_pbuf)
				return fm::Result("XError",fm::Result::OPFailed,"UnkownXError","glXCreatePbuffer",__FILE__,__LINE__);

			return createContext(config,sharedContext);
		}
					
		/////////////////////////////////////////////////////////////
		fm::Result GLContext::destroy()
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

			return fm::Result();
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



