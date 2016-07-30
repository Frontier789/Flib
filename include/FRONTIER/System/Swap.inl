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
#ifndef FRONTIER_SWAP_INL_INCLUDED
#define FRONTIER_SWAP_INL_INCLUDED
#include <FRONTIER/System/Swap.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T>
	typename fm::Enable_if<!fm::Has_swap<T>::value,void>::type  swap(T &a,T &b)
	{
		T tmp = a; a = b; b = tmp;
	}
	
	/////////////////////////////////////////////////////////////
	/// @brief Swaps two items using the swap member function if available
	/// 
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	typename fm::Enable_if<fm::Has_swap<T>::value,void>::type swap(T &a,T &b)
	{
		a.swap(b);
	}
}

#endif // FRONTIER_SWAP_INL_INCLUDED
