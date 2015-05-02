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
#ifndef FRONTIER_MATH_INL_INCLUDED
#define FRONTIER_MATH_INL_INCLUDED

#include <FRONTIER/System/macros/dont_include_inl_begin>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/macros/dont_include_inl_end>

#include <FRONTIER/System/Math.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	namespace math
	{
		/////////////////////////////////////////////////////////////
		template<class T>
		inline const T &min(const T &left,const T &right)
		{
			if (left < right)
				return left;

			return right;
		}

		/////////////////////////////////////////////////////////////
		template<class T>
		inline const T &max(const T &left,const T &right)
		{
			if (left > right)
				return left;

			return right;
		}
	}
}

#endif // FRONTIER_MATH_INL_INCLUDED
