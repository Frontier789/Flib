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
#include <FRONTIER/Gui/ScrollListener.hpp>
#include <FRONTIER/Window/Event.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	ScrollListener::ScrollListener()
	{
		addEventHandler([&](fw::Event &ev) -> bool {
			
			if (ev.type == fw::Event::MouseWheelMoved)
			{
				if (mouseInside() || mouseGrabbed())
				{
					onScroll(ev.wheel.delta,ev.wheel.horizontal);
					
					return true;
				}
			}
			
			return false;
		});
	}

	/////////////////////////////////////////////////////////////
	void ScrollListener::onScroll(float,bool)
	{
		
	}
}
