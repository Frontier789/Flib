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
#include <FRONTIER/Gui/GuiScrollBar.hpp>
#include <FRONTIER/Window/Event.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	GuiScrollBar::GuiScrollBar(GuiContext &cont) : GuiElement(cont),
												   m_scrollSize(.1),
												   m_state(0)
	{
		
	}

	/////////////////////////////////////////////////////////////
	void GuiScrollBar::onScroll(float amount)
	{
		float delta = -amount * m_scrollSize;
		
		setState(getState() + delta);
	}

	/////////////////////////////////////////////////////////////
	void GuiScrollBar::setScrollSize(float scrollSize)
	{
		m_scrollSize = scrollSize;
	}

	/////////////////////////////////////////////////////////////
	void GuiScrollBar::setState(float state)
	{
		if (state < 0) state = 0;
		if (state > 1) state = 1;
		
		m_state = state;
		callCallback(m_state);
	}

	/////////////////////////////////////////////////////////////
	float GuiScrollBar::getScrollSize() const
	{
		return m_scrollSize;
	}

	/////////////////////////////////////////////////////////////
	float GuiScrollBar::getState() const
	{
		return m_state;
	}
}
