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
#include <FRONTIER/System/util/OS.h>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Window/Event.hpp>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiEvent.cpp"
#elif defined(FRONTIER_OS_LINUX) && !defined(FRONTIER_OS_ANDROID)
	#include "Xlib/XlibEvent.cpp"
#else
	#warning No EventImpl!
#endif

namespace fw
{
	////////////////////////////////////////////////////////////
	ButtonEvent::operator fm::vec2i() const
	{
		return fm::vec2i(x,y);
	}

	////////////////////////////////////////////////////////////
	MotionEvent::operator fm::vec2i() const
	{
		return fm::vec2i(x,y);
	}

	////////////////////////////////////////////////////////////
	MouseWheelEvent::operator fm::vec2i() const
	{
		return fm::vec2i(x,y);
	}

	////////////////////////////////////////////////////////////
	ResizeEvent::operator fm::vec2u() const
	{
		return fm::vec2u(w,h);
	}

	////////////////////////////////////////////////////////////
	DropEvent::operator fm::vec2i() const
	{
		return fm::vec2i(x,y);
	}

	////////////////////////////////////////////////////////////
	fm::vec2i Mouse::getPosition(const fw::Window &window)
	{
		return getPosition(window.getOSWindow());
	}

	////////////////////////////////////////////////////////////
	Event::EventData::EventData(KeyboardEvent key)     : key(key)       {}
	Event::EventData::EventData(ButtonEvent mouse)     : mouse(mouse)   {}
	Event::EventData::EventData(MotionEvent motion)    : motion(motion) {}
	Event::EventData::EventData(ResizeEvent size)      : size(size)     {}
	Event::EventData::EventData(TextEvent text)        : text(text)     {}
	Event::EventData::EventData(MouseWheelEvent wheel) : wheel(wheel)   {}
	Event::EventData::EventData(DropEvent drop)        : drop(drop)     {}

	////////////////////////////////////////////////////////////
	Event::Event() : type(Empty)
	{

	}

	////////////////////////////////////////////////////////////
	Event::Event(Event::EventType type) : type(type)
	{

	}

	////////////////////////////////////////////////////////////
	Event::Event(Event::EventType type,EventData data) : type(type)
	{
		key    = data.key;
		mouse  = data.mouse;
		motion = data.motion;
		size   = data.size;
		text   = data.text;
		wheel  = data.wheel;
		drop   = data.drop;
	}

	////////////////////////////////////////////////////////////
	Event::operator bool() const
	{
		return type != Empty;
	}

	/////////////////////////////////////////////////////////////
	void Mouse::setPosition(const fm::vec2i &pos,const fw::Window &window)
	{
		setPosition(pos,window.getOSWindow());
	}
}
