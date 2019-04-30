//////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2014 Frontier (fr0nt13r789@github.c/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
//////////////////////////////////////////////////////////////////////////
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Window/Window.hpp>
#include <FRONTIER/System/util/OS.h>

#ifdef FRONTIER_OS_WINDOWS
	#include <FRONTIER/Window/Wapi/WapiWindow.hpp>
	#include "Wapi/WapiGetLastError.cpp"
	#include "Wapi/WapiWindow.cpp"
#elif defined(FRONTIER_OS_LINUX) && !defined(FRONTIER_OS_ANDROID)
	#include "Xlib/XlibWindow.cpp"
#endif

#ifndef FRONTIER_NO_WINDOW

namespace fw
{
	/////////////////////////////////////////////////////////////
	fm::Result Window::create(priv::WindowHandle windowHandle,fw::GLContext::Settings settings)
	{
		return this->fw::GLContext::create(windowHandle,settings);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::create(const fm::vec2s &size,fw::GLContext::Settings settings)
	{
		return this->fw::GLContext::create(size,settings);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::create(fw::GLContext::Settings settings)
	{
		return this->fw::GLContext::create(settings);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::destroy()
	{
		return this->fw::GLContext::destroy();
	}

	/////////////////////////////////////////////////////////////
	Window::Window() : m_window(new priv::Window)
	{

	}

	/////////////////////////////////////////////////////////////
	Window::Window(Window &&win) : m_window(nullptr)
	{
		win.swap(*this);
	}

	/////////////////////////////////////////////////////////////
	Window::Window(const fm::vec2i &pos,const fm::vec2u &size,const fm::String &title,fw::Window::WindowStyle style,Window *parent,Handle container,fw::GLContext::Settings settings) : m_window(new priv::Window)
	{
		open(pos,size,title,style,parent,container,settings);
	}

	/////////////////////////////////////////////////////////////
	Window::Window(const fm::vec2u &size,const fm::String &title,fw::Window::WindowStyle style,Window *parent,Handle container,fw::GLContext::Settings settings) : m_window(new priv::Window)
	{
		open(size,title,style,parent,container,settings);
	}

	/////////////////////////////////////////////////////////////
	Window::~Window()
	{
		delete m_window;
	}

	/////////////////////////////////////////////////////////////
	void Window::close()
	{
		beforeClose();
		this->destroy();
		m_window->close();
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::open(const fm::vec2i &pos,const fm::vec2u &size,const fm::String &title,fw::Window::WindowStyle style,Window *parent,Handle container,fw::GLContext::Settings settings)
	{
		fm::Result res;
		res += m_window->open(pos.x,pos.y,size.w,size.h,title,style,(parent ? &parent->getOSWindow() : nullptr),(priv::Window::Handle)container);
		res += this->create((priv::Window::Handle)m_window->getHandle(),settings);
		res += this->setActive();
		afterOpen();

		return res;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::open(const fm::vec2u &size,const fm::String &title,fw::Window::WindowStyle style,Window *parent,Handle container,fw::GLContext::Settings settings)
	{
		fm::Result res;
		res += m_window->open(size.w,size.h,title,style,(parent ? &parent->getOSWindow() : nullptr),(priv::Window::Handle)container);
		res += this->create((priv::Window::Handle)m_window->getHandle(),settings);
		res += this->setActive();
		afterOpen();

		return res;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::destroyContext()
	{
		return this->destroy();
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::createContext(fw::GLContext::Settings settings)
	{
		return this->create(settings);
	}

	/////////////////////////////////////////////////////////////
	bool Window::isOpen() const
	{
		return m_window->isOpen();
	}

	/////////////////////////////////////////////////////////////
	void Window::minimize()
	{
		m_window->minimize();
	}

	/////////////////////////////////////////////////////////////
	bool Window::isMinimized() const
	{
		return m_window->isMinimized();
	}

	/////////////////////////////////////////////////////////////
	void Window::maximize()
	{
		m_window->maximize();
	}

	/////////////////////////////////////////////////////////////
	bool Window::isMaximized() const
	{
		return m_window->isMaximized();
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::setFocus()
	{
		fm::Result res;
		res += m_window->setActive();
		res += setContextActive();

		return res;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::setContextActive(bool active)
	{
		return this->setActive(active);
	}

	/////////////////////////////////////////////////////////////
	void Window::showCursor(bool show)
	{
		m_window->showCursor(show);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::setRect(const fm::vec2i &pos,const fm::vec2u &size)
	{
		return m_window->setRect(pos.x,pos.y,size.w,size.h);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::getRect(fm::vec2i &pos,fm::vec2u &size)
	{
		return m_window->getRect(pos.x,pos.y,size.w,size.h);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::setPosition(fm::vec2i pos)
	{
		return m_window->setPosition(pos.x,pos.y);
	}

	/////////////////////////////////////////////////////////////
	fm::vec2i Window::getPosition() const
	{
		fm::vec2i ret;
		m_window->getPosition(ret.x,ret.y);
		return ret;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::setSize(const fm::vec2u &size)
	{
		return m_window->setSize(size.x,size.y);
	}

	/////////////////////////////////////////////////////////////
	fm::vec2u Window::getSize() const
	{
		fm::vec2u ret;
		m_window->getSize(ret.w,ret.h);
		return ret;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::setTitle(const fm::String &title)
	{
		return m_window->setTitle(title);
	}

	/////////////////////////////////////////////////////////////
	fm::String Window::getTitle() const
	{
		fm::String ret;
		m_window->getTitle(ret);
		return ret;
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::setVisible(bool visible)
	{
		return m_window->setVisible(visible);
	}

	/////////////////////////////////////////////////////////////
	bool Window::popEvent(Event &ev)
	{
		return m_window->popEvent(ev);
	}

	/////////////////////////////////////////////////////////////
	bool Window::waitEvent(Event &ev)
	{
		return m_window->waitEvent(ev);
	}

	/////////////////////////////////////////////////////////////
	void Window::postEvent(const Event &ev)
	{
		m_window->postEvent(ev);
	}
			
	/////////////////////////////////////////////////////////////
	void Window::enableDrop(bool enable)
	{
		m_window->enableDrop(enable);
	}
	
	/////////////////////////////////////////////////////////////
	bool Window::isDropEnabled() const
	{
		return m_window->isDropEnabled();
	}

	/////////////////////////////////////////////////////////////
	void Window::enableKeyRepeat(bool enable)
	{
		m_window->enableKeyRepeat(enable);
	}

	/////////////////////////////////////////////////////////////
	bool Window::isKeyRepeatEnabled() const
	{
		return m_window->isKeyRepeatEnabled();
	}

	/////////////////////////////////////////////////////////////
	void Window::enableResize(bool enable)
	{
		m_window->enableResize(enable);
	}

	/////////////////////////////////////////////////////////////
	bool Window::isResizeEnabled() const
	{
		return m_window->isResizeEnabled();
	}

	/////////////////////////////////////////////////////////////
	void Window::setIcon(const fg::Image &icon)
	{
		m_window->setIcon(icon);
	}

	/////////////////////////////////////////////////////////////
	Window::operator Window::Handle() const
	{
		return getHandle();
	}

	/////////////////////////////////////////////////////////////
	Window::Handle Window::getHandle() const
	{
		return m_window->getHandle();
	}

	/////////////////////////////////////////////////////////////
	void Window::setEventCallback(Window::EventCallback callback)
	{
		m_window->setEventCallback(callback);
	}

	/////////////////////////////////////////////////////////////
	fm::Result Window::swapBuffers()
	{
		return fw::GLContext::swapBuffers();
	}

	/////////////////////////////////////////////////////////////
	const priv::Window &Window::getOSWindow() const
	{
		return *m_window;
	}

	/////////////////////////////////////////////////////////////
	priv::Window &Window::getOSWindow()
	{
		return *m_window;
	}

	/////////////////////////////////////////////////////////////
	Window &Window::swap(Window &win)
	{
		std::swap(m_window,win.m_window);
		this->GLContext::swap(win);
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Window &Window::operator=(Window &&win)
	{
		return this->swap(win);
	}

	#define getByteRef(obj,i) (*(((unsigned char*)&obj)+i))

	/////////////////////////////////////////////////////////////
	Window::WindowStyle operator|(const Window::WindowStyle &left,const Window::WindowStyle &right)
	{
		Window::WindowStyle ret;
		for (int i=0;i<(int)sizeof(Window::WindowStyle);i++)
			getByteRef(ret,i) = getByteRef(left,i) | getByteRef(right,i);
		return ret;
	}

	/////////////////////////////////////////////////////////////
	Window::WindowStyle operator&(const Window::WindowStyle &left,const Window::WindowStyle &right)
	{
		Window::WindowStyle ret;
		for (int i=0;i<(int)sizeof(Window::WindowStyle);i++)
			getByteRef(ret,i) = getByteRef(left,i) & getByteRef(right,i);
		return ret;
	}

	/////////////////////////////////////////////////////////////
	Window::WindowStyle operator~(const Window::WindowStyle &style)
	{
		Window::WindowStyle ret;
		for (int i=0;i<(int)sizeof(Window::WindowStyle);i++)
			getByteRef(ret,i) = ~getByteRef(style,i);
		return ret;
	}
}

#endif // FRONTIER_NO_WINDOW
