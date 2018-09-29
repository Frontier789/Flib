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
#include <FRONTIER/Gui/GuiButton.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <iostream>
using namespace std;


namespace fgui
{
	
	/////////////////////////////////////////////////////////////
	void GuiButton::applyState(InnerState)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiButton::GuiButton(GuiContext &cont) : GuiElement(cont),
											 m_text(cont)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	GuiButton::GuiButton(GuiContext &cont,const fm::String &text,fm::Delegate<void,GuiButton &> callback) : GuiElement(cont),
																											m_text(cont,text)
	{
		setSize(m_text.getSize());
		setCallback(callback);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::onDraw(fg::ShaderManager &shader)
	{
		m_text.onDraw(shader);
	}

	/////////////////////////////////////////////////////////////
	void GuiButton::onUpdate(const fm::Time &/* dt */)
	{
		fm::vec2s textSize = m_text.getSize();
		
		if (m_prevTextSize != textSize)
		{
			m_prevTextSize = textSize;
			
			setSize(getSize());
		}
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2s GuiButton::getSize() const
	{
		fm::vec2s size = GuiElement::getSize();
		
		fm::vec2 s = m_text.getSize();
		
		if (size.w < s.w) size.w = s.w;
		if (size.h < s.h) size.h = s.h;
		
		return size;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::setSize(fm::vec2s size)
	{
		fm::vec2 s = m_text.getSize();
		
		if (size.w < s.w) size.w = s.w;
		if (size.h < s.h) size.h = s.h;
		
		m_text.setPosition(getPosition() + size / 2 - m_text.getSize() / 2);
		
		GuiElement::setSize(size);
		
		recheckMouseInside();
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::setPosition(fm::vec2i pos)
	{
		m_text.setPosition(pos + getSize() / 2 - m_text.getSize() / 2);
		
		GuiElement::setPosition(pos);
		
		recheckMouseInside();
	}
	
	/////////////////////////////////////////////////////////////
	GuiText &GuiButton::getGuiText()
	{
		return m_text;
	}
	
	/////////////////////////////////////////////////////////////
	const GuiText &GuiButton::getGuiText() const
	{
		return m_text;
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::setCharacterSize(fm::Size characterSize)
	{
		m_text.setCharacterSize(characterSize);
		
		setSize(getSize());
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::setText(const fm::String &text,bool adaptSizeToText)
	{
		m_text.setText(text);
		
		if (!adaptSizeToText)
		{
			setSize(getSize());
		}
		else
		{
			setSize(fm::vec2s(1,1));
		}
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::traverseHierarchy(fm::Delegate<void,GuiElement &> func)
	{
		func(m_text);
		func(*this);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::onMouseEnter(fm::vec2)
	{
		if (!isPressed(fw::Mouse::Left))
			applyState(Hovered);
		else
			applyState(Pressed);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::onMouseLeave(fm::vec2)
	{
		applyState(Normal);
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::onPress(fw::Mouse::Button button,fm::vec2)
	{
		if (button == fw::Mouse::Left)
		{
			applyState(Pressed);
			
			m_actionCb(*this,GuiButton::Press);
		}
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::onRelease(fw::Mouse::Button button,fm::vec2)
	{
		if (button == fw::Mouse::Left)
		{
			if (mouseInside())
				applyState(Hovered);
			else
				applyState(Normal);	
			
			m_actionCb(*this,GuiButton::Release);		
		}
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::onClick(fw::Mouse::Button button,fm::vec2)
	{
		if (button == fw::Mouse::Left)
		{
			m_actionCb(*this,GuiButton::Click);
			
			callCallback();
		}
	}
	
	/////////////////////////////////////////////////////////////
	void GuiButton::onMouseMove(fm::vec2,fm::vec2)
	{
		m_actionCb(*this,GuiButton::Drag);
	}
}
