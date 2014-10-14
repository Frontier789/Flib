////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/Config.hpp>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiEvent.cpp"
#else
	#warning No EventImpl!
#endif

namespace fw
{
	////////////////////////////////////////////////////////////
	Event::Event() : type(Empty)
	{
		
	}
	
	////////////////////////////////////////////////////////////
	Event::Event(Event::EventType type) : type(type)
	{
		
	}
	
	////////////////////////////////////////////////////////////
	Event::operator bool() const
	{
		return type != Empty;
	}
}
		 