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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/Graphics/CodePoint.hpp>
#include <FRONTIER/System/macros/OS.h>
#include <cstdlib>


namespace fg
{
	////////////////////////////////////////////////////////////
	CodePoint::CodePoint()
	{

	}

	////////////////////////////////////////////////////////////
	CodePoint::CodePoint(char c,const std::locale &locale)
	{
		// on windows use mbtowc
		#if defined(FRONTIER_OS_WINDOWS) &&                       \
		   (defined(__GLIBCPP__) || defined (__GLIBCXX__)) &&     \
		  !(defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION))
			
			(void)locale;
			
			wchar_t character = 0;
			mbtowc(&character, &c, 1);
			cp = (fm::Uint32)character;

		#else // otherwise use std::locale

			cp = (fm::Uint32)(std::use_facet< std::ctype<wchar_t> >(locale).widen(c));

		#endif
	}

	////////////////////////////////////////////////////////////
	CodePoint::CodePoint(wchar_t c) : cp(c)
	{
		
	}

	////////////////////////////////////////////////////////////
	CodePoint::operator fm::Uint32() const
	{
		return cp;
	}
}
