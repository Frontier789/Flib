////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_CONFIG_HPP_INCLUDED
#define FRONTIER_CONFIG_HPP_INCLUDED
#define FRONTIER_CONFIG

//////////////////////////////////
/// wrapping
/// for cycles
//////////////////////////////////
#include <FRONTIER/System/macros/C.hpp>

//////////////////////////////////
/// OS identification
//////////////////////////////////
#include <FRONTIER/System/macros/OS.h>

////////////////////////////////////////////////////////////
// Debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG) && !defined(FRONTIER_NO_DEBUG)
	#define FRONTIER_DEBUG
#endif


////////////////////////////////////////////////////////////
// Export api
////////////////////////////////////////////////////////////
#include <FRONTIER/System/macros/API.h>

//////////////////////////////////
/// commun types and
/// values
//////////////////////////////////
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/TYPES.hpp>
namespace fm
{
	const float PI = 3.14159265358979f;
	const float euler_e = 2.71828f;
}
#endif
