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
#ifndef FRONTIER_COMMONTYPES_HPP_INCLUDED
#define FRONTIER_COMMONTYPES_HPP_INCLUDED
#include <FRONTIER/System/util/RequireCpp11.hpp>
#include <cstddef>
#include <cstdint>

namespace fm
{
	typedef std::size_t   Size;
	typedef std::uint8_t  Uint8;
	typedef std::uint16_t Uint16;
	typedef std::uint32_t Uint32;
	typedef std::uint64_t Uint64;
	typedef std::int8_t   Int8;
	typedef std::int16_t  Int16;
	typedef std::int32_t  Int32;
	typedef std::int64_t  Int64;
	
	typedef std::intptr_t  IntPtr;
	typedef std::uintptr_t UintPtr;
	
	template<class T> class vector2;
	template<class T> class vector3;
	template<class T> class vector4;
	
	typedef vector2<Size> vec2s;
	typedef vector3<Size> vec3s;
	typedef vector3<Size> vec4s;
}

#endif // FRONTIER_COMMONTYPES_HPP_INCLUDED
