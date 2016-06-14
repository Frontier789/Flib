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
#ifndef FRONTIER_IS_ARITHMETIC_HPP_INCLUDED
#define FRONTIER_IS_ARITHMETIC_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Is_floating_point.hpp>
#include <FRONTIER/System/type_traits/Is_integral.hpp>
#define FRONTIER_IS_ARITHMETIC

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member enum 'value' that is true if the template parameter is a arithmetic type (integer or floating point)
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
	class Is_arithmetic
	{
	public:
		enum {
			value /** @cond DOXYGEN_HIDE */ = fm::Is_integral<T>::value ||
											  fm::Is_floating_point<T>::value /** @endcond */ ///< True if T is a arithmetic type (integer or floating point)
		};
	};
}

#endif // FRONTIER_IS_ARITHMETIC_HPP_INCLUDED
