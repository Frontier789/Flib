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
#ifndef FRONTIER_ARGUMENTLIST_HPP_INCLUDED
#define FRONTIER_ARGUMENTLIST_HPP_INCLUDED
#include <FRONTIER/System/util/RequireCpp11.hpp>
#define FRONTIER_ARGUMENTLIST

namespace fm
{
	template<class...>
	class ArgumentList
	{
	public:
		enum {
			size = 0
		};
		
		typedef void Head;
		typedef ArgumentList<> Tail;
		
		template<template<class...> class ApplyTo>
		using Apply = ApplyTo<>;
	};

	/////////////////////////////////////////////////////////////
	/// @brief Class used to handle a template argument list
	///
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	template<class FirstArg,class... Args>
	class ArgumentList<FirstArg,Args...>
	{
	public:
		enum {
			size = ArgumentList<Args...>::size + 1 ///< The length of the argument list
		};
		
		typedef FirstArg Head; ///< The first class of the argument list
		typedef ArgumentList<Args...> Tail; ///< The list formed from the argument list by taking out the first class
		
		template<template<class...> class ApplyTo>
		using Apply = ApplyTo<FirstArg,Args...>;
	};
}

#endif // FRONTIER_ARGUMENTLIST_HPP_INCLUDED
