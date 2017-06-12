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
												   m_scrollState(0),
												   m_scrollSize(.1)
	{
		
	}

	/////////////////////////////////////////////////////////////
	void GuiScrollBar::onScroll(float amount)
	{
		float delta = amount * m_scrollSize;
		
		setScrollState(getScrollState() + delta);
	}

	/////////////////////////////////////////////////////////////
	void GuiScrollBar::setScrollSize(float scrollSize)
	{
		m_scrollSize = scrollSize;
	}

	/////////////////////////////////////////////////////////////
	void GuiScrollBar::setScrollState(float state)
	{
		if (state < 0) state = 0;
		if (state > 1) state = 1;
		
		m_scrollState = state;
		callCallback(m_scrollState);
	}

	/////////////////////////////////////////////////////////////
	float GuiScrollBar::getScrollSize() const
	{
		return m_scrollSize;
	}

	/////////////////////////////////////////////////////////////
	float GuiScrollBar::getScrollState() const
	{
		return m_scrollState;
	}
}
