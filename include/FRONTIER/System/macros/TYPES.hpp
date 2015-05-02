//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
#ifndef FRONTIER_TYPES_HPP_INCLUDED
#define FRONTIER_TYPES_HPP_INCLUDED
#include <FRONTIER/System/macros/TYPESHELPER.hpp>
#define FRONTIER_TYPES

namespace fm
{
	namespace priv
	{
		/// Compares sizeof(Type::first)*BitsPerByte and BitCount and continues the recursion with the correct TypeSelector
		template<class Types,int BitCount,int MatchesFirst = -1> 
		class TypeSelectorEqual
		{
		public:
			typedef typename TypeSelectorEqual<Types,
											   BitCount,
											   sizeof(typename Types::First)*bits_per_byte::value == BitCount>::type type;
		};

		/// The first type didn't match so we continues the recursion with the shifted list
		template<class Types,int BitCount> 
		class TypeSelectorEqual<Types,BitCount,0>
		{
		public:
			typedef typename TypeSelectorEqual<typename Types::WithoutFirst,
											   BitCount,
											   -1>::type type;
		};

		/// The first type matches so exit the recursion
		template<class Types,int BitCount> 
		class TypeSelectorEqual<Types,BitCount,1>
		{
		public:
			typedef typename Types::First type;
		};
	}
}

namespace fm
{
	/////////////////////////////////////////////////////////////
	typedef priv::TypeSelectorEqual<priv::Intlist, 8>::type Int8;
	typedef priv::TypeSelectorEqual<priv::Intlist,16>::type Int16;
	typedef priv::TypeSelectorEqual<priv::Intlist,32>::type Int32;
	typedef priv::TypeSelectorEqual<priv::Intlist,64>::type Int64;


	/////////////////////////////////////////////////////////////
	typedef priv::TypeSelectorEqual<priv::Uintlist, 8>::type Uint8;
	typedef priv::TypeSelectorEqual<priv::Uintlist,16>::type Uint16;
	typedef priv::TypeSelectorEqual<priv::Uintlist,32>::type Uint32;
	typedef priv::TypeSelectorEqual<priv::Uintlist,64>::type Uint64;


	/////////////////////////////////////////////////////////////
	typedef priv::TypeSelectorEqual<priv::Floatlist,32>::type Float32;
	typedef priv::TypeSelectorEqual<priv::Floatlist,64>::type Float64;	
}

#endif // FRONTIER_TYPES_HPP_INCLUDED
