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
#ifndef FRONTIER_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
#define FRONTIER_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Is_function.hpp>
#include <FRONTIER/System/type_traits/Remove_cv.hpp>
#define FRONTIER_IS_MEMBER_FUNCTION_POINTER

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member enum 'value' that is true if the template parameter is a pointer to a member function
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
    class Is_member_function_pointer
    {
    	/// @cond DOXYGEN_HIDE
		template<class U>
		class Is_member_function_pointer_helper
		{
		public:
			enum {
				value = false
			};
		};
 
		template<class U, class R>
		class Is_member_function_pointer_helper<U R::*>
		{
		public:
			enum {
				value = fm::Is_function<U>::value
			};
		};
		/// @endcond
		
    public:
        enum {
			value /** @cond DOXYGEN_HIDE */ = Is_member_function_pointer_helper<typename fm::Remove_cv<T>::type>::value/** @endcond */ ///< True if T is a enumeration
        };
    };
}

#endif // FRONTIER_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
