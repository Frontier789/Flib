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
#include <FRONTIER/Gui/GuiCallback.hpp>
#include <FRONTIER/Window/Event.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	void GuiCallback::addEventHandler(fm::Delegate<bool,fw::Event&> handler)
	{
		m_handlers.push_back(handler);
	}
	
	/////////////////////////////////////////////////////////////
	bool GuiCallback::forwardToHandlers(fw::Event &ev)
	{
		for (auto &func : m_handlers)
		{
			if (func(ev)) return true;
		}
		
		return false;
	}
	
	/////////////////////////////////////////////////////////////
	bool GuiCallback::contains(fm::vec2 p) const
	{
		(void)p;
		return false;
	}
}
