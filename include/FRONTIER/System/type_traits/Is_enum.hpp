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
#ifndef FRONTIER_IS_ENUM_HPP_INCLUDED
#define FRONTIER_IS_ENUM_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Is_member_pointer.hpp>
#include <FRONTIER/System/type_traits/Is_arithmetic.hpp>
#include <FRONTIER/System/type_traits/Is_reference.hpp>
#include <FRONTIER/System/type_traits/Is_function.hpp>
#include <FRONTIER/System/type_traits/Is_pointer.hpp>
#include <FRONTIER/System/type_traits/Is_array.hpp>
#include <FRONTIER/System/type_traits/Is_void.hpp>
#define FRONTIER_IS_ENUM

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member enum 'value' that is true if the template parameter is a enumeration
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
    class Is_enum
    {
		/// @cond DOXYGEN_HIDE
        struct charX2
        {
            char c[2];
        };
        template<class U> static char   check(void (U::*)(void));
        template<class U> static charX2 check(...);
		/// @endcond
    public:
        enum {
			value /** @cond DOXYGEN_HIDE */ = sizeof(check<T>(0)) == sizeof(charX2)
                && !fm::Is_arithmetic    <T>::value
                && !fm::Is_pointer       <T>::value
                && !fm::Is_reference     <T>::value
                && !fm::Is_array         <T>::value
                && !fm::Is_member_pointer<T>::value
                && !fm::Is_function      <T>::value
				&& !fm::Is_void          <T>::value /** @endcond */ ///< True if T is a enumeration
        };
    };
}

#endif // FRONTIER_IS_ENUM_HPP_INCLUDED
