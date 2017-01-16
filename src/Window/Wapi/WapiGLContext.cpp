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
#include <FRONTIER/Window/Wapi/WapiGetLastError.hpp>
#include <FRONTIER/Window/Wapi/WapiGLContext.hpp>

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB               0x2093
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define ERROR_INVALID_VERSION_ARB                 0x2095
#define ERROR_INVALID_PROFILE_ARB                 0x2096

#define FRONTIER_DUMMY_WINDOW_CLASS "dummy_class"

namespace fw
{
	namespace Wapi
	{
		unsigned int GLContext::m_contextWindowCount = 0;

		/////////////////////////////////////////////////////////////
		GLContext::GLContext() : m_hdc(NULL),
								 m_hwnd(NULL),
								 m_hglrc(NULL),
								 m_ownWindow(NULL)
		{

		}

		/////////////////////////////////////////////////////////////
		GLContext::~GLContext()
		{
			destroy();
		}

		/////////////////////////////////////////////////////////////
		fm::Result GLContext::destroy()
		{
			if (m_ownWindow) // if the context created that window
			{
				if (m_hwnd) // if the window was valid, destroy it
				{
					if (DestroyWindow(m_hwnd))
					{
						m_hwnd = NULL;
						m_contextWindowCount--;

						// If there are no more dummy windows deregister their class
						if (!m_contextWindowCount)
							if (!UnregisterClassA(FRONTIER_DUMMY_WINDOW_CLASS, GetModuleHandle(NULL)))
								return fw::WapiGetLastError("UnregisterClassA");
					}
					else
						return fw::WapiGetLastError("DestroyWindow");
				}
				m_ownWindow = false;
			}

			// Discard context
			if (m_hglrc)
			{
				if (!wglDeleteContext(m_hglrc))
					return fw::WapiGetLastError("wglDeleteContext");
				
				m_hglrc = NULL;
			}

			// Release the DC
			if (m_hdc)
				ReleaseDC(m_hwnd,m_hdc),
				m_hdc = NULL;

			// reset window handle
			m_hwnd = NULL;

			return fm::Result();
		}

		/////////////////////////////////////////////////////////////
		fm::Result GLContext::init(HGLRC sharedContext)
		{
			// deactivate context
			HGLRC oldContext = wglGetCurrentContext();
			HDC   oldHDC = wglGetCurrentDC();
			
			if (oldContext)
				wglMakeCurrent(NULL,NULL);
			
			// Get DeviceContext of the window
			m_hdc = GetDC(m_hwnd);
			if (!m_hdc) // Check for errors
				return fw::WapiGetLastError("GetDC");

			// set up pixelformet
			fm::Result res = setPixelFormat();
			
			if (!res) return res;

			// wglCreateContextAttribsARB is suitable for creating GL context 3.x and above
			while (m_settings.majorVersion >= 3)
			{
				// Retrieve the neede function pointer
				HGLRC (*wglCreateContextAttribsARB)(HDC,HGLRC,const int*)  = (HGLRC (*)(HDC,HGLRC,const int*))wglGetProcAddress("wglCreateContextAttribsARB");

				// the function may not be available
				if (wglCreateContextAttribsARB)
				{
					int attributes[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, m_settings.majorVersion,
					                   	 WGL_CONTEXT_MINOR_VERSION_ARB, m_settings.minorVersion,
										 WGL_CONTEXT_PROFILE_MASK_ARB, m_settings.compatiblityProfile ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB :
																										WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
										 0, 0};

					m_hglrc = wglCreateContextAttribsARB(m_hdc, NULL, attributes);
				}

				// decrease the version
				if (!m_hglrc)
					m_settings.decreaseVersion();

				else break;
			}

			// If the context still not created
			if (!m_hglrc)
			{
				// use wglCreateContext
				m_hglrc = wglCreateContext(m_hdc);
				if (!m_hglrc)
					return fw::WapiGetLastError("wglCreateContext");
				
				const unsigned char *(*glGetString)(unsigned int) = (const unsigned char *(*)(unsigned int))wglGetProcAddress("glGetString");

				if (glGetString)
				{
					unsigned int i=0;

					// find out the version
					wglMakeCurrent(m_hdc,m_hglrc);

					m_settings.majorVersion = 0;
					m_settings.minorVersion = 0;
					const unsigned char *version = glGetString(0x1F02 /*GL_VERSION*/); // "MAJOR.MINOR....(extra)"

					// extract major version (loop until the '.')
					for (;*(version+i) >= '0' && *(version+i) <= '9';i++)
						m_settings.majorVersion = m_settings.majorVersion*10+*(version+i)-'0';

					i++; // jump the '.'

					// extract minor version
					for (;*(version+i) >= '0' && *(version+i) <= '9';i++)
						m_settings.minorVersion = m_settings.minorVersion*10+*(version+i)-'0';

					wglMakeCurrent(m_hdc,NULL);
				}
			}

			// share resources
			if (sharedContext)
				if (!wglShareLists(sharedContext,m_hglrc))
					return fw::WapiGetLastError("wglShareLists");
			
			// restore last context
			if (oldContext)
				wglMakeCurrent(oldHDC,oldContext);

			return fm::Result();
		}

		/////////////////////////////////////////////////////////////
		fm::Result GLContext::create(HWND windowHandle,HGLRC sharedContext,fw::GLContext::Settings settings)
		{
			// Start by cleaning
			destroy();

			// Copy the window handle and the settings
			m_hwnd = windowHandle;
			m_settings = settings;

			// The context does not own the window
			m_ownWindow = false;

			return init(sharedContext);
		}

		/////////////////////////////////////////////////////////////
		fm::Result GLContext::create(HGLRC sharedContext,unsigned int width,unsigned int height,fw::GLContext::Settings settings)
		{
			// Start by cleaning
			destroy();

			// iIf needed create a dummy class
			if (m_contextWindowCount == 0)
			{
				WNDCLASS winClass;
				ZeroMemory(&winClass,sizeof(winClass));

				// Fill in the fields of the WNDCLASS
				winClass.lpfnWndProc   = (WNDPROC)DefWindowProc;
				winClass.lpszClassName = FRONTIER_DUMMY_WINDOW_CLASS; // The name of the dummy class

				// Tell windows about the new class
				if (!RegisterClassA(&winClass))
					return fw::WapiGetLastError("RegisterClassA");
			}

			// note that a new window is being created
			m_contextWindowCount++;

			// create the new (hidden) window
			m_hwnd = CreateWindowA(FRONTIER_DUMMY_WINDOW_CLASS,
								   "dummy_window",WS_POPUP,
								   CW_USEDEFAULT,CW_USEDEFAULT,width,height,
								   NULL,NULL,NULL,NULL);

			if (!m_hwnd)
				return fw::WapiGetLastError("CreateWindowA");

			// the context owns this window
			m_ownWindow = true;

			// copy the settings
			m_settings = settings;

			return init(sharedContext);
		}

		/////////////////////////////////////////////////////////////
		bool GLContext::hasThreadGL()
		{
			return wglGetCurrentContext();
		}

		/////////////////////////////////////////////////////////////
		fm::Result GLContext::setActive(bool active)
		{
			BOOL result = 0;
			// If this class owns a valid context and it is being activated
			if (m_hdc && m_hglrc && active)
				result = wglMakeCurrent(m_hdc,m_hglrc); // Then activate it
			else
				result = wglMakeCurrent(m_hdc,NULL);    // Otherwise deactivate the current

			if (!result) // Check for errors
				return fw::WapiGetLastError("wglMakeCurrent");

			return fm::Result();
		}
		
		/////////////////////////////////////////////////////////////
		fm::Result GLContext::getSize(unsigned int &w,unsigned int &h)
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
					return fw::WapiGetLastError("GetClientRect");

				w = client_rect.right;
				h = client_rect.bottom;
			}
			
			return fm::Result();
		}

		/////////////////////////////////////////////////////////////
		fm::Result GLContext::swapBuffers()
		{
			if (m_hdc && m_hglrc)
				if (!SwapBuffers(m_hdc))
					return fw::WapiGetLastError("SwapBuffers");
				
			return fm::Result();
		}

		/////////////////////////////////////////////////////////////
		GLContext::Handle GLContext::getHandle() const
		{
			return m_hglrc;
		}

		/////////////////////////////////////////////////////////////
		GLContext::operator GLContext::Handle() const
		{
			return m_hglrc;
		}

		/////////////////////////////////////////////////////////////
		fm::Result GLContext::setPixelFormat()
		{
			// fill out the pfd with our requirements
			PIXELFORMATDESCRIPTOR descriptor;

			ZeroMemory(&descriptor,sizeof(descriptor));

			descriptor.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
			descriptor.nVersion     = 1;
			descriptor.iLayerType   = PFD_MAIN_PLANE;
			descriptor.dwLayerMask  = PFD_MAIN_PLANE;
			descriptor.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			descriptor.iPixelType   = PFD_TYPE_RGBA;
			descriptor.cColorBits   = 32;
			descriptor.cDepthBits   = 24;
			descriptor.cStencilBits = 8;
			descriptor.cAlphaBits   = 8;

			// ask windows for a pixelformat's id that matches our requirements
			int formatID = ChoosePixelFormat(m_hdc, &descriptor);
			if (!formatID)
				return fw::WapiGetLastError("ChoosePixelFormat");
			

			// Extract the depth and stencil bits from the chosen format
			PIXELFORMATDESCRIPTOR obtainedFormat;
			obtainedFormat.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
			obtainedFormat.nVersion = 1;
			DescribePixelFormat(m_hdc, formatID, sizeof(PIXELFORMATDESCRIPTOR), &obtainedFormat);

			// Set the chosen pixel format
			if (!SetPixelFormat(m_hdc, formatID, &obtainedFormat))
				return fw::WapiGetLastError("SetPixelFormat");

			return fm::Result();
		}

		/////////////////////////////////////////////////////////////
		const fw::GLContext::Settings &GLContext::getSettings() const
		{
			return m_settings;
		}
	}
}
