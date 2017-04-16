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
#ifndef FRONTIER_EVENT_INL_INCLUDED
#define FRONTIER_EVENT_INL_INCLUDED
#include <FRONTIER/System/Vector2.hpp>
#define FRONTIER_EVENT

namespace fw
{
	/////////////////////////////////////////////////////////////
	template<class T>
	ButtonEvent::operator fm::vector2<T>() const
	{
		return fm::vector2<T>(x,y);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	MotionEvent::operator fm::vector2<T>() const
	{
		return fm::vector2<T>(x,y);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	ResizeEvent::operator fm::vector2<T>() const
	{
		return fm::vector2<T>(w,h);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	DropEvent::operator fm::vector2<T>() const
	{
		return fm::vector2<T>(x,y);
	}
}

#endif // FRONTIER_EVENT_HPP_INCLUDED
