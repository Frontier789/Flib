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
#ifndef FRONTIER_MATH_HPP_INCLUDED
#define FRONTIER_MATH_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_MATH

namespace fm
{
	
	/////////////////////////////////////////////////////////////
	/// @brief Holds fast approximations to mathematical functions
	/// 
	/// These functions do not use any std:: function
	/// The idea for square root and inv square root are from Quake III engine (see <a href="http://en.wikipedia.org/wiki/Fast_inverse_square_root">this article</a>)
	/// Also the result is less precise than their std:: sibling
	/// funcN is faster and less precise than funcK where N<K
	/// 
	/////////////////////////////////////////////////////////////
	namespace math
	{
		/////////////////////////////////////////////////////////////
		/// @brief A really fast implementation of approximated sqrt
		/// 
		/// @param number The number to find the square root of
		/// 
		/// @return The approximated square root
		/// 
		/////////////////////////////////////////////////////////////
		float FRONTIER_API sqrt1(const float &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief A fast implementation of approximated sqrt (more precise than sqrt1)
		/// 
		/// @param number The number to find the square root of
		/// 
		/// @return The approximated square root
		/// 
		/////////////////////////////////////////////////////////////
		float FRONTIER_API sqrt2(const float &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief A fast implementation of approximated sqrt (more precise than sqrt2)
		/// 
		/// @param number The number to find the square root of
		/// 
		/// @return The approximated square root
		/// 
		/////////////////////////////////////////////////////////////
		float FRONTIER_API sqrt3(const float &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief A really fast implementation of approximated inverse of sqrt
		/// 
		/// @param number The number to find the square root of
		/// 
		/// @return The approximated inverse square root
		/// 
		/////////////////////////////////////////////////////////////
		float FRONTIER_API invsqrt1(const float &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief A fast implementation of approximated inverse of sqrt (more precise than invsqrt1)
		/// 
		/// @param number The number to find the square root of
		/// 
		/// @return The approximated inverse square root
		/// 
		/////////////////////////////////////////////////////////////
		float FRONTIER_API invsqrt2(const float &number);
		
		/////////////////////////////////////////////////////////////
		/// @brief A fast implementation of approximated inverse of sqrt (more precise than sqrt2)
		/// 
		/// @param number The number to find the square root of
		/// 
		/// @return The approximated inverse square root
		/// 
		/////////////////////////////////////////////////////////////
		float FRONTIER_API invsqrt3(const float &number);
	}
	
	using namespace math;
}

#endif // FRONTIER_MATH_HPP_INCLUDED
