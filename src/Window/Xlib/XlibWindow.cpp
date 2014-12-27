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
#include <FRONTIER/Window/Xlib/XlibWindow.hpp>
#include <FRONTIER/Window/Window.hpp>
#include <FRONTIER/Window/FwLog.hpp>
#include <string>

namespace fw
{
	namespace Xlib 
	{
		/////////////////////////////////////////////////////////////
		Keyboard::Key keyFromKS(unsigned int param)
		{
			if (param == XK_Left)          return Keyboard::Left; 
			if (param == XK_Right)         return Keyboard::Right;  
			if (param == XK_Down)          return Keyboard::Down; 
			if (param == XK_Up)            return Keyboard::Up;
			if (param == XK_Escape)        return Keyboard::Escape;   
			if (param == XK_Return)        return Keyboard::Enter;   
			if (param == XK_Sys_Req)       return Keyboard::PrintScreen;    
			if (param == XK_Scroll_Lock)   return Keyboard::ScrollLock;        
			if (param == XK_Pause)         return Keyboard::PauseBreak;  
			if (param == XK_BackSpace)     return Keyboard::Backspace;      
			if (param == XK_Insert)        return Keyboard::Insert;   
			if (param == XK_Delete)        return Keyboard::Delete;   
			if (param == XK_Home)          return Keyboard::Home; 
			if (param == XK_End)           return Keyboard::End;
			if (param == XK_Page_Up)       return Keyboard::PageUp;    
			if (param == XK_Page_Down)     return Keyboard::PageDown;      
			if (param == XK_KP_Divide)     return Keyboard::Divide;      
			if (param == XK_KP_Multiply)   return Keyboard::Multiply;        
			if (param == XK_KP_Subtract)   return Keyboard::Minus;        
			if (param == XK_KP_Add)        return Keyboard::Plus;   
			if (param == XK_KP_Separator)  return Keyboard::Comma;         
			if (param == XK_Tab)           return Keyboard::Tab;
			if (param == XK_Caps_Lock)     return Keyboard::CapsLock;      
			if (param == XK_Shift_L)       return Keyboard::LShift;    
			if (param == XK_Shift_R)       return Keyboard::RShift;    
			if (param == XK_Control_L)     return Keyboard::LCtrl;      
			if (param == XK_Control_R)     return Keyboard::RCtrl;      
			if (param == XK_Super_L)       return Keyboard::LSuper;    
			if (param == XK_Super_R)       return Keyboard::RSuper;    
			if (param == XK_Print)         return Keyboard::Print;  
			if (param == XK_Alt_L)         return Keyboard::LAlt;  
			if (param == XK_Alt_R)         return Keyboard::RAlt;  
			if (param == XK_space)         return Keyboard::Space;
			
			if (param >= XK_0 && param <= XK_9)
				return Keyboard::Key(Keyboard::Num0+param-XK_0);

			if (param >= XK_KP_0 && param <= XK_KP_9)
				return Keyboard::Key(Keyboard::Numpad0+param-XK_KP_0);

			if (param >= XK_F1 && param <= XK_F12)
				return Keyboard::Key(Keyboard::F1+param-XK_F1);

			if (param >= XK_A && param <= XK_Z)
				return Keyboard::Key(Keyboard::A+param-XK_A);

			if (param >= XK_a && param <= XK_z)
				return Keyboard::Key(Keyboard::A+param-XK_a);

			return Keyboard::Unknown;

		}

		/////////////////////////////////////////////////////////////
		Mouse::Button buttonFromBn(unsigned int param)
		{
			if (param == Button1) return Mouse::Left;
			if (param == Button3) return Mouse::Right;
			if (param == Button2) return Mouse::Middle;

			return Mouse::Unknown;
		}

		/////////////////////////////////////////////////////////////
		void Window::getProperty(Atom *&atoms,unsigned long *count) const
		{
			Atom actual_type;
			int actual_format;
			unsigned long bytes_after;

			XGetWindowProperty(m_disp,m_win,
							   m_stateAtom,
							   0,1024,False,XA_ATOM, 
							   &actual_type,&actual_format,count,&bytes_after,
							   (unsigned char**)&atoms);
		}

		////////////////////////////////////////////////////////////
		void Window::processEvent(XEvent &xev)
		{
			// mouse got outside our window
			if (xev.type == LeaveNotify)
			{
				Event ev(Event::MouseLeft);
				postEvent(ev);
			}
			
			// mouse got inside our window
			if (xev.type == EnterNotify)
			{
				Event ev(Event::MouseEntered);
				postEvent(ev);
			}
			
			// gained focus
			if (xev.type == FocusIn)
			{
				Event ev(Event::FocusGained);
				postEvent(ev);
			}
			
			// lost focus
			if (xev.type == FocusOut)
			{
				Event ev(Event::FocusLost);
				postEvent(ev);
			}
			
			// the windows position/size/Z-order changed
			if (xev.type == ConfigureNotify)
			{
				// process only resize
				if (xev.xconfigure.width  != m_prevW || 
					xev.xconfigure.height != m_prevH)
				{
					m_prevW = xev.xconfigure.width;
					m_prevH = xev.xconfigure.height;
					
					Event ev(Event::Resized);
					ev.size.w = xev.xconfigure.width;
					ev.size.h = xev.xconfigure.height;
					postEvent(ev);
				}
			}

			// the window was asked to close
			if (xev.type == ClientMessage && (Atom)xev.xclient.data.l[0] == m_delAtom)
			{
				Event ev(Event::Closed);
				postEvent(ev);
			}

			// A mouse button was pressed or released or mouse wheel was rolled
			if (xev.type == ButtonPress || xev.type == ButtonRelease)
			{
				// mouse wheel
				if (xev.xbutton.button == Button4 || 
					xev.xbutton.button == Button5)
				{
					// handle it only once (since it gets posted as release too)
					if (xev.type == ButtonPress)
					{
						Event ev(Event::MouseWheelMoved);
						if (xev.xbutton.button == Button4)
							ev.wheel.delta = 1;
						if (xev.xbutton.button == Button5)
							ev.wheel.delta = -1;

						// get mod states
						ev.wheel.shift = fw::Keyboard::isKeyHeld(fw::Keyboard::LShift) || fw::Keyboard::isKeyHeld(fw::Keyboard::RShift);
						ev.wheel.ctrl  = fw::Keyboard::isKeyHeld(fw::Keyboard::LCtrl)  || fw::Keyboard::isKeyHeld(fw::Keyboard::RCtrl);
						ev.wheel.alt   = fw::Keyboard::isKeyHeld(fw::Keyboard::LAlt)   || fw::Keyboard::isKeyHeld(fw::Keyboard::RAlt);
						
						// copy position
						ev.wheel.x =  xev.xbutton.x;
						ev.wheel.y =  xev.xbutton.y;
						postEvent(ev);						
					}

					return;
				}

				Event ev(xev.type == ButtonPress ? Event::ButtonPressed : Event::ButtonReleased);

				ev.mouse.button = buttonFromBn(xev.xbutton.button);

				ev.mouse.x = xev.xbutton.x;
				ev.mouse.y = xev.xbutton.y;
				postEvent(ev);
			}

			// the mouse was moved
			if (xev.type == MotionNotify)
			{
				Event ev(Event::MouseMoved);
				ev.pos.x = xev.xmotion.x;
				ev.pos.y = xev.xmotion.y;
				postEvent(ev);
			}

			// key press or release
			if (xev.type == KeyPress || xev.type == KeyRelease)
			{
				// retrieve the keysym
				KeySym xkeysym;
				char c;
				if (XLookupString(&xev.xkey,&c,0,&xkeysym,0) == 1)
				{
					// if it was a release
					if (xev.type == KeyRelease)
					{
						// and the next event is a corresponding repeat-press
						if (XEventsQueued(m_disp,QueuedAfterReading))
						{
							XEvent nxev;
							XPeekEvent(m_disp, &nxev);

							if (nxev.type == KeyPress)
								if (nxev.xkey.time == xev.xkey.time &&
					    			nxev.xkey.keycode == xev.xkey.keycode)
									return; // then ignore it
						}
					}

					// if it is a repeat-press, ignore if repeat is disabled
					if (xkeysym == m_lastDown && !m_enableRepeat && xev.type == KeyPress)
						return; 

					// if it is the final release
					if (xev.type == KeyRelease)
						m_lastDown = XK_VoidSymbol; // reset the last_down
					else
						m_lastDown = xkeysym; // else note the pressed key
					
					Event ev(xev.type == KeyPress ? Event::KeyPressed : Event::KeyReleased);

					ev.key.code = keyFromKS(xkeysym);

					ev.key.shift = fw::Keyboard::isKeyHeld(fw::Keyboard::LShift) || fw::Keyboard::isKeyHeld(fw::Keyboard::RShift);
					ev.key.ctrl  = fw::Keyboard::isKeyHeld(fw::Keyboard::LCtrl)  || fw::Keyboard::isKeyHeld(fw::Keyboard::RCtrl);
					ev.key.alt   = fw::Keyboard::isKeyHeld(fw::Keyboard::LAlt)   || fw::Keyboard::isKeyHeld(fw::Keyboard::RAlt);
					
					postEvent(ev);
				}
			}
		}
		
		////////////////////////////////////////////////////////////
		bool Window::checkDisplay()
		{
			if (m_disp) 
				return true;

			m_disp = XOpenDisplay(NULL);

			if (!m_disp)
			{
				fw_log << "XOpenDisplay failed" << std::endl;
				return false;
			}

			m_rootWin = DefaultRootWindow(m_disp);

			return true;
		}

		////////////////////////////////////////////////////////////
		Window::Window() : m_opened(false),
						   m_enableRepeat(true),
						   m_lastDown(XK_VoidSymbol),
						   m_disp(NULL),
						   m_delAtom(0),
						   m_stateAtom(0),
						   m_prevW(0),
						   m_prevH(0),
						   m_stateHiddenAtom(0)
		{

		}

		////////////////////////////////////////////////////////////
		Window::Window(int x,int y,unsigned int w,unsigned int h,const std::string &title,unsigned int style) : m_opened(false),
																												m_enableRepeat(true),
																												m_lastDown(XK_VoidSymbol),
																												m_disp(NULL),
																												m_delAtom(0),
																												m_stateAtom(0),
																												m_prevW(0),
																												m_prevH(0),
																												m_stateHiddenAtom(0)
		{
			open(x,y,w,h,title,style);
		}

		////////////////////////////////////////////////////////////
		Window::~Window()
		{
			close();

			if (m_disp)
				XCloseDisplay(m_disp);
		}

		////////////////////////////////////////////////////////////
		bool Window::open(int x,int y,unsigned int w,unsigned int h,const std::string &title,unsigned int style)
		{
			if (!checkDisplay())
				return false;

			// close the window if we had it open
			close();

			// ask X to create a window
			m_win = XCreateSimpleWindow(m_disp,m_rootWin,x,y,w,h,0,BlackPixel(m_disp,0),BlackPixel(m_disp,0));
			
			if (m_win != (::Window)NULL)
			{
				m_opened = true;

				XSelectInput(m_disp,m_win,LeaveWindowMask|EnterWindowMask|FocusChangeMask|StructureNotifyMask|PointerMotionMask|ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask);

				// get atoms
				m_stateAtom       = XInternAtom(m_disp,"_NET_WM_STATE",False);
				m_stateHiddenAtom = XInternAtom(m_disp,"_NET_WM_STATE_HIDDEN",False);
				m_delAtom         = XInternAtom(m_disp,"WM_DELETE_WINDOW",False);

				// register deletion event
				XSetWMProtocols(m_disp,m_win,&m_delAtom,1);
				
				// tell X to show it
				XMapWindow(m_disp,m_win);

				// manually set title and position
				setTitle(title);
				setPosition(x,y);
				
				// reset storages
				enableKeyRepeat(false);
				m_lastDown = XK_VoidSymbol;
				m_prevW = 0;
				m_prevH = 0;

				// tell X to do what we asked, now
				XFlush(m_disp);
				
				return true;
			}
			
			return false;
		}

		////////////////////////////////////////////////////////////
		bool Window::isOpen() const
		{
			return m_opened;
		}

		/////////////////////////////////////////////////////////////
		void Window::minimize()
		{
			if (isOpen())
			{
				if (isMinimized())
					XMapWindow(m_disp,m_win);
				else
					XIconifyWindow(m_disp,m_win,XDefaultScreen(m_disp));
			}
		}

		/////////////////////////////////////////////////////////////
		bool Window::isMinimized() const
		{
			if (!isOpen())
				return false;

			unsigned long count;
			Atom *atoms = NULL;

			getProperty(atoms,&count);

			for(unsigned long i=0;i<count; i++) 
				if(atoms[i] == m_stateHiddenAtom) 
				{
					XFree(atoms);
					return true;
				}
			XFree(atoms);
			return false;
		}

		////////////////////////////////////////////////////////////
		void Window::close()
		{
			if (m_opened)
			{
				XDestroyWindow(m_disp,m_win);
				XFlush(m_disp);
				m_opened = false;

				// empty out event queue
				m_eventQueue.clear();
			}
		}

		////////////////////////////////////////////////////////////
		bool Window::setRect(int x,int y,unsigned int w,unsigned int h)
		{
			if (!isOpen())
				return false;

			// simply tell X to resize and move the window
			XMoveResizeWindow(m_disp,m_win,x,y,w,h);
			XFlush(m_disp);

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getRect(int &x,int &y,unsigned int &w,unsigned int &h)
		{
			if (!isOpen())
				return false;

			unsigned int border,depth;

			// simply retrieve the data
			XGetGeometry(m_disp,m_win,&m_rootWin,&x,&y,&w,&h,&border,&depth);

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::setPosition(int x,int y)
		{
			if (!isOpen())
				return false;

			// simply ask X to move the window
			XMoveWindow(m_disp,m_win,x,y);
			XFlush(m_disp);
			
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getPosition(int &x,int &y) const
		{
			if (!isOpen())
				return false;

			unsigned int border,depth;
			unsigned int w,h;

			// simply retrieve the data
			XGetGeometry(m_disp,m_win,&m_rootWin,&x,&y,&w,&h,&border,&depth);

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::setSize(unsigned int w,unsigned int h)
		{
			if (!isOpen())
				return false;

			// simply ask X to resize the window
			XResizeWindow(m_disp,m_win,w,h);
			XFlush(m_disp);
			
			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getSize(unsigned int &w,unsigned int &h) const
		{
			if (!isOpen())
				return false;

			int x,y;
			unsigned int border,depth;

			// simply retrieve the data
			XGetGeometry(m_disp,m_win,&m_rootWin,&x,&y,&w,&h,&border,&depth);

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::setTitle(const std::string &title)
		{
			if (!isOpen())
				return false;
			
			std::string s = title;
			char *c = &s[0];
			
			XStoreName(m_disp,m_win,c); 

			XFlush(m_disp);

			return true;
		}

		////////////////////////////////////////////////////////////
		bool Window::getTitle(std::string &title) const
		{
			if (!isOpen())
				return false;
			
			char *c;
			
			XFetchName(m_disp,m_win,&c);
			title = c;
			
			XFree(c); 

			XFlush(m_disp);

			return true;
		}
		
		////////////////////////////////////////////////////////////
		bool Window::popEvent(Event &ev)
		{
			if (!isOpen())
				return false;

			// Display is unique for every window therefore 
			// we dont have to bother about events that dont belong
			// to this window

			XEvent xev;
			while (XPending(m_disp))
			{
				XNextEvent(m_disp,&xev);

				// process only the current window's events
				if (xev.xany.window == m_win)
					processEvent(xev);
			}

			if (!m_eventQueue.empty())
			{
				ev = m_eventQueue[0];
				m_eventQueue.pop_front();
				return true;
			}
			return false;
		}

		////////////////////////////////////////////////////////////
		bool Window::waitEvent(Event &ev)
		{
			if (!isOpen())
				return false;

			// Display is unique for every window therefore 
			// we dont have to bother about events that dont belong
			// to this window

			// if we have event in stock we return it
			if (!m_eventQueue.empty())
			{
				ev = m_eventQueue[0];
				m_eventQueue.pop_front();
				return true;
			}

			// otherwise wait for one
			XEvent xev;
			while (m_eventQueue.empty())
			{
				XNextEvent(m_disp,&xev);

				// process only the current window's events
				if (xev.xany.window == m_win)
					processEvent(xev);
			}

			ev = m_eventQueue[0];
			m_eventQueue.pop_front();
			return true;
		}

		////////////////////////////////////////////////////////////
		void Window::postEvent(const Event &ev)
		{
			m_eventQueue.resize(m_eventQueue.size()+1,ev);
		}

		/////////////////////////////////////////////////////////////
		void Window::enableKeyRepeat(bool enable)
		{
			m_enableRepeat = enable;
		}

		/////////////////////////////////////////////////////////////
		bool Window::isKeyRepeatEnabled() const
		{
			return m_enableRepeat;
		}

		////////////////////////////////////////////////////////////
		::Window Window::getHandle() const
		{
			return m_win;
		}

		////////////////////////////////////////////////////////////
		Window::operator ::Window() const
		{
			return m_win;
		}
	}
}
