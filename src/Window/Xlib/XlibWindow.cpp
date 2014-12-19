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
						   m_disp(NULL)
		{

		}

		////////////////////////////////////////////////////////////
		Window::Window(int x,int y,unsigned int w,unsigned int h,const std::string &title,unsigned int style) : m_opened(false),
																												m_disp(NULL)
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
				XSelectInput(m_disp,m_win,ButtonPressMask);
				
				// tell him to show it
				XMapWindow(m_disp,m_win);

				// manually set title and position
				setTitle(title);
				setPosition(x,y);
				
				// tell X to do what we asked now
				XFlush(m_disp);

				m_opened = true;
				
				return true;
			}
			
			return false;
		}

		////////////////////////////////////////////////////////////
		bool Window::isOpen() const
		{
			return m_opened;
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
		void Window::processEvent(XEvent &xev)
		{
			// we can be sure that we only process the current window's events
			// because this function is only called internally on events 
			// retieved by XWindowEvent and XCheckWindowEvent

			// A mouse buttonwas pressed or mouse wheel was rolled
			if (xev.type == ButtonPress)
			{
				// mouse wheel
				if (xev.xbutton.button == Button4 || 
					xev.xbutton.button == Button5)
				{
					Event ev(Event::MouseWheelMoved);
					if (xev.xbutton.button == Button4)
						ev.wheel.delta = 1;
					if (xev.xbutton.button == Button5)
						ev.wheel.delta = -1;


					ev.mouse.x =  xev.xbutton.x;
					ev.mouse.y =  xev.xbutton.y;
					postEvent(ev);

					return;
				}

				Event ev(Event::ButtonPressed);

				if (xev.xbutton.button==Button1)
					ev.mouse.button = Mouse::Left;
				if (xev.xbutton.button==Button3)
					ev.mouse.button = Mouse::Right;
				if (xev.xbutton.button==Button2)
					ev.mouse.button = Mouse::Middle;

				ev.mouse.x = xev.xbutton.x;
				ev.mouse.y = xev.xbutton.y;
				postEvent(ev);
			}
		}
		
		////////////////////////////////////////////////////////////
		bool Window::popEvent(Event &ev)
		{
			if (!isOpen())
				return false;

			XEvent xev;
			while(XCheckWindowEvent(m_disp,m_win,-1,&xev))
				processEvent(xev);

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

			XEvent xev;
			XWindowEvent(m_disp,m_win,-1,&xev);
			processEvent(xev);

			if (!m_eventQueue.empty())
			{
				ev = m_eventQueue[0];
				m_eventQueue.pop_front();
				return true;
			}
			return false;
		}

		////////////////////////////////////////////////////////////
		void Window::postEvent(const Event &ev)
		{
			m_eventQueue.resize(m_eventQueue.size()+1,ev);
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
