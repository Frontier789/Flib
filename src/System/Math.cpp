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
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/Math.hpp>

namespace fm
{
	#define FRONTIER_SQRT_BEGIN_CODE     \
	priv::FloatLong fl;                  \
	fl.f = number;                       \
	fl.l = 0x5f3759df - ( fl.l >> 1 );   \
	                                     \
	float x = number * 0.5f;             \
	float y = fl.f;
	
	namespace priv
	{
		/////////////////////////////////////////////////////////////
		union FloatLong
		{
			fm::Int32 l;
			float f;
		};
	}
	
	namespace math
	{
		/////////////////////////////////////////////////////////////
		float sqrt1(const float &number)
		{
			FRONTIER_SQRT_BEGIN_CODE

			y = y * ( 1.5f - ( x * y * y ) );
			return number * y;
		}
		
		/////////////////////////////////////////////////////////////
		float sqrt2(const float &number)
		{
			FRONTIER_SQRT_BEGIN_CODE

			y = y * ( 1.5f - ( x * y * y ) );
			y = y * ( 1.5f - ( x * y * y ) );
			return number * y;
		}
		
		/////////////////////////////////////////////////////////////
		float sqrt3(const float &number)
		{
			FRONTIER_SQRT_BEGIN_CODE

			y = y * ( 1.5f - ( x * y * y ) );
			y = y * ( 1.5f - ( x * y * y ) );
			y = y * ( 1.5f - ( x * y * y ) );
			return number * y;
		}
		
		/////////////////////////////////////////////////////////////
		float invsqrt1(const float &number)
		{
			FRONTIER_SQRT_BEGIN_CODE

			y = y * ( 1.5f - ( x * y * y ) );
			return y;
		}
		
		/////////////////////////////////////////////////////////////
		float invsqrt2(const float &number)
		{
			FRONTIER_SQRT_BEGIN_CODE

			y = y * ( 1.5f - ( x * y * y ) );
			y = y * ( 1.5f - ( x * y * y ) );
			return y;
		}
		
		/////////////////////////////////////////////////////////////
		float invsqrt3(const float &number)
		{
			FRONTIER_SQRT_BEGIN_CODE

			y = y * ( 1.5f - ( x * y * y ) );
			y = y * ( 1.5f - ( x * y * y ) );
			y = y * ( 1.5f - ( x * y * y ) );
			return y;
		}
	}
}
