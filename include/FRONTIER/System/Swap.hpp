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
#ifndef FRONTIER_SWAP_HPP_INCLUDED
#define FRONTIER_SWAP_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Has_swap.hpp>
#define FRONTIER_SWAP

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Swaps two items using the classic method
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	typename fm::Enable_if<!fm::Has_swap<T>::value,void>::type  swap(T &a,T &b);
	
	/////////////////////////////////////////////////////////////
	/// @brief Swaps two items using the swap member function if available
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	typename fm::Enable_if<fm::Has_swap<T>::value,void>::type swap(T &a,T &b);
}

#endif // FRONTIER_SWAP_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Swap.inl>
#endif
