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
#include <FRONTIER/Gui/MouseMoveListener.hpp>
#include <FRONTIER/Window/Event.hpp>

namespace fgui
{
	MouseMoveListener::MouseMoveListener() : m_mouseIn(false)
	{
		addEventHandler([&](fw::Event &ev) -> bool {
			
			if (ev.type == fw::Event::MouseMoved)
			{
				fm::vec2i p = ev.motion;
				bool in = contains(p);
				
				if (m_mouseIn)
				{
					if (in)
					{
						onMouseMoved(p,m_prevPos);
						m_prevPos = p;
					}
					else
					{
						onMouseLeave(p);
					}
				}
				else
				{
					if (in)
					{
						onMouseEnter(p);
					}
				}
				
				m_mouseIn = in;
			}
			
			if (ev.type == fw::Event::ButtonReleased)
			{
				fm::vec2 p = ev.mouse;
				bool in = contains(p);
				
				if (!m_mouseIn && in)
				{
					onMouseEnter(p);
				}
			}
			
			return false;
		});
	}

	/////////////////////////////////////////////////////////////
	void MouseMoveListener::onMouseEnter(fm::vec2 p)
	{
		(void)p;
	}

	/////////////////////////////////////////////////////////////
	void MouseMoveListener::onMouseLeave(fm::vec2 p)
	{
		(void)p;
	}

	/////////////////////////////////////////////////////////////
	void MouseMoveListener::onMouseMoved(fm::vec2 p,fm::vec2 prevP)
	{
		(void)p;
		(void)prevP;
	}

	/////////////////////////////////////////////////////////////
	bool MouseMoveListener::mouseInside() const
	{
		return m_mouseIn;
	}
}

