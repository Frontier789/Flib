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
#ifndef FRONTIER_IDENTITY_HPP_INCLUDED
#define FRONTIER_IDENTITY_HPP_INCLUDED
#define FRONTIER_IDENTITY

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Has a member typedef of the template argument named type
	///
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class T>
	class Identity
	{
	public:
		typedef T type; ///< Alias to T
	};
}

#endif // FRONTIER_IDENTITY_HPP_INCLUDED
