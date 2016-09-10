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
#ifndef FRONTIER_UNDERLYING_TYPE_HPP_INCLUDED
#define FRONTIER_UNDERLYING_TYPE_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Type_if.hpp>
#include <FRONTIER/System/macros/TYPES.hpp>
#define FRONTIER_UNDERLYING_TYPE

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member type 'type' that matches the underlying type of enum type T
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
    class Underlying_type
    {
		/// @cond DOXYGEN_HIDE
		template<class U>
		class Is_unsigned
		{
		public:
			enum {
				value = U(-1) < U(0)
			};			
		};
		/// @endcond
    public:
		typedef /** @cond DOXYGEN_HIDE */ typename Type_if<Is_unsigned<T>::value,
										  typename priv::TypeSelectorEqual<priv::Uintlist,sizeof(T)*fm::priv::bits_per_byte::value>::type,
										  typename priv::TypeSelectorEqual<priv:: Intlist,sizeof(T)*fm::priv::bits_per_byte::value>::type>:: /** @endcond */ type type; ///< The undelying integer type
    };
}

#endif // FRONTIER_UNDERLYING_TYPE_HPP_INCLUDED
