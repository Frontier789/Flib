////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_ICE_HPP_INCLUDED
#define FRONTIER_ICE_HPP_INCLUDED
#include <FRONTIER/System/util/RequireCpp11.hpp>
#include <type_traits>
#define FRONTIER_ICE

namespace fm
{
	//////////////////////////////////////////////////////////////////////////
	/// @brief Has a value member that is true iff all the template arguments are true
	/// 
	/// @ingroup System
	/// 
	//////////////////////////////////////////////////////////////////////////
	template<bool... Args>
	class IceAnd : public std::true_type {};

	//////////////////////////////////////////////////////////////////////////
	/// 
	//////////////////////////////////////////////////////////////////////////
	template<bool... Args>
	class IceAnd<true,Args...> : public IceAnd<Args...> {};

	//////////////////////////////////////////////////////////////////////////
	/// 
	//////////////////////////////////////////////////////////////////////////
	template<bool... Args>
	class IceAnd<false,Args...> : public std::false_type {};

	//////////////////////////////////////////////////////////////////////////
	/// @brief Has a value member that is true iff at least one of the template arguments is true
	/// 
	/// @ingroup System
	/// 
	//////////////////////////////////////////////////////////////////////////
	template<bool... Args>
	class IceOr : public std::false_type {};

	//////////////////////////////////////////////////////////////////////////
	/// 
	//////////////////////////////////////////////////////////////////////////
	template<bool... Args>
	class IceOr<true,Args...> : public std::true_type {};

	//////////////////////////////////////////////////////////////////////////
	/// 
	//////////////////////////////////////////////////////////////////////////
	template<bool... Args>
	class IceOr<false,Args...> : public IceOr<Args...> {};

	//////////////////////////////////////////////////////////////////////////
	/// @brief Has a value member that is true iff the template argument is false
	/// 
	/// @ingroup System
	/// 
	//////////////////////////////////////////////////////////////////////////
	template<bool Arg1>
	class IceNot : public std::false_type {};

	//////////////////////////////////////////////////////////////////////////
	/// 
	//////////////////////////////////////////////////////////////////////////
	template<>
	class IceNot<false> : public std::true_type {};
}

#endif // FRONTIER_ICE_HPP_INCLUDED