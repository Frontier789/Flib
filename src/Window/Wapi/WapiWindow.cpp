#include <FRONTIER/Window/Wapi/WapiWindow.hpp>
#include <FRONTIER/Window/Window.hpp>
#include <FRONTIER/Window/FwLog.hpp>
#include <string>
#include <iostream>

namespace fw
{
	/////////////////////////////////////////////////////////////
	std::string WapiGetLastError()
	{
		std::string ret;
		char *errorText; // FormatMessage allocates memory automatically for us
		DWORD err=GetLastError();
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL,
					   err,
					   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					   (LPTSTR)&errorText,0,NULL);
		if (errorText)
		{
			// if we got any description then we convert it to string
			ret=std::string(errorText);
			::LocalFree(errorText); //we free up the allocated memory
		}
		
		while (ret.length() && (ret[ret.length()-1]=='\t' || 
								ret[ret.length()-1]==' '  || 
								ret[ret.length()-1]=='\r' || 
								ret[ret.length()-1]=='\n') ) ret.resize(ret.length()-1);
		
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	bool adjustWindowSize(int &w,int &h,DWORD style)
	{
		RECT Rct;
		Rct.top    = 0;
		Rct.left   = 0;
		Rct.right  = w;
		Rct.bottom = h;
		
		// we want to specify the client rect not the whole window rect 
		if (!AdjustWindowRect(&Rct,style,FALSE))
		{
			fw_log << "AdjustWindowRect failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
			return false;
		}
		
		w = (Rct.right  - Rct.left);
		h = (Rct.bottom - Rct.top);
		
		return true;
	}
	
	/////////////////////////////////////////////////////////////
	DWORD getDWORDfromStyle(int style)
	{
		// A resizeable window needs border
		if (style & Window::Resize)  
			style |= Window::Border;
		
		// A close button can only be located in the titlebar
		if (style & Window::Close)
			style |= Window::Titlebar;
		
		// So does a minimize button
		if (style & Window::Minimize)
			style |= Window::Titlebar|Window::Close;

		// And a maximize button
		if (style & Window::Maximaze)
			style |= Window::Titlebar|Window::Close;
		
		// A titlebar means border
		if (style & Window::Titlebar)
			style |= Window::Border;
		
		
		// convert the style to DWORD
		// so windows understands it too
		
		DWORD ret = WS_OVERLAPPED;
		if (style & Window::None)
			ret |= WS_POPUP;
			
		if (style & Window::Close)
			ret |= WS_SYSMENU;
			
		if (style & Window::Border)
			ret |= WS_POPUP|WS_THICKFRAME;
			
		if (style & Window::Titlebar)
			ret |= WS_CAPTION;
		
		if (style & Window::Minimize)
			ret |= WS_MINIMIZEBOX;
		
		if (style & Window::Maximaze)
			ret |= WS_MAXIMIZEBOX;
		
		
		if (style == Window::Border && 
			style == Window::None)
				ret &= ~WS_OVERLAPPED;
		
		return ret;
	}

	namespace Wapi
	{
		////////////////////////////////////////////////////////////
		LRESULT CALLBACK Window::forwardEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			// Thankfully we recieve a pointer to the created window
			// beacuse we set it in CreateWindow
			if (msg == WM_NCCREATE) 
			{
				// Associate the pointer with the HWND as userdata
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
				
				return DefWindowProc(hwnd, msg, wParam, lParam); // Let windows do his thing
			}
			
			// ALT key or F10 would pause the execution so we prevent windows from handling them
			if ((msg == WM_SYSCOMMAND) && (wParam == SC_KEYMENU))
				return 0;
			
			// we now can extract our window pointer from user data
			Window *win = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			
			
			if (win)
				return win->handleEvent(hwnd,msg,wParam,lParam);
			
			
			// If we did not handle the event then we pass it to windows
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		
		////////////////////////////////////////////////////////////
		LRESULT Window::handleEvent(HWND hwnd,UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch(msg)
			{
				// refresh cursor image
				case WM_SETCURSOR:
				{
					if (LOWORD(lParam) == HTCLIENT)
						if (!m_showCursor)
						{
							SetCursor(NULL);
							return TRUE;
						}

					break;
				}
				
				// by default let windows handle it
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			return 0;
		}
		
		////////////////////////////////////////////////////////////
		unsigned int Window::m_windowCount = 0;

		////////////////////////////////////////////////////////////
		Window::Window() : m_hwnd(NULL),
						   m_showCursor(true)
		{
			
		}
		
		////////////////////////////////////////////////////////////
		Window::Window(int x,int y,int w,int h,const char *title,unsigned int style) : m_hwnd(NULL),
																					   m_showCursor(true)
		{
			open(x,y,w,h,title,style);
		}
		
		////////////////////////////////////////////////////////////
		Window::~Window()
		{
			cleanUp();
		}
		
		////////////////////////////////////////////////////////////
		bool Window::cleanUp()
		{
			if (m_hwnd) // if the window was valid we destroy it
			{
				if (DestroyWindow(m_hwnd))
				{
					m_hwnd = NULL;
					m_windowCount--;
			
					// If we dont have any windows left we unregister our window class
					if (!m_windowCount)
					{
						if (UnregisterClassA(FRONTIER_WINDOWS_CLASS_NAME, GetModuleHandle(NULL)))
							return true;
						else
						{
							fw_log << "UnregisterClassA failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
							return false;
						}
					}
				}
				else
				{
					fw_log << "DestroyWindow failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
			}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::init()
		{
			// having no window means we must register a WNDCLASS
			if (m_windowCount==0)
			{
				WNDCLASS winClass = {0};
		
				// Fill in the fields of the WNDCLASS
				winClass.style         = 0;
				winClass.lpfnWndProc   = Window::forwardEvent; // When a event occures this function is called
				winClass.cbClsExtra    = 0;
				winClass.cbWndExtra    = 0;
				winClass.hInstance     = GetModuleHandle(NULL);
				winClass.hIcon         = NULL;
				winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
				winClass.hbrBackground = 0;
				winClass.lpszMenuName  = NULL;
				winClass.lpszClassName = FRONTIER_WINDOWS_CLASS_NAME; // The name of the class
				
				// Tell windows we have a class
				if (RegisterClassA(&winClass))
					return true;
				else
				{
					fw_log << "RegisterClassA failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
			}
			
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::open(int x,int y,int w,int h,const char *title,unsigned int style)
		{
			// clean our resources before recreating
			cleanUp();
			init();
			
			DWORD createStyle = getDWORDfromStyle(style);
	
			if (!adjustWindowSize(w,h,createStyle))
				return false;
			
			// initialize the window
			m_hwnd = CreateWindowA(FRONTIER_WINDOWS_CLASS_NAME,
								   title,
								   createStyle,
								   x,y,w,h,
								   NULL,NULL,NULL,
								   this); // set createdata to 'this'
			
			if(!m_hwnd)
			{
				// yet again if we fail...
				fw_log << "CreateWindow failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
				return false;
			}
			
			// Tell windows to show our window
			ShowWindow(m_hwnd, SW_SHOW);
				
			m_windowCount++;
			
			return true;
		}
		
		////////////////////////////////////////////////////////////
		void Window::close()
		{
			cleanUp();
		}
		
		////////////////////////////////////////////////////////////
		void Window::showCursor(bool show)
		{
			m_showCursor = show;
			if (show)
				SetCursor(LoadCursor(NULL,IDC_ARROW));
			else
				SetCursor(NULL);
		}
		
		////////////////////////////////////////////////////////////
		bool Window::setRect(int x,int y,int w,int h)
		{
			if (m_hwnd)
				if (!SetWindowPos(m_hwnd, // target HWND
								  NULL,   // HWND to insert ours after (ignored because of SWP_NOREPOSITION)
								  x,y,    // new position
								  w,h,    // new size
								  SWP_NOREPOSITION|SWP_NOSIZE))
					{
						fw_log << "SetWindowPos failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
						return false;
					}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::getRect(int &x,int &y,int &w,int &h)
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
				{
					fw_log << "GetClientRect failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				
				// transform it to screen coordinates
				if (!ClientToScreen(m_hwnd,(POINT*)&client_rect.left) || !ClientToScreen(m_hwnd,(POINT*)&client_rect.right))
				{
					fw_log << "ClientToScreen failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				
				w = client_rect.right -client_rect.left;
				h = client_rect.bottom-client_rect.top;
				x = client_rect.left;
				y = client_rect.top;
			}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::setPosition(int x,int y)
		{
			if (m_hwnd)
				if (!SetWindowPos(m_hwnd, // target HWND
								  NULL,   // HWND to insert ours after (ignored because of SWP_NOREPOSITION)
								  x,y,    // new position
								  0,0,    // new size                  (ignored because of SWP_NOSIZE)
								  SWP_NOREPOSITION|SWP_NOSIZE))
					{
						fw_log << "SetWindowPos failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
						return false;
					}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::getPosition(int &x,int &y) const
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
				{
					fw_log << "GetClientRect failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				
				// transform it to screen coordinates
				if (!ClientToScreen(m_hwnd,(POINT*)&client_rect.left) || !ClientToScreen(m_hwnd,(POINT*)&client_rect.right))
				{
					fw_log << "ClientToScreen failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				
				x = client_rect.left;
				y = client_rect.top;
			}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::setSize(int w,int h)
		{
			if (m_hwnd)
			{
				if (!adjustWindowSize(w,h,GetWindowLong(m_hwnd,GWL_STYLE)))
					return false;
				
				if (!SetWindowPos(m_hwnd, // target HWND
								  NULL,   // HWND to insert ours after (ignored because of SWP_NOREPOSITION)
								  0,0,    // new position              (ignored because of SWP_NOMOVE)
								  w,h,    // new size                  
								  SWP_NOREPOSITION|SWP_NOMOVE))
					{
						fw_log << "SetWindowPos failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
						return false;
					}		
			}

			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::getSize(int &w,int &h) const
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
				{
					fw_log << "GetClientRect failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				
				// transform it to screen coordinates
				if (!ClientToScreen(m_hwnd,(POINT*)&client_rect.left) || !ClientToScreen(m_hwnd,(POINT*)&client_rect.right))
				{
					fw_log << "ClientToScreen failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				
				w = client_rect.right -client_rect.left;
				h = client_rect.bottom-client_rect.top;
			}
			return true;
		}

		////////////////////////////////////////////////////////////
		HWND Window::getHandle() const
		{
			return m_hwnd;
		}
		
		////////////////////////////////////////////////////////////
		Window::operator HWND() const
		{
			return m_hwnd;
		}
	}
}
