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
#ifndef FRONTIER_WILL_RESULT_IN_HPP_INCLUDED
#define FRONTIER_WILL_RESULT_IN_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Is_same.hpp>
#include <FRONTIER/System/type_traits/Type_if.hpp>
#include <FRONTIER/System/type_traits/YesNo.hpp>

/////////////////////////////////////////////////////////////
/// @brief Resulves to true iff evaulating expr would result in type
/// 
/// @ingroup System
/// 
/////////////////////////////////////////////////////////////
#define FRONTIER_WILL_RESULT_IN(expr,type) (sizeof(fm::priv::will_result_in_helper<type>(expr))==sizeof(fm::priv::Yes))

#ifndef FRONTIER_VECTOR_RETURN_TYPE_FIRST
	#define FRONTIER_FIND_RETURN_TYPE(T1,T2,op) typename fm::Type_if<(sizeof(fm::priv::will_result_in_helper<T2>( *(T1*)0 op *(T2*)0 ))==sizeof(fm::priv::Yes)),T2,T1>::type
#else
	#define FRONTIER_FIND_RETURN_TYPE(T1,T2,op) T1
#endif

namespace fm
{
	namespace priv
	{
		template<class EXPECTED,class T>
		typename fm::Type_if<fm::Is_same<EXPECTED,T>::value,fm::priv::Yes,fm::priv::No>::type will_result_in_helper(const T &a);
	}
}

#endif // FRONTIER_WILL_RESULT_IN_HPP_INCLUDED
