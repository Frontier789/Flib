//////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
//////////////////////////////////////////////////////////////////////////
#ifndef FRONTIER_SIZE_HPP_INCLUDED
#define FRONTIER_SIZE_HPP_INCLUDED
#include <FRONTIER/System/macros/TYPESHELPER.hpp>
#define FRONTIER_SIZE

namespace fm
{
	namespace priv
	{
		/// Compares sizeof(Type::first)*BitsPerByte and BitCount and continues the recursion with the correct TypeSelector
		template<class Types,int BitCount,int MatchesFirst = -1> 
		class TypeSelectorAtLeast
		{
		public:
			typedef typename TypeSelectorAtLeast<Types,
											   BitCount,
											   sizeof(typename Types::First)*bits_in_byte::value >= BitCount>::type type;
		};

		/// The first type didn't match so we continues the recursion with the shifted list
		template<class Types,int BitCount> 
		class TypeSelectorAtLeast<Types,BitCount,0>
		{
		public:
			typedef typename TypeSelectorAtLeast<typename Types::WithoutFirst,
											   BitCount,
											   -1>::type type;
		};

		/// The first type matches so exit the recursion
		template<class Types,int BitCount> 
		class TypeSelectorAtLeast<Types,BitCount,1>
		{
		public:
			typedef typename Types::First type;
		};
	}
}

namespace fm
{
	/////////////////////////////////////////////////////////////
	typedef priv::TypeSelectorAtLeast<priv::Intlist,sizeof((int*)(0)-(int*)(0))*fm::priv::bits_in_byte::value>::type Ptrdiff;
	typedef priv::TypeSelectorAtLeast<priv::Uintlist,sizeof(sizeof(void*))*fm::priv::bits_in_byte::value>::type Size;
	typedef priv::TypeSelectorAtLeast<priv::Uintlist,sizeof(void*)*fm::priv::bits_in_byte::value>::type UintPtr;
	typedef priv::TypeSelectorAtLeast<priv::Intlist,sizeof(void*)*fm::priv::bits_in_byte::value>::type IntPtr;
}

#endif // FRONTIER_SIZE_HPP_INCLUDED