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
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/FrameBuffer.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Gui/GuiWindow.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/System/Rect.hpp>
#include <iostream>
using namespace std;

namespace fgui
{
	/////////////////////////////////////////////////////////////
	GuiWindow::GuiWindow()
	{
		
	}

	/////////////////////////////////////////////////////////////
	GuiWindow::GuiWindow(GuiWindow &&win)
	{
		this->swap(win);
	}

	/////////////////////////////////////////////////////////////
	GuiWindow::GuiWindow(const fm::vec2i &pos,const fm::vec2u &size,const fm::String &title,fw::Window::WindowStyle style,fw::GLContext::Settings settings) : 
		Window(pos,size,title,style,nullptr,0,settings),
		GuiContext(size)																																					
	{
		setClearColor(fm::vec4::White);
		setDepthTest(fg::LEqual);
	}

	/////////////////////////////////////////////////////////////
	GuiWindow::GuiWindow(const fm::vec2u &size,const fm::String &title,fw::Window::WindowStyle style,fw::GLContext::Settings settings) : 
		Window(size,title,style,nullptr,0,settings),
		GuiContext(size)
	{
		setClearColor(fm::vec4::White);
		setDepthTest(fg::LEqual);
	}
	
	/////////////////////////////////////////////////////////////	
	void GuiWindow::beforeClose()
	{
		freeGL();
	}
	
	/////////////////////////////////////////////////////////////	
	void GuiWindow::afterOpen()
	{
		initGL(getSize());
		
		setClearColor(fm::vec4::White);
		setDepthTest(fg::LEqual);
	}

	/////////////////////////////////////////////////////////////
	GuiWindow &GuiWindow::swap(GuiWindow &win)
	{
		this->Window::swap(win);
		this->GuiContext::swap(win);
		
		setupShader();
		win.setupShader();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	GuiWindow &GuiWindow::operator=(GuiWindow &&win)
	{
		return this->swap(win);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2s GuiWindow::getSize() const
	{
		return Window::getSize();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiWindow::setSize(fm::vec2s size)
	{
		Window::setSize(size);
		
		setupShader();
	}

	/////////////////////////////////////////////////////////////
	void GuiWindow::handleEvent(fw::Event &ev)
	{
		if (ev.type == fw::Event::Resized)
		{
			getShader().getCamera().setCanvasSize(getSize());
			fg::FrameBuffer::setViewport(fm::rect2s(fm::vec2s(),getSize()));
		}
		
		GuiContext::handleEvent(ev);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiWindow::drawElements()
	{
		GLContext::bindDefaultFrameBuffer();
		
		GuiContext::drawElements();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiWindow::handlePendingEvents()
	{
		fw::Event ev;
		while (popEvent(ev))
		{
			handleEvent(ev);
		}
	}

	/////////////////////////////////////////////////////////////
	void GuiWindow::drawElementsGuiLoop()
	{
		clear();

		drawElements();

		swapBuffers();
	}
}
