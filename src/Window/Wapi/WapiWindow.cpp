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
#include <FRONTIER/Window/Wapi/WapiWindow.hpp>
#include <FRONTIER/System/macros/LOCK_MUTEX.h>
#include <FRONTIER/Graphics/Image.hpp>
#include <FRONTIER/Window/Window.hpp>
#include <algorithm>
/*
BOOL destroyAndPromptW(HWND h)
{
	fw::fw_log << "destroying " << h << std::endl;
	return DestroyWindow(h);
}

HWND WINAPI createAndPrimptWex(DWORD exStyles,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle,int x,int y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam)
{
	fw::fw_log << "excreating " << lpWindowName << std::endl;
	HWND ret = CreateWindowEx(exStyles,lpClassName,lpWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
	fw::fw_log << "created with id " << ret << std::endl;
	return ret;
}


#define DestroyWindow destroyAndPromptW
#define CreateWindowExA createAndPrimptWex
*/

#define WM_FACTIVATE_TREE (WM_USER + 0x0001)

namespace fw
{
	/////////////////////////////////////////////////////////////
	Keyboard::Key keyFromVK(LPARAM param,WPARAM lParam)
	{
		UINT scancode = (lParam & 0x00ff0000) >> 16;
		int  extended = (lParam & 0x01000000) != 0;

		if (param == VK_SHIFT)
			param = MapVirtualKey(scancode,3 /*MAPVK_VSC_TO_VK_EX*/);
		if (param == VK_CONTROL)
			param = extended ? VK_RCONTROL : VK_LCONTROL;
		if (param == VK_MENU)
			param = extended ? VK_RMENU : VK_LMENU;

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
		if (param == 0x6D) 		   return Keyboard::Minus;
		if (param == 0x6B) 		   return Keyboard::Plus;
		if (param == 0x6E) 		   return Keyboard::Comma;
		if (param == VK_TAB)       return Keyboard::Tab;
		if (param == VK_CAPITAL)   return Keyboard::CapsLock;
		if (param == VK_LSHIFT)    return Keyboard::LShift;
		if (param == VK_RSHIFT)    return Keyboard::RShift;
		if (param == VK_LCONTROL)  return Keyboard::LCtrl;
		if (param == VK_RCONTROL)  return Keyboard::RCtrl;
		if (param == VK_LWIN)      return Keyboard::LSuper;
		if (param == VK_RWIN)      return Keyboard::RSuper;
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
	bool adjustWindowSize(unsigned int &w,unsigned int &h,DWORD style,DWORD exStyle)
	{
		RECT Rct;
		Rct.top    = 100;
		Rct.left   = 100;
		Rct.right  = 150;
		Rct.bottom = 150;

		// convert window-rect to client-rect
		if (!AdjustWindowRectEx(&Rct,style,FALSE,exStyle))
			return false;

		w = (Rct.right  - Rct.left)-50+w;
		h = (Rct.bottom - Rct.top)-50+h;

		return true;
	}

	/////////////////////////////////////////////////////////////
	DWORD getDWORDfromStyle(unsigned int &style)
	{
		// menus have nothing
		if (style & Window::Menu)
			style = Window::Menu|Window::SkipTaskbar;

		// A resizeable window needs border
		if (style & Window::Resize)
			style |= Window::Border;

		// A x button needs titlebar
		if (style & Window::Close)
			style |= Window::Titlebar;

		// So does a minimize button
		if (style & Window::Minimize)
			style |= Window::Titlebar|Window::Close;

		// And a maximize button
		if (style & Window::Maximize)
			style |= Window::Titlebar|Window::Close;

		// A titlebar means border
		if (style & Window::Titlebar)
			style |= Window::Border;

		// convert the style to DWORD
		// so windows understands it too

		DWORD ret = WS_OVERLAPPED;
		if (style == Window::None || style & Window::Menu)
			ret |= WS_POPUP;

		if (style & Window::Close)
			ret |= WS_SYSMENU;

		if (style & Window::Border)
			ret |= WS_POPUP|WS_THICKFRAME;

		if (style & Window::Titlebar)
			ret |= WS_CAPTION;

		if (style & Window::Minimize)
			ret |= WS_MINIMIZEBOX;

		if (style & Window::Maximize)
			ret |= WS_MAXIMIZEBOX;


		if (style == Window::Border ||
			style == Window::None ||
			style & Window::Menu)
				ret &= ~WS_OVERLAPPED;

		return ret;
	}

	namespace Wapi
	{
		////////////////////////////////////////////////////////////
		bool Window::recFind(Wapi::Window *ancestor,HWND hwnd)
		{
			// check if it is the one
			if (ancestor->getHandle() == hwnd)
				return true;

			// recursive call on every children
			for (fm::Size i=0;i<ancestor->m_children.size();i++)
				if (recFind(ancestor->m_children[i],hwnd))
					return true;

			return false;
		}

		////////////////////////////////////////////////////////////
		LRESULT CALLBACK Window::forwardEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (msg == WM_NCCREATE)
			{
				// Associate the pointer with the HWND as userdata
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);

				return DefWindowProc(hwnd, msg, wParam, lParam); // Let windows do his thing
			}

			// ALT key or F10 would pause the execution
			if ((msg == WM_SYSCOMMAND) && (wParam == SC_KEYMENU))
				return 0;

			// extract the window pointer from user data
			Window *win = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);


			if (win)
				return win->handleEvent(hwnd,msg,wParam,lParam);


			// unhandled events are passed to windows
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		////////////////////////////////////////////////////////////
		LRESULT Window::handleEvent(HWND hwnd,UINT msg, WPARAM wParam, LPARAM lParam)
		{
			fm::IntPtr userResult;
			if (m_eventCallback && m_eventCallback(this,(unsigned int)msg,
														(fm::UintPtr)wParam,
														(fm::IntPtr)lParam,
														&userResult))
				return (LRESULT)userResult;

			switch(msg)
			{
				// our window got focus
				case WM_SETFOCUS:
				{
					postEvent(Event::FocusGained);
					return 0;
				}

				// our window lost focus
				case WM_KILLFOCUS:
				{
					postEvent(Event::FocusLost);
					return 0;
				}

				// our window was asked to close
				case WM_CLOSE:
				{
					postEvent(Event::Closed);
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

				// the mouse left the window
				case WM_MOUSELEAVE:
				{
					// post leave event
					m_cursorInside = false;
					Event ev(Event::MouseLeft);
					postEvent(ev);
					return 0;
				}

				// mouse moved
				case WM_MOUSEMOVE:
				{
					// if no button is down
					if (!(wParam & MK_LBUTTON ||
						  wParam & MK_MBUTTON ||
						  wParam & MK_RBUTTON))
					{
						// release the capture
						if (GetCapture() == m_hwnd)
							ReleaseCapture();
					}
					else if (GetCapture() != m_hwnd)
					{
						// else set capture to receive move event outside the window
						SetCapture(m_hwnd);
					}

					if (!m_cursorInside)
					{
						// ask windows to send notification when mouse leaves
						TRACKMOUSEEVENT tme;
						tme.cbSize = sizeof(TRACKMOUSEEVENT);
						tme.dwFlags = TME_LEAVE;
						tme.dwHoverTime = HOVER_DEFAULT;
						tme.hwndTrack = m_hwnd;
						TrackMouseEvent(&tme);

						// the mouse came inside
						m_cursorInside = true;

						SetCursor(LoadCursor(NULL,IDC_ARROW));

						Event ev(Event::MouseEntered);
						postEvent(ev);
					}

					// post move event
					Event ev(Event::MouseMoved);
					ev.motion.x = ((int)(short)LOWORD(lParam));
					ev.motion.y = ((int)(short)HIWORD(lParam));
					postEvent(ev);

					return 0;
				}

				// key pushed down
				case WM_SYSKEYDOWN:
				case WM_KEYDOWN:
				{
					// manually filter keyrepeat
					if (!m_enableRepeat)
					{
						if (bool(lParam & 1l << 30l))
							return 0;
					}
					
					Event ev(Event::KeyPressed);
					ev.key.code  = keyFromVK(wParam,lParam);
					ev.key.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000);
					ev.key.alt   = (GetKeyState(VK_MENU)    & 0x8000);
					ev.key.shift = (GetKeyState(VK_SHIFT)   & 0x8000);
					postEvent(ev);
					return 0;
				}

				// key released up
				case WM_SYSKEYUP:
				case WM_KEYUP:
				{
					// remember to reset the last pressed key when released
					Event ev(Event::KeyReleased);
					ev.key.code  = keyFromVK(wParam,lParam);
					ev.key.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000);
					ev.key.alt   = (GetKeyState(VK_MENU)    & 0x8000);
					ev.key.shift = (GetKeyState(VK_SHIFT)   & 0x8000);
					postEvent(ev);
					return 0;
				}

				// mouse button pressed
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_MBUTTONDOWN:
				{
					Event ev(Event::ButtonPressed);

					if (msg==WM_LBUTTONDOWN)
						ev.mouse.button = Mouse::Left;
					if (msg==WM_RBUTTONDOWN)
						ev.mouse.button = Mouse::Right;
					if (msg==WM_MBUTTONDOWN)
						ev.mouse.button = Mouse::Middle;

					ev.mouse.x = ((int)(short)LOWORD(lParam));
					ev.mouse.y = ((int)(short)HIWORD(lParam));
					postEvent(ev);
					return 0;
				}

				// mouse button released
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
				case WM_MBUTTONUP:
				{
					// post a move event for the item under the captured cursor
					if (GetCapture() == m_hwnd)
					{
						ReleaseCapture();
						DefWindowProc(m_hwnd,WM_MOUSEMOVE,0,lParam);
					}


					Event ev(Event::ButtonReleased);

					if (msg==WM_LBUTTONUP)
						ev.mouse.button = Mouse::Left;
					if (msg==WM_RBUTTONUP)
						ev.mouse.button = Mouse::Right;
					if (msg==WM_MBUTTONUP)
						ev.mouse.button = Mouse::Middle;

					ev.mouse.x =  ((int)(short)LOWORD(lParam));
					ev.mouse.y =  ((int)(short)HIWORD(lParam));
					postEvent(ev);
					return 0;
				}

				// area-hit test
				case WM_NCHITTEST:
				{
					LRESULT defResult = DefWindowProc(hwnd, msg, wParam, lParam);

					// disable parts where the user could resize the window if needed
					if (!m_resizeable)
						if (defResult == HTBOTTOM ||
							defResult == HTBOTTOMLEFT ||
							defResult == HTBOTTOMRIGHT ||
							defResult == HTLEFT ||
							defResult == HTRIGHT ||
							defResult == HTTOP ||
							defResult == HTTOPLEFT ||
							defResult == HTTOPRIGHT)
								defResult = HTBORDER;


					if (!m_cursorHitTest) // if the user didn't specify a function
						return defResult;
					else // if the user did specify a function
					{
						// wheres the cursor
						POINT mousePos;
						mousePos.x = ((int)(short)LOWORD(lParam));
						mousePos.y = ((int)(short)HIWORD(lParam));

						// get window's bounding rectangle
						RECT windowRect;
						if (!GetWindowRect(hwnd,&windowRect))
							return defResult;

						// get window's client area's bounding rectangle
						RECT clientRect;
						if (!GetClientRect(hwnd,&clientRect))
							return defResult;

						// and call the user defined function
						return m_cursorHitTest(mousePos,windowRect,clientRect,m_resizeable,defResult);
					}
				}

				// decoration hit
				case WM_NCLBUTTONDBLCLK:
				case WM_NCRBUTTONDBLCLK:
				case WM_NCLBUTTONDOWN:
				case WM_NCRBUTTONDOWN:
				case WM_NCLBUTTONUP:
				case WM_NCRBUTTONUP:
				{
					// prevent opening the window menu by clicking on the icon
					if (wParam==HTSYSMENU)
						wParam = HTCAPTION;

					// forward maximazition to user
					if (m_resizeable && wParam==HTCAPTION && msg==WM_NCLBUTTONDBLCLK)
					{
						if (!isMaximized())
							maximize();
						else
							ShowWindow(hwnd,SW_RESTORE);

						return 0;
					}

					// prevent opening the window menu by right clicking on the caption
					if ((wParam==HTCAPTION || wParam==HTCLOSE || wParam==HTMINBUTTON || wParam==HTMAXBUTTON) &&
						(msg==WM_NCRBUTTONUP || msg==WM_NCRBUTTONDOWN))
						return 0;

					// let windows do his business
					return DefWindowProc(hwnd, msg, wParam, lParam);
				}

				// resize event
				case WM_SIZE:
				{
				    // window is resized not moved
					m_windowMoved = false;

					// if the user is not sizing by hand
					if (wParam != SIZE_MINIMIZED)
					{
						Event ev(Event::Resized);
						ev.size.w = LOWORD(lParam);
						ev.size.h = HIWORD(lParam);
						postEvent(ev);
						break;
					}

					return DefWindowProc(hwnd, msg, wParam, lParam);
				}

				// the window is being moved
				case WM_MOVE:
				{
					// window is moved not resized
					m_windowMoved = true;

					return DefWindowProc(hwnd, msg, wParam, lParam);
				}

				// the user started to resize the window
				case WM_ENTERSIZEMOVE:
				{
					if (!m_windowMoved)
					{
						unsigned int w,h;
						getSize(w,h);
						Event ev(Event::Resized);
						ev.size.w = w;
						ev.size.h = h;
						postEvent(ev);
						break;
					}

					return DefWindowProc(hwnd, msg, wParam, lParam);
				}

				// the user finished resizing the window
				case WM_EXITSIZEMOVE:
				{
					m_windowMoved = false;

					return DefWindowProc(hwnd, msg, wParam, lParam);
				}

				// character entered
				case WM_CHAR:
				{
					Event ev(Event::TextEntered);
					ev.text.character = (char)wParam;
					ev.text.utf8character = (wchar_t)wParam;
					postEvent(ev);

					return 0;
				}

				// window-button pressed
				case WM_SYSCOMMAND:
				{
					// minimization request
					if (wParam == SC_MINIMIZE)
					{
						minimize();
						return 0;
					}

					// maximization request
					if (wParam == SC_MAXIMIZE)
					{
						maximize();
						return 0;
					}

					return DefWindowProc(hwnd, msg, wParam, lParam);
				}

				// this trick lets the window be bigger than the desktop
				case WM_GETMINMAXINFO:
				{
					DefWindowProc(hwnd, msg, wParam, lParam);
					MINMAXINFO *pmmi = (MINMAXINFO*)lParam;
					pmmi->ptMaxTrackSize.x = 420000;
					pmmi->ptMaxTrackSize.y = 420000;
					/*
					if (!m_resizeable)
					{
						// get client size
						unsigned int w,h;
						getSize(w,h);

						RECT Rct;
						Rct.top    = 100;
						Rct.left   = 100;
						Rct.right  = 150;
						Rct.bottom = 150;

						// get the decoration size
						if (!AdjustWindowRect(&Rct,m_style,FALSE))
							return false;

						w += (Rct.right  - Rct.left)-50;
						h += (Rct.bottom - Rct.top)-50;

						pmmi->ptMinTrackSize.x = w;
						pmmi->ptMaxTrackSize.x = w;

						pmmi->ptMinTrackSize.y = h;
						pmmi->ptMaxTrackSize.y = h;
					}*/

					return 0;
				}

				// mouse wheel moved
				case WM_MOUSEWHEEL:
				{
					fm::vec2i pos = Mouse::getPosition(*this);
					Event ev(Event::MouseWheelMoved);
					ev.wheel.delta = GET_WHEEL_DELTA_WPARAM(wParam)/float(WHEEL_DELTA);
					ev.wheel.ctrl  = (GetKeyState(VK_CONTROL) & 0x8000);
					ev.wheel.alt   = (GetKeyState(VK_MENU)    & 0x8000);
					ev.wheel.shift = (GetKeyState(VK_SHIFT)   & 0x8000);
					ev.wheel.x     = pos.x;
					ev.wheel.y     = pos.y;
					postEvent(ev);
					return 0;
				}

				// file drop
				case WM_DROPFILES:
				{
					HDROP hDrop = (HDROP)wParam;

					// query the number of files
					unsigned int fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

					Event ev(Event::FileDrop);
					fm::vec2i pos = Mouse::getPosition(*this);
					ev.drop.x = pos.x;
					ev.drop.y = pos.y;

					for (unsigned int i=0;i<fileCount;i++)
					{
						unsigned int bufSize = DragQueryFile(hDrop,i,NULL,0);

						char *buf = new char[bufSize+1];

						DragQueryFile(hDrop,i,buf,bufSize+1);

						buf[bufSize] = '\0';

						ev.drop.files.push_back(std::string(buf));

						delete[] buf;
					}
					postEvent(ev);
					return 0;
				}

				// decoration switched to "active" state
				case WM_NCACTIVATE:
				{
					Wapi::Window *parent = this;
					while (parent->m_parent)
						parent = parent->m_parent;

					bool inTree = recFind(parent,(HWND)lParam);

					if (inTree)
						wParam = 1;
					else
						SendMessage(parent->getHandle(),WM_FACTIVATE_TREE,wParam,lParam);

					m_decorActive = wParam;
				}

				// custom message (WM_USER+1)
				case WM_FACTIVATE_TREE:
				{
					// forward to children
					for (fm::Size i=0;i<m_children.size();i++)
						SendMessage(m_children[i]->getHandle(),WM_FACTIVATE_TREE,wParam,lParam);


					// save decor state
					m_decorActive = wParam;

					return DefWindowProc(hwnd, WM_NCACTIVATE, wParam, lParam);
				}

				// by default let windows handle it
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}

			// shouldn't be reached (unless break)
			return 0;
		}

		////////////////////////////////////////////////////////////
		unsigned int Window::m_windowCount = 0;

	#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		////////////////////////////////////////////////////////////
		fm::Mutex Window::m_windowCountMutex;
	#endif


		////////////////////////////////////////////////////////////
		Window::Window() : m_hwnd(NULL),
						   m_showCursor(true),
						   m_resizeable(true),
						   m_enableRepeat(false),
						   m_cursorInside(false),
						   m_acceptDrop(false),
						   m_icon(NULL),
						   m_isOpened(false),
						   m_eventCallback(NULL),
						   m_ownedParent(NULL),
						   m_parent(NULL),
						   m_decorActive(false),
						   m_windowMoved(false),
						   m_cursorHitTest(NULL)
		{

		}

		////////////////////////////////////////////////////////////
		Window::Window(int x,int y,unsigned int w,unsigned int h,const fm::String &title,unsigned int style,Wapi::Window *parent,HWND container) : m_hwnd(NULL),
																																				   m_showCursor(true),
																																				   m_resizeable(true),
																																				   m_enableRepeat(false),
																																				   m_cursorInside(false),
																																				   m_acceptDrop(false),
																																				   m_icon(NULL),
																																				   m_isOpened(false),
																																				   m_eventCallback(NULL),
																																				   m_ownedParent(NULL),
																																				   m_parent(NULL),
																																				   m_decorActive(false),
																																				   m_windowMoved(false),
																																				   m_cursorHitTest(NULL)
		{
			open(x,y,w,h,title,style,parent,container);
		}

		////////////////////////////////////////////////////////////
		Window::~Window()
		{
			cleanUp();
		}

		////////////////////////////////////////////////////////////
		bool Window::cleanUp()
		{
			// destrox children properly
			std::deque<Wapi::Window *> children = m_children;
			fm::Size count = children.size();
			for (fm::Size i = 0;i<count;i++)
				children[i]->close();

			m_children.clear();

			// delete window
			if (m_hwnd)
			{
				DestroyWindow(m_hwnd);

				m_hwnd = NULL;
			}


			// decrease window count
			if (m_isOpened)
			{
				FRONTIER_LOCK_MUTEX(m_windowCountMutex);
				m_windowCount--;
				if (!m_windowCount) // unregister class if no more windows
					UnregisterClassA(FRONTIER_WINDOWS_CLASS_NAME, GetModuleHandle(NULL));

				FRONTIER_UNLOCK_MUTEX(m_windowCountMutex);

				m_isOpened = false;
			}

			// delete owned parent
			if (m_ownedParent)
			{
				DestroyWindow(m_ownedParent);

				m_ownedParent = NULL;
			}


			// unregister this from parent's children
			if (m_parent)
			{
				std::deque<Wapi::Window *>::iterator it = std::find(m_parent->m_children.begin(),m_parent->m_children.end(),this);
				if (it != m_parent->m_children.end())
					m_parent->m_children.erase(it);

				m_parent = NULL;
			}


			// destroy icon
			if (m_icon)
				DestroyIcon(m_icon),
				m_icon = NULL;

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::createClass()
		{
			WNDCLASS winClass;

			ZeroMemory(&winClass,sizeof(winClass));

			// Fill in the fields of the WNDCLASS
			winClass.style         = 0;
			winClass.lpfnWndProc   = Window::forwardEvent; // When a event occures this function is called
			winClass.cbClsExtra    = 0;
			winClass.cbWndExtra    = 0;
			winClass.hInstance     = GetModuleHandle(NULL);
			winClass.hIcon         = NULL;
			winClass.hCursor       = NULL;
			winClass.hbrBackground = NULL;
			winClass.lpszMenuName  = NULL;
			winClass.lpszClassName = FRONTIER_WINDOWS_CLASS_NAME; // The name of the class

			// Tell windows about the new class
			if (RegisterClassA(&winClass))
				return true;

			return false;
		}

		////////////////////////////////////////////////////////////
		bool Window::open(int x,int y,unsigned int w,unsigned int h,const fm::String &title,unsigned int style,Wapi::Window *parent,HWND container)
		{
			// clean our resources before (re)creating
			cleanUp();

			// reset values
			m_showCursor = true;
			m_enableRepeat = false;
			m_resizeable = true;

			// convert style to dword
			DWORD createStyle = getDWORDfromStyle(style);

			m_parent = parent;

			if (m_parent)
				m_parent->m_children.push_back(this);

			// if the window shouldn't appear on the taskbar (automatic for WS_EX_TOOLWINDOW)
			// create a hidden parent
			if ((style & fw::Window::SkipTaskbar) && !m_parent)
				m_ownedParent = CreateWindowExA(0,FRONTIER_WINDOWS_CLASS_NAME,"invisible window",0,0,0,1,1,NULL,NULL,NULL,NULL);

			// update windowCount
			FRONTIER_LOCK_MUTEX(m_windowCountMutex);
			if (m_windowCount==0)
				createClass();
			m_windowCount++;
			FRONTIER_UNLOCK_MUTEX(m_windowCountMutex);


			// initialize the window
			m_hwnd = CreateWindowExW(((style & fw::Window::Toolbar) ? WS_EX_TOOLWINDOW : 0),
									 FRONTIER_WINDOWS_WCLASS_NAME,
									 &title.wstr()[0],
									 createStyle,
									 x,y,10,10,
									 m_parent ? m_parent->getHandle() : m_ownedParent,
									 NULL,NULL,
									 this); // set createdata to 'this'

			// SetParent raises the window even if container is NULL
			if (container)
				SetParent(m_hwnd,container);

			// windows wouldn't let us initially create a window bigger than the screen
			setSize(w,h);

			// Note if the window is intended to be resizeable
			m_resizeable = (style & fw::Window::Resize);

			if(!m_hwnd)
			{
				// upon fail...
				m_isOpened = false;

				return false;
			}
			else
				m_isOpened = true;

			// note the window's style
			m_style   = GetWindowLongPtr(m_hwnd, GWL_STYLE);
			m_exStyle = GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);

			// go fullscreen if requested
			if (style & fw::Window::Fullscreen)
				setFullscreen(w,h);

			// enable drag 'n' drop
			DragAcceptFiles(m_hwnd,m_acceptDrop);


			// Tell windows to show our window if its not hidden
			if (!(style & fw::Window::Hidden))
				ShowWindow(m_hwnd,(style & fw::Window::Toolbar) ? SW_SHOWNOACTIVATE : SW_SHOW);


			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::open(unsigned int w,unsigned int h,const fm::String &title,unsigned int style,Wapi::Window *parent,HWND container)
		{
			int x,y;
			
			if (getDWORDfromStyle(style) & WS_OVERLAPPED)
			{
				x = CW_USEDEFAULT;
				y = CW_USEDEFAULT;
			}
			else
			{
				x = GetSystemMetrics(SM_CXSCREEN)/2 - w/2;
				y = GetSystemMetrics(SM_CYSCREEN)/2 - h/2;
			}
			
			return open(x,y,w,h,title,style,parent,container);
		}

		////////////////////////////////////////////////////////////
		bool Window::isOpen() const
		{
			return m_hwnd!=NULL;
		}

		////////////////////////////////////////////////////////////
		void Window::close()
		{
			cleanUp();

			// Empty the event queue
			m_eventQueue.clear();
		}

		/////////////////////////////////////////////////////////////
		void Window::minimize()
		{
			if (m_hwnd)
				ShowWindow(m_hwnd,SW_SHOWMINIMIZED);
		}

		/////////////////////////////////////////////////////////////
		bool Window::isMinimized() const
		{
			if (!m_hwnd)
				return false;

			WINDOWPLACEMENT windowProp;
			windowProp.length = sizeof(WINDOWPLACEMENT);

			if (!GetWindowPlacement(m_hwnd,&windowProp))
				return false;

			return windowProp.showCmd == SW_SHOWMINIMIZED;
		}

		/////////////////////////////////////////////////////////////
		void Window::maximize()
		{
			if (m_hwnd)
			{
				ShowWindow(m_hwnd,SW_SHOWMAXIMIZED);

				Event ev(Event::Resized);
				getSize(ev.size.w,ev.size.h);
				postEvent(ev);
			}

		}

		/////////////////////////////////////////////////////////////
		bool Window::isMaximized() const
		{
			if (!m_hwnd)
				return false;

			WINDOWPLACEMENT windowProp;
			windowProp.length = sizeof(WINDOWPLACEMENT);

			if (!GetWindowPlacement(m_hwnd,&windowProp))
				return false;

			return windowProp.showCmd == SW_SHOWMAXIMIZED;
		}

		/////////////////////////////////////////////////////////////
		bool Window::setActive()
		{
			if (m_hwnd)
			{
				// bring the window to front
				if (!SetActiveWindow(m_hwnd))
					return false;

				// set keyboard focus
				if (!SetFocus(m_hwnd))
					return false;
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
		bool Window::setRect(int x,int y,unsigned int w,unsigned int h)
		{
			if (m_hwnd)
			{
				if (!adjustWindowSize(w,h,GetWindowLong(m_hwnd,GWL_STYLE),GetWindowLong(m_hwnd,GWL_EXSTYLE)))
					return false;

				if (!SetWindowPos(m_hwnd, // target HWND
								  NULL,   // Z-order specifier
								  x,y,    // new position
								  w,h,    // new size
								  SWP_NOREPOSITION|SWP_NOZORDER|SWP_NOACTIVATE))
						return false;
			}

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getRect(int &x,int &y,unsigned int &w,unsigned int &h)
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
					return false;

				// transform it to screen coordinates
				if (!ClientToScreen(m_hwnd,(POINT*)&client_rect.left) || !ClientToScreen(m_hwnd,(POINT*)&client_rect.right))
					return false;

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
								  NULL,   // Z-order specifier
								  x,y,    // new position
								  0,0,    // new size                  (ignored because of SWP_NOSIZE)
								  SWP_NOREPOSITION|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE))
						return false;

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getPosition(int &x,int &y) const
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
					return false;

				// transform it to screen coordinates
				if (!ClientToScreen(m_hwnd,(POINT*)&client_rect.left) || !ClientToScreen(m_hwnd,(POINT*)&client_rect.right))
					return false;

				x = client_rect.left;
				y = client_rect.top;
			}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::setSize(unsigned int w,unsigned int h)
		{
			if (m_hwnd)
			{
				if (!adjustWindowSize(w,h,GetWindowLong(m_hwnd,GWL_STYLE),GetWindowLong(m_hwnd,GWL_EXSTYLE)))
					return false;

				if (!SetWindowPos(m_hwnd, // target HWND
								  NULL,   // Z-order specifier
								  0,0,    // new position (ignored because of SWP_NOMOVE)
								  w,h,    // new size
								  SWP_NOREPOSITION|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE))
						return false;

			}

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getSize(unsigned int &w,unsigned int &h) const
		{
			if (m_hwnd)
			{
				RECT client_rect;
				if (!GetClientRect(m_hwnd,&client_rect)) // retrieve client rect
					return false;

				w = client_rect.right;
				h = client_rect.bottom;
			}
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::setTitle(const fm::String &title)
		{
			if (m_hwnd)
				if (!SetWindowTextW(m_hwnd,&title.wstr()[0]))
					return false;

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getTitle(fm::String &title) const
		{
			if (m_hwnd)
			{
				unsigned int bufsize = GetWindowTextLength(m_hwnd) + 1;
				wchar_t *ret = new wchar_t[bufsize];

				int written = GetWindowTextW(m_hwnd, ret, bufsize);

				ret[written] = 0;

				if (!written)
				{
					 // delete allocated memory!
					delete ret;
					return false;
				}
				title = fm::String(ret);

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

			// pop the queue and return true
			if (!m_eventQueue.empty())
			{
				ev = m_eventQueue[0];
				m_eventQueue.pop_front();
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

			// loop waiting for an event
			while (m_eventQueue.empty())
			{
				MSG msg;
				// GetMessage suspends the thread until an event occures
				// (this event may not be a window event but a thread event that is the while loop for)
				if (GetMessage(&msg, NULL, 0, 0) == -1)
					return false;

				TranslateMessage(&msg);
				DispatchMessage (&msg);
			}

			ev = m_eventQueue[0];
			m_eventQueue.pop_front();
			return true;
		}

		////////////////////////////////////////////////////////////
		void Window::postEvent(const Event &ev)
		{
		    if (m_eventQueue.size() && m_eventQueue.back().type == fw::Event::Resized && ev.type == fw::Event::Resized)
            {
                m_eventQueue.pop_back();
            }

			m_eventQueue.resize(m_eventQueue.size()+1,ev);
			//m_eventQueue.push_back(ev);
		}

		/////////////////////////////////////////////////////////////
		void Window::enableDrop(bool enable)
		{
			m_acceptDrop = enable;
			if (m_hwnd)
				DragAcceptFiles(m_hwnd,m_acceptDrop);
		}

		/////////////////////////////////////////////////////////////
		bool Window::isDropEnabled() const
		{
			return m_acceptDrop;
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

		/////////////////////////////////////////////////////////////
		void Window::enableResize(bool enable)
		{
			m_resizeable = enable;
		}

		/////////////////////////////////////////////////////////////
		bool Window::isResizeEnabled() const
		{
			return m_resizeable;
		}


		////////////////////////////////////////////////////////////
		bool Window::setFullscreen(unsigned int width,unsigned int height,bool fullscreen)
		{
			if (m_hwnd)
			{
				if (fullscreen)
				{
					// assume the screen resolution
					if (!width || !height)
					{
						// find the monitor of the window
						HMONITOR hMonitor = MonitorFromWindow(m_hwnd,MONITOR_DEFAULTTONULL);
						MONITORINFO monInfo;
						monInfo.cbSize = sizeof(monInfo);

						if (!hMonitor)
							return false;

						if (!GetMonitorInfo(hMonitor,&monInfo))
							return false;

						// set width and height to match the monitor's
						width  = monInfo.rcMonitor.right  - monInfo.rcMonitor.left;
						height = monInfo.rcMonitor.bottom - monInfo.rcMonitor.top;
					}

					// take off the decor
					SetWindowLongPtr(m_hwnd, GWL_STYLE,(m_style | WS_VISIBLE) &~ (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU));

					// set topmost
					if (!SetWindowPos(m_hwnd, HWND_TOPMOST, 0,0,width,height,SWP_FRAMECHANGED | SWP_NOMOVE))
						return false;
				}
				else
				{
					// put decor back
					SetWindowLongPtr(m_hwnd, GWL_STYLE,m_style|WS_VISIBLE);


					if (!adjustWindowSize(width,height,m_style,m_exStyle))
						return false;

					// set notopmost
					if (!SetWindowPos(m_hwnd, HWND_NOTOPMOST,0,0,width,height,SWP_NOMOVE))
						return false;
				}

				return true;
			}
			return false;
		}

		/////////////////////////////////////////////////////////////
		void Window::setIcon(const fg::Image &icon)
		{
			if (m_hwnd)
			{
				// delete last icon
				if (m_icon)
					DestroyIcon(m_icon);

				std::vector<fg::Color> *iconPixels = (std::vector<fg::Color> *)&icon;

				fm::Size width  =  *((fm::Size *)(iconPixels+1));
				fm::Size height = *(((fm::Size *)(iconPixels+1))+1);
				fm::Size size   = width*height;

				std::vector<fg::Color> iconColors;

				if (size)
				{
					iconColors.reserve(size);
					iconColors.resize (size);

					// swap RGBA to BGRA
					for (fm::Size i=0;i<size;i++)
						iconColors[i] = (*iconPixels)[i],
						std::swap(iconColors[i].r,iconColors[i].b);
				}

				// convert to HICON
				m_icon = CreateIcon(NULL, width, height, 1, sizeof(fg::Color)*FRONTIER_BITS_PER_BYTE, NULL, (unsigned char*)&iconColors[0]);

				if (m_icon)
				{
					SendMessage(m_hwnd,WM_SETICON,ICON_SMALL,(LPARAM)m_icon);
					SendMessage(m_hwnd,WM_SETICON,ICON_BIG,(LPARAM)m_icon);
				}
			}
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

		/////////////////////////////////////////////////////////////
		void Window::setEventCallback(EventCallback callback)
		{
			m_eventCallback = callback;
		}
	}
}
