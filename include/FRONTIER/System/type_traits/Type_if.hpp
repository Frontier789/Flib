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
#ifndef FRONTIER_TYPE_IF_HPP_INCLUDED
#define FRONTIER_TYPE_IF_HPP_INCLUDED
#define FRONTIER_TYPE_IF

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member typedef of the first template argument if B is true if not hten of the second
	/// 
	/// @ingroup System
	/// 
	/////////////////////////////////////////////////////////////
	template<bool B,class T1,class T2>
	class Type_if
	{
	public:
		typedef T1 type; ///< Alias to T1 if B is true (to T2 otherwise)
	};
	
	/// @cond DOXYGEN_HIDE
	template<class T1,class T2>
	class Type_if<false,T1,T2>
	{
	public:
		typedef T2 type;
	};
	/// @endcond
}

#endif // FRONTIER_TYPE_IF_HPP_INCLUDED
