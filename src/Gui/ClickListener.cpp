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
#include <FRONTIER/Gui/ClickListener.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	ClickListener::ClickListener() : m_dblClickLength(fm::milliseconds(1200))
	{
		for (auto &clk : m_clickClks) clk.setTime(m_dblClickLength + fm::seconds(1));
		for (auto &press : m_btnPressed) press = false;
		
		addEventHandler([&](fw::Event &ev) -> bool {
			
			if (ev.type == fw::Event::ButtonPressed)
			{
				fw::Mouse::Button btn = ev.mouse.button;
				fm::vec2 p = fm::vec2i(ev.mouse);
				
				if (!m_btnPressed[btn] && mouseInside())
				{
					m_btnPressed[btn] = true;
					
					onPress(btn,p);
					if (m_clickClks[btn].getTime() <= m_dblClickLength)
					{
						onDblClick(btn,p);
					}
					
					setActive();
					
					m_pressPoses[btn] = p;
					m_clickClks[btn].restart();
					
					return true;
				}
			}
			
			if (ev.type == fw::Event::MouseMoved)
			{
				bool mouseGrab = false;
				fm::vec2i p = ev.motion;
				
				for (fm::Size i=0;i<fw::Mouse::ButtonCount;++i)
				{
					if (m_btnPressed[i])
						mouseGrab = true;
				}
				
				if (mouseGrab)
				{
					onMouseMoved(p,m_prevPos);
					m_prevPos = p;
					
					return true;
				}
			}
			
			if (ev.type == fw::Event::ButtonReleased)
			{
				fw::Mouse::Button btn = ev.mouse.button;
				fm::vec2 p = fm::vec2i(ev.mouse);
				
				if (m_btnPressed[btn])
				{
					m_btnPressed[btn] = false;
					
					onRelease(btn,p);
					
					if (mouseInside())
						onClick(btn,p);
					
					setActive(false);
				}
			}
			
			return false;
		});
	}
	
	/////////////////////////////////////////////////////////////
	void ClickListener::setDoubleClickTime(fm::Time maxTime)
	{
		m_dblClickLength = maxTime;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Time ClickListener::getDoubleClickTime() const
	{
		return m_dblClickLength;
	}

	/////////////////////////////////////////////////////////////
	float ClickListener::getClickDistance(fw::Mouse::Button btn) const
	{
		return (m_pressPoses[btn] - getLastMousePos()).length();
	}
	
	/////////////////////////////////////////////////////////////
	void ClickListener::onPress(fw::Mouse::Button button,fm::vec2 p)
	{
		(void)button;
		(void)p;
	}
	
	/////////////////////////////////////////////////////////////
	void ClickListener::onRelease(fw::Mouse::Button button,fm::vec2 p)
	{
		(void)button;
		(void)p;
	}
	
	/////////////////////////////////////////////////////////////
	void ClickListener::onClick(fw::Mouse::Button button,fm::vec2 p)
	{
		(void)button;
		(void)p;
	}
	
	/////////////////////////////////////////////////////////////
	void ClickListener::onDblClick(fw::Mouse::Button button,fm::vec2 p)
	{
		(void)button;
		(void)p;
	}
	
	/////////////////////////////////////////////////////////////
	bool ClickListener::isPressed(fw::Mouse::Button button) const
	{
		return m_btnPressed[button];
	}
}
