#include <FRONTIER/Window/Wapi/WapiWindow.hpp>
#include <FRONTIER/Window/Window.hpp>
#include <FRONTIER/Window/FwLog.hpp>
#include <string>
#include <iostream>

namespace fw
{
	/////////////////////////////////////////////////////////////
	Keyboard::Key keyFromVK(unsigned int param)
	{
		if (param == VK_LEFT)      return Keyboard::Left;
		if (param == VK_RIGHT)     return Keyboard::Right;
		if (param == VK_DOWN)      return Keyboard::Down;
		if (param == VK_UP)        return Keyboard::Up;
		if (param == VK_ESCAPE)    return Keyboard::Escape;
		if (param == VK_RETURN)    return Keyboard::Enter;
		if (param == VK_SNAPSHOT)  return Keyboard::PrintScreen;
		if (param == VK_SCROLL)    return Keyboard::ScrollLock;
		if (param == VK_PAUSE)     return Keyboard::PauseBreak;
		if (param == VK_BACK)      return Keyboard::Backspace;
		if (param == VK_INSERT)    return Keyboard::Insert;
		if (param == VK_DELETE)    return Keyboard::Delete;
		if (param == VK_HOME)      return Keyboard::Home;
		if (param == VK_END)       return Keyboard::End;
		if (param == VK_PRIOR)     return Keyboard::PageUp;
		if (param == VK_NEXT)      return Keyboard::PageDown;
		if (param == VK_DIVIDE)    return Keyboard::Divide;
		if (param == VK_MULTIPLY)  return Keyboard::Multiply;
		if (param == VK_OEM_MINUS) return Keyboard::Minus;
		if (param == VK_OEM_PLUS)  return Keyboard::Plus;
		if (param == VK_OEM_COMMA) return Keyboard::Comma;
		if (param == VK_TAB)       return Keyboard::Tab;
		if (param == VK_CAPITAL)   return Keyboard::CapsLock;
		if (param == VK_LSHIFT)    return Keyboard::LShift;
		if (param == VK_RSHIFT)    return Keyboard::RShift;
		if (param == VK_LCONTROL)  return Keyboard::LCtrl;
		if (param == VK_RCONTROL)  return Keyboard::RCtrl;
		if (param == VK_LWIN)      return Keyboard::LWindows;
		if (param == VK_RWIN)      return Keyboard::RWindows;
		if (param == VK_PRINT)     return Keyboard::Print;
		if (param == VK_LMENU)     return Keyboard::LAlt;
		if (param == VK_RMENU)     return Keyboard::RAlt;
		if (param == VK_SPACE)     return Keyboard::Space;
		
		if (param>=0x30 && param<=0x39)
			return Keyboard::Key(Keyboard::Num0+(param-0x30));
		
		if (param>=VK_NUMPAD0 && param<=VK_NUMPAD9)
			return Keyboard::Key(Keyboard::Numpad0+(param-VK_NUMPAD0));
		
		if (param>=VK_F1 && param<=VK_F12)
			return Keyboard::Key(Keyboard::F1+(param-VK_F1));
		
		if (param>=0x41 && param<=0x5A)
			return Keyboard::Key(Keyboard::A+(param-0x41));
		
		return Keyboard::Unknown;
		
	}
	
	/////////////////////////////////////////////////////////////
	Mouse::Button buttonFromVK(unsigned int param)
	{
		if (param == VK_LBUTTON)  return Mouse::Left;
		if (param == VK_RBUTTON)  return Mouse::Right;
		if (param == VK_MBUTTON)  return Mouse::Middle;
		
		return Mouse::Unknown;
	}
	
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
				// our window got focus
				case WM_SETFOCUS:
				{
					Event ev;
					ev.type = Event::FocusGained;
					m_eventQueue.push(ev);
					return 0;
				}
				
				// our window lost focus
				case WM_KILLFOCUS:
				{
					Event ev;
					ev.type = Event::FocusLost;
					m_eventQueue.push(ev);
					return 0;	
				}
				
				// our window was asked to close
				case WM_CLOSE:
				{
					Event ev;
					ev.type = Event::Closed;
					m_eventQueue.push(ev);
					return 0;
				}
				
				// refresh cursor image
				case WM_SETCURSOR:
				{
					if (LOWORD(lParam) == HTCLIENT)
						if (!m_showCursor)
						{
							SetCursor(NULL);
							return TRUE;
						}

					return DefWindowProc(hwnd, msg, wParam, lParam);
				}
				
				// mouse moved
				case WM_MOUSEMOVE: 
				{
					Event ev;
					ev.type = Event::MouseMoved;
					ev.pos.x =  ((int)(short)LOWORD(lParam));
					ev.pos.y =  ((int)(short)HIWORD(lParam));
					m_eventQueue.push(ev);
					return 0;
				}
				
				// key pushed down
				case WM_SYSKEYDOWN:
				case WM_KEYDOWN:
				{
					// manually filter keyrepeat
					if (!m_enableRepeat)
					{
						if (m_lastDown == wParam) 
							return 0;
						else
							m_lastDown = wParam;
					}
					Event ev;
					ev.type = Event::KeyPressed;
					ev.key.code = keyFromVK(wParam);
					ev.key.ctrl  = GetKeyState(VK_CONTROL);
					ev.key.alt   = GetKeyState(VK_MENU);
					ev.key.shift = GetKeyState(VK_SHIFT);
					m_eventQueue.push(ev);
					return 0;
				}
				
				// key released up
				case WM_SYSKEYUP:
				case WM_KEYUP:
				{
					// remember to reset the last pressed key when released
					m_lastDown = 0;
					Event ev;
					ev.type = Event::KeyReleased;
					ev.key.code  = keyFromVK(wParam);
					ev.key.ctrl  = GetKeyState(VK_CONTROL);
					ev.key.alt   = GetKeyState(VK_MENU);
					ev.key.shift = GetKeyState(VK_SHIFT);
					m_eventQueue.push(ev);
					return 0;
				}
				
				// mouse button pressed
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_MBUTTONDOWN:
				{
					Event ev;
					ev.type = Event::ButtonPressed;
					if (msg==WM_LBUTTONDOWN)
						ev.mouse.button = Mouse::Left;
					if (msg==WM_RBUTTONDOWN)
						ev.mouse.button = Mouse::Right;
					if (msg==WM_MBUTTONDOWN)
						ev.mouse.button = Mouse::Middle;
					ev.mouse.x =  ((int)(short)LOWORD(lParam));
					ev.mouse.y =  ((int)(short)HIWORD(lParam));
					m_eventQueue.push(ev);
					return 0;
				}
				
				// mouse button released
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
				case WM_MBUTTONUP:
				{
					Event ev;
					ev.type = Event::ButtonReleased;
					if (msg==WM_LBUTTONUP)
						ev.mouse.button = Mouse::Left;
					if (msg==WM_RBUTTONUP)
						ev.mouse.button = Mouse::Right;
					if (msg==WM_MBUTTONUP)
						ev.mouse.button = Mouse::Middle;
					ev.mouse.x =  ((int)(short)LOWORD(lParam));
					ev.mouse.y =  ((int)(short)HIWORD(lParam));
					m_eventQueue.push(ev);
					return 0;
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
						   m_enableRepeat(false),
						   m_lastDown(0),
						   m_cursorHitTest(NULL)
		{
			
		}
		
		////////////////////////////////////////////////////////////
		Window::Window(int x,int y,int w,int h,const std::string &title,unsigned int style) : m_hwnd(NULL),
																							  m_showCursor(true),
																							  m_resizeable(true),
																							  m_enableRepeat(false),
																							  m_lastDown(0),
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
	
			// set default cursor
			m_showCursor = true;
			
			// disable keyrepeat
			m_enableRepeat = false;
			
			// set last pressed key to unknown
			m_lastDown = 0;
			
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
			
			// Empty the event queue
			std::queue<Event> emptyEventQueue;
			std::swap(emptyEventQueue,m_eventQueue);
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
		bool Window::isOpen() const
		{
			return m_hwnd!=NULL;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::popEvent(Event &ev)
		{
			if (!m_hwnd)
				return false;
			
			MSG msg;
			
			// Prcess every pending message 
			while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage (&msg);			
			}
			
			// if we have an event to report
			// then return true and pop the queue
			if (!m_eventQueue.empty())
			{
				ev = m_eventQueue.front();
				m_eventQueue.pop();
				return true;
			}
			
			// otherwise return false;
			return false;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::waitEvent(Event &ev)
		{
			// In case the window is invalid a message would never appear
			if (!m_hwnd)
				return false;
				
			// we loop until we dont have a event
			while (m_eventQueue.empty())
			{
				MSG msg;
				// GetMessage suspends the thread until an event occures 
				// (this event may not be a window event but a thread event that is the while loop for)
				if (GetMessage(&msg, NULL, 0, 0)==-1)
				{
					fw_log << "GetMessage failed (lastError=\"" << WapiGetLastError() << "\")" << std::endl;
					return false;
				}
				TranslateMessage(&msg);
				DispatchMessage (&msg);
			}

			ev = m_eventQueue.front();
			m_eventQueue.pop();
			return true;
		}
		
		////////////////////////////////////////////////////////////
		void Window::enableKeyRepeat(bool enable)
		{
			m_enableRepeat = enable;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::isKeyRepeatEnabled() const
		{
			return m_enableRepeat;
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
