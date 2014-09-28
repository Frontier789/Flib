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
	DWORD getDWORDfromStyle(unsigned int &style)
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
				
				// area-hit test
				case WM_NCHITTEST:
				{
					if (!m_cursorHitTest) // if the user didn't specify a function 
					{
						if (!m_resizeable) // Only if we dont want resize
						{
							// ask windows what he thinks the cursor is on
							LRESULT defResult = DefWindowProc(hwnd, msg, wParam, lParam);
							
							// tell him he is wrong when he thinks 
							// the cursor is on something resizeable
							if (defResult == HTBOTTOM ||
								defResult == HTBOTTOMLEFT ||
								defResult == HTBOTTOMRIGHT ||
								defResult == HTLEFT ||
								defResult == HTRIGHT ||
								defResult == HTTOP ||
								defResult == HTTOPLEFT ||
								defResult == HTTOPRIGHT)
									return HTBORDER; 
									
							return defResult;
						}
					}
					else // if the user did specify a function 
					{
						// what windows would return
						LRESULT defRet = DefWindowProc(hwnd, msg, wParam, lParam);
						
						// wheres the cursor
						POINT mousePos;
						mousePos.x = ((int)(short)LOWORD(lParam)); 
						mousePos.y = ((int)(short)HIWORD(lParam));
						
						// get window's bounding rectangle
						RECT windowRect;
						if (!GetWindowRect(hwnd,&windowRect))
						{
							fw_log << "GetWindowRect failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
							return defRet;
						}
						
						// get window's client area's bounding rectangle
						RECT clientRect;
						if (!GetClientRect(hwnd,&clientRect))
						{
							fw_log << "GetClientRect failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
							return defRet;
						}
						
						// and call the user defined function
						return m_cursorHitTest(mousePos,windowRect,clientRect,m_resizeable,defRet);
					}
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
						   m_showCursor(true),
						   m_resizeable(true),
						   m_cursorHitTest(NULL)
		{
			
		}
		
		////////////////////////////////////////////////////////////
		Window::Window(int x,int y,int w,int h,const std::string &title,unsigned int style) : m_hwnd(NULL),
																							  m_showCursor(true),
																							  m_resizeable(true),
																							  m_cursorHitTest(NULL)
		{
			open(x,y,w,h,title,style);
		}
		
		////////////////////////////////////////////////////////////
		Window::Window(int x,int y,int w,int h,const std::wstring &title,unsigned int style) : m_hwnd(NULL),
																							   m_showCursor(true),
																							   m_resizeable(true),
																							   m_cursorHitTest(NULL)
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
		
		bool openME(int x,int y,int w,int h,const void *title,Window *win,unsigned int style,bool wideTitle,HWND &m_hwnd,unsigned int &m_windowCount,bool &m_resizeable)
		{
			DWORD createStyle = getDWORDfromStyle(style);
	
			if (!adjustWindowSize(w,h,createStyle))
				return false;
			
			if (!wideTitle)
			{
				// initialize the window
				m_hwnd = CreateWindowA(FRONTIER_WINDOWS_CLASS_NAME,
									   ((std::string*)title)->c_str(),
									   createStyle,
									   x,y,w,h,
									   NULL,NULL,NULL,
									   win); // set createdata to 'this'				
			}
			else
			{
				// initialize the window
				m_hwnd = CreateWindowW(FRONTIER_WINDOWS_WCLASS_NAME,
									   ((std::wstring*)title)->c_str(),
									   createStyle,
									   x,y,w,h,
									   NULL,NULL,NULL,
									   win); // set createdata to 'this'				
			}

			// If the window is intended to be resizeable we note it
			m_resizeable = (style & fw::Window::Resize);
			
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
		bool Window::open(int x,int y,int w,int h,const std::string &title,unsigned int style)
		{
			// clean our resources before recreating
			cleanUp();
			init();
			
			return openME(x,y,w,h,&title,this,style,false,m_hwnd,m_windowCount,m_resizeable);
		}
		
		////////////////////////////////////////////////////////////
		bool Window::open(int x,int y,int w,int h,const std::wstring &title,unsigned int style)
		{
			// clean our resources before recreating
			cleanUp();
			init();
			
			return openME(x,y,w,h,&title,this,style,true,m_hwnd,m_windowCount,m_resizeable);
		}
		
		////////////////////////////////////////////////////////////
		void Window::close()
		{
			cleanUp();
		}
		
		/////////////////////////////////////////////////////////////
		void Window::minimize()
		{
			if (m_hwnd)
				ShowWindow(m_hwnd,SW_SHOWMINIMIZED);
		}
		
		/////////////////////////////////////////////////////////////
		void Window::maximize()
		{
			if (m_hwnd)
				ShowWindow(m_hwnd,SW_SHOWMAXIMIZED);
		}
		
		/////////////////////////////////////////////////////////////
		bool Window::setActive()
		{
			if (m_hwnd)
			{
				// bring the window to front
				if (!SetActiveWindow(m_hwnd))
				{
					fw_log << "SetActiveWindow failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				
				// set keyboard focus
				if (!SetFocus(m_hwnd))
				{
					fw_log << "SetFocus failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
			}
			return true;
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
		bool Window::setTitle(const std::string &title)
		{
			if (m_hwnd)
				if (!SetWindowTextA(m_hwnd,title.c_str()))
				{
					fw_log << "SetWindowText failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::setTitle(const std::wstring &title)
		{
			if (m_hwnd)
				if (!SetWindowTextW(m_hwnd,title.c_str()))
				{
					fw_log << "SetWindowText failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::getTitle(std::string &title) const
		{
			if (m_hwnd)
			{
				unsigned int bufsize = GetWindowTextLength(m_hwnd) + 1;
				char *ret = new char[bufsize];
				if (!GetWindowTextA(m_hwnd, ret, bufsize))
				{
					 // we must delete allocated memory!
					delete ret;
					fw_log << "GetWindowText failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				title = std::string(ret);
				delete ret;		
			}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::getTitle(std::wstring &title) const
		{
			if (m_hwnd)
			{
				unsigned int bufsize = GetWindowTextLength(m_hwnd) + 1;
				wchar_t *ret = new wchar_t[bufsize];
				if (!GetWindowTextW(m_hwnd, ret, bufsize))
				{
					 // we must delete allocated memory!
					delete ret;
					fw_log << "GetWindowText failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				title = std::wstring(ret);
				delete ret;		
			}
			return true;
		}
		
		////////////////////////////////////////////////////////////
		void Window::setVisible(bool visible)
		{
			if (m_hwnd)
				ShowWindow(m_hwnd,visible ? SW_SHOW : SW_HIDE);
		}
		
		////////////////////////////////////////////////////////////
		void Window::setCursorHitTest(LRESULT (*hitTestFunc)(const POINT&,const RECT&,const RECT&,bool,const LRESULT&))
		{
			m_cursorHitTest = hitTestFunc;
		}
				
		////////////////////////////////////////////////////////////
		Window::operator HWND() const
		{
			return m_hwnd;
		}
		
		////////////////////////////////////////////////////////////
		HWND Window::getHandle() const
		{
			return m_hwnd;
		}
	}
}
