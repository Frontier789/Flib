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
												   m_stateCount(0),
												   m_state(0)
	{
		
	}

	/////////////////////////////////////////////////////////////
	void GuiScrollBar::onScroll(float amount)
	{
		m_state = m_state + (m_stateCount ? amount / m_stateCount : amount);
		callCallback(amount);
	}

	/////////////////////////////////////////////////////////////
	void GuiScrollBar::setStateCount(fm::Size stateCount)
	{
		m_stateCount = stateCount;
	}

	/////////////////////////////////////////////////////////////
	void GuiScrollBar::setState(float state)
	{
		m_state = state;
	}

	/////////////////////////////////////////////////////////////
	fm::Size GuiScrollBar::getStateCount()
	{
		return m_stateCount;
	}

	/////////////////////////////////////////////////////////////
	float GuiScrollBar::getState()
	{
		return m_state;
	}
}
