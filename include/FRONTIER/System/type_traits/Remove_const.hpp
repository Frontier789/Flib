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
#ifndef FRONTIER_REMOVE_CONST_HPP_INCLUDED
#define FRONTIER_REMOVE_CONST_HPP_INCLUDED
#define FRONTIER_REMOVE_CONST

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member typedef 'type' which is the same as T except it is not const
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
    class Remove_const
    {
   	public:
   		typedef T type;
    };
    
    /// @cond DOXYGEN_HIDE
    template<class U>
	class Remove_const<const U>
    {
   	public:
   		typedef typename Remove_const<U>::type type;
    };
    /// @endcond
}

#endif // FRONTIER_REMOVE_CONST_HPP_INCLUDED
