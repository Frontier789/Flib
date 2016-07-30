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
#ifndef FRONTIER_HAS_SWAP_HPP_INCLUDED
#define FRONTIER_HAS_SWAP_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Enable_if.hpp>
#define FRONTIER_HAS_SWAP

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member enum 'value' that is true iff T::swap exists and has a signature of T &T::swap(T &) or void T::swap(T &)
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<typename T>
	class Has_swap
	{
	public:
		/// @cond DOXYGEN_HIDE
		class charX2
		{
			char a[2];
		};
		
		typedef charX2 NO;
		typedef char   YES;
		
		template<class U,U &(U::*)(U &)>
		class SFINAE0;
		
		template<class U,void (U::*)(U &)>
		class SFINAE1;
		
		template<class Q> 
		static YES Test(SFINAE0<Q,&Q::swap> *p);
		
		template<class Q> 
		static YES Test(SFINAE1<Q,&Q::swap> *p);
		
		template<class Q> 
		static NO Test(...);
		/// @endcond
		
	public:
		enum {
			value /** @cond DOXYGEN_HIDE */ = sizeof(Test<T>(0)) == sizeof(YES)/** @endcond */ ///< True iff T::swap exists and has a signature of T &T::swap(T &) or void T::swap(T &)
		};
	};
}

#endif // FRONTIER_HAS_SWAP_HPP_INCLUDED
