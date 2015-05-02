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
#ifndef FRONTIER_REMOVE_POINTER_HPP_INCLUDED
#define FRONTIER_REMOVE_POINTER_HPP_INCLUDED
#define FRONTIER_REMOVE_POINTER

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member typedef 'type' which is the same what T points to (if T is not a pointer then 'type' matches T)
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<class T>
    class Remove_pointer
    {
   	public:
   		typedef T type;
    };
    
    /// @cond DOXYGEN_HIDE
    template<class U>
	class Remove_pointer<U*>
    {
   	public:
   		typedef U type;
    };
    /// @endcond
}

#endif // FRONTIER_REMOVE_POINTER_HPP_INCLUDED
