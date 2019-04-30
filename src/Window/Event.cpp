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
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/System/util/OS.h>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiEvent.cpp"
#elif defined(FRONTIER_OS_LINUX) && !defined(FRONTIER_OS_ANDROID)
	#include "Xlib/XlibEvent.cpp"
#else
	#ifndef FRONTIER_MISSING_IMPL_NO_WARNING
		#warning No EventImpl!
	#endif
#endif

namespace fw
{
	/////////////////////////////////////////////////////////////
	fm::vec3i Keyboard::keyToDelta(Key key,bool allowWASD,bool allowArrows,bool allowShiftSpace)
	{
		fm::vec3i ret;
		if (allowWASD) {
			if (key == Keyboard::W) ret.y++;
			if (key == Keyboard::A) ret.x--;
			if (key == Keyboard::S) ret.y--;
			if (key == Keyboard::D) ret.x++;
		}
		if (allowArrows) {
			if (key == Keyboard::Up)    ret.y++;
			if (key == Keyboard::Left)  ret.x--;
			if (key == Keyboard::Down)  ret.y--;
			if (key == Keyboard::Right) ret.x++;
		}
		if (allowShiftSpace) {
			if (key == Keyboard::LShift) ret.z--;
			if (key == Keyboard::RShift) ret.z--;
			if (key == Keyboard::Space)  ret.z++;
		}
		
		return ret;
	}
	
	/////////////////////////////////////////////////////////////
	MouseCursor::MouseCursor(const fg::Image &img,
							 fm::vec2s clickSpot,
							 fm::Uint8 transparencyLimit) : m_impl(nullptr)
	{
		loadFromImage(img,clickSpot,transparencyLimit);
	}
	
	////////////////////////////////////////////////////////////
	fm::vec2i Mouse::getPosition(const fw::Window &window)
	{
		return getPosition(window.getOSWindow());
	}

	/////////////////////////////////////////////////////////////
	void Mouse::setCursor(const MouseCursor &cursor)
	{
		cursor.setAsCurrent();
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
	void Mouse::setPosition(fm::vec3 pos,const fw::Window &window)
	{
		setPosition(pos,window.getOSWindow());
	}
}
