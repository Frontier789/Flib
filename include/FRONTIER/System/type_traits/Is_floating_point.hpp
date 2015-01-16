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
#ifndef FRONTIER_IS_FLOATING_POINT_HPP_INCLUDED
#define FRONTIER_IS_FLOATING_POINT_HPP_INCLUDED
#define FRONTIER_IS_FLOATING_POINT

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member enum 'value' that is true if the template parameter is a floating point type (float,double,long double)
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
	class Is_floating_point
	{
	public:
		enum {
			value /** @cond DOXYGEN_HIDE */ = false /** @endcond */ ///< True if T is a floating point type (float,double,long double)
		};
	};
	
	/// @cond DOXYGEN_HIDE
	template<>
	class Is_floating_point<float>
	{
	public:
		enum {
			value = true
		};
	};
	
	template<>
	class Is_floating_point<double>
	{
	public:
		enum {
			value = true
		};
	};
	
	template<>
	class Is_floating_point<long double>
	{
	public:
		enum {
			value = true
		};
	};
	/// @endcond
}

#endif // FRONTIER_IS_FLOATING_POINT_HPP_INCLUDED
