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
#include <FRONTIER/Window/Wapi/fwWapiPrintLastError.hpp>
#include <FRONTIER/Window/Wapi/WapiGlContext.hpp>
#include <FRONTIER/Window/FwLog.hpp>

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
		unsigned int GlContext::m_contextCount = 0;
		
		/////////////////////////////////////////////////////////////
		GlContext::GlContext() : m_hdc(NULL),
								 m_hwnd(NULL),
								 m_hglrc(NULL),
								 m_ownWindow(NULL)
		{
			
		}
		
		/////////////////////////////////////////////////////////////
		GlContext::~GlContext()
		{
			cleanUp();
		}
		
		/////////////////////////////////////////////////////////////
		bool GlContext::cleanUp()
		{
			if (m_ownWindow) // if we created that window
			{
				if (m_hwnd) // if the window was valid we destroy it
				{
					if (DestroyWindow(m_hwnd))
					{
						m_hwnd = NULL;
						m_contextCount--;
				
						// If we dont have any dummy windows left we unregister our dummy window class
						if (!m_contextCount)
							if (!UnregisterClassA(FRONTIER_DUMMY_WINDOW_CLASS, GetModuleHandle(NULL)))
							{
								fw::WapiPrintLastError(fw_log,UnregisterClassA);
								return false;
							}
					}
					else
					{
						fw::WapiPrintLastError(fw_log,DestroyWindow);
						return false;
					}
				}
				m_ownWindow = false;
			}
			
			// If we have a context then discard it
			if (m_hglrc)
			{
				if (!wglDeleteContext(m_hglrc))
				{
					fw::WapiPrintLastError(fw_log,wglDeleteContext);
					return false;
				}
				m_hglrc = NULL;
			}
			
			// Release the DC if we have one
			if (m_hdc)
				ReleaseDC(m_hwnd,m_hdc),
				m_hdc = NULL;
			
			// since we dont own this window we dont do anything with it
			m_hwnd = NULL;
			
			return true;
		}
		
		/////////////////////////////////////////////////////////////
		bool GlContext::create(HWND windowHandle,HGLRC sharedContext,fw::GlContext::Settings settings)
		{
			// Start by cleaning
			cleanUp();
			
			// Copy the window handle and the settings
			m_hwnd = windowHandle;
			m_settings = settings;
			
			// we didn't create the window here
			m_ownWindow = false;
			
			// Get DeviceContext of the window
			m_hdc = GetDC(windowHandle);
			if (!m_hdc) // Check for errors
			{
				fw::WapiPrintLastError(fw_log,GetDC);
				return false;
			}
			
			// TODO: make setPixelFormat better :| 
			if (!setPixelFormat())
				return false;
			
			// wglCreateContextAttribsARB is suitable for creating GL context 3.x and above 
			while (m_settings.majorVersion >= 3)
			{
				// we must retrieve the function pointer ourshelves
				HGLRC (*wglCreateContextAttribsARB)(HDC,HGLRC,const int*)  = (HGLRC (*)(HDC,HGLRC,const int*))wglGetProcAddress("wglCreateContextAttribsARB");
				
				// the function may not be available
				if (wglCreateContextAttribsARB)
				{
					int attributes[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, m_settings.majorVersion,
					                   	 WGL_CONTEXT_MINOR_VERSION_ARB, m_settings.minorVersion,
										 WGL_CONTEXT_PROFILE_MASK_ARB, m_settings.compatiblityProfile ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : 
																										WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
										 0, 0 };
					
					m_hglrc = wglCreateContextAttribsARB(m_hdc, sharedContext, attributes);
				}
				
				// we dont mind checking nonexisting versions 
				if (!m_hglrc)
				{
					if (m_settings.minorVersion) m_settings.minorVersion--;
					else
						m_settings.majorVersion--,
						m_settings.minorVersion = 9;
				}
				else break;
			}

			// If  we still do not have a context
			if (!m_hglrc)
			{
				// use the good old wglCreateContext
				m_hglrc = wglCreateContext(m_hdc);
				if (!m_hglrc)
				{
					fw::WapiPrintLastError(fw_log,wglCreateContext);
					return false;
				}
				
				const unsigned char *(*glGetString)(unsigned int) = (const unsigned char *(*)(unsigned int))wglGetProcAddress("glGetString");
				
				if (glGetString)
				{
					unsigned int i=0;
					
					// find out what version we got
					wglMakeCurrent(m_hdc,m_hglrc);
					
					m_settings.majorVersion = 0;
					m_settings.minorVersion = 0;
					const unsigned char *version = glGetString(0x1F02 /*GL_VERSION*/); // MAJOR.MINOR....(extra) 
					
					// extract major version (loop until the '.')
					for (;*(version+i) >= '0' && *(version+i) <= '9';i++)
						m_settings.majorVersion=m_settings.majorVersion*10+*(version+i)-'0';
					
					i++; // jump the .
					
					// extract minor version
					for (;*(version+i) >= '0' && *(version+i) <= '9';i++)
						m_settings.minorVersion=m_settings.minorVersion*10+*(version+i)-'0';
					
					wglMakeCurrent(m_hdc,NULL);	
				}
			}
			
			// share resources
			if (sharedContext)
				wglShareLists(m_hglrc,sharedContext);
			
			return true;
		}
		
		/////////////////////////////////////////////////////////////
		bool GlContext::create(HGLRC sharedContext,fw::GlContext::Settings settings)
		{
			cleanUp();
			
			if (m_contextCount==0)
			{
				WNDCLASS winClass;
				ZeroMemory(&winClass,sizeof(winClass));
		
				// Fill in the fields of the WNDCLASS
				winClass.lpfnWndProc   = (WNDPROC)DefWindowProc;
				winClass.lpszClassName = FRONTIER_DUMMY_WINDOW_CLASS; // The name of the dummy class
				
				// Tell windows we have a class
				if (!RegisterClassA(&winClass))
				{
					fw::WapiPrintLastError(fw_log,RegisterClassA);
					return false;
				}
			}
			
			m_contextCount++;
			
			HWND hwnd = CreateWindowA(FRONTIER_DUMMY_WINDOW_CLASS,
									  "dummy_window",0,
									  CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
									  NULL,NULL,NULL,NULL);
			
			if (!hwnd)
			{
				fw::WapiPrintLastError(fw_log,CreateWindowA);
				return false;
			}
			
			m_ownWindow = true;
			
			return create(hwnd,sharedContext,settings);
		}
		
		/////////////////////////////////////////////////////////////
		bool GlContext::setActive(bool active)
		{
			BOOL result = 0;
			// If this class owns a valid context and we want to activate it
			if (m_hdc && m_hglrc && active)
				result = wglMakeCurrent(m_hdc,m_hglrc); // Then activate it
			else
				result = wglMakeCurrent(m_hdc,NULL);     // Otherwise deactivate the current
			
			if (!result) // Check for errors
			{
				fw::WapiPrintLastError(fw_log,wglMakeCurrent);
				return false;
			}
			
			return true;
		}
		
		/////////////////////////////////////////////////////////////
		bool GlContext::swapBuffers()
		{
			if (m_hdc && m_hglrc)
				if (!SwapBuffers(m_hdc))
				{
					fw::WapiPrintLastError(fw_log,SwapBuffers);
					return false;
				}
			return true;
		}
		
		/////////////////////////////////////////////////////////////
		GlContext::Handle GlContext::getHandle() const
		{
			return m_hglrc;
		}
		
		/////////////////////////////////////////////////////////////
		GlContext::operator GlContext::Handle() const
		{
			return m_hglrc;
		}
		
		/////////////////////////////////////////////////////////////
		bool GlContext::setPixelFormat()
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
			descriptor.cColorBits   = m_settings.bitsPerPixel;
			descriptor.cDepthBits   = m_settings.depthBits;
			descriptor.cStencilBits = m_settings.stencilBits;
			descriptor.cAlphaBits   = m_settings.bitsPerPixel==32 ? 8 : 0;

			// ask windows for a pixelformat's id that matches our requirements
			int formatID = ChoosePixelFormat(m_hdc, &descriptor);
			if (!formatID)
			{
				fw::WapiPrintLastError(fw_log,to find a suitable pixel format for device context -- cannot create OpenGL context);
				return false;
			}

			// Extract the depth and stencil bits from the chosen format
			PIXELFORMATDESCRIPTOR obtainedFormat;
			obtainedFormat.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
			obtainedFormat.nVersion = 1;
			DescribePixelFormat(m_hdc, formatID, sizeof(PIXELFORMATDESCRIPTOR), &obtainedFormat);
			m_settings.bitsPerPixel = obtainedFormat.cColorBits;
			m_settings.depthBits    = descriptor.cDepthBits;
			m_settings.stencilBits  = descriptor.cStencilBits;

			// Set the chosen pixel format
			if (!SetPixelFormat(m_hdc, formatID, &obtainedFormat))
			{
				fw::WapiPrintLastError(fw_log,to set pixel format for device context -- cannot create OpenGL context);
				return false;
			}

			return true;
		}
	}
}
