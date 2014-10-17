////////////////////////////////////////////////////////////////////////// <!--
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
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_HAS_COMPONENTS_HPP_INCLUDED
#define FRONTIER_HAS_COMPONENTS_HPP_INCLUDED
#include <FRONTIER/System/type_traits/Enable_if.hpp>
#include <FRONTIER/System/type_traits/Is_enum.hpp>
#define FRONTIER_HAS_COMPONENTS

namespace fm
{
	namespace priv
	{
		template<typename T>
		class HasComponentsHelper
		{
			class charX2
			{
				char a[2];
			};
			
			typedef charX2 NO;
			typedef char   YES;
			
			template<class U> 
			static typename fm::enable_if<fm::is_enum<typename U::components>::value,YES>::type Test(void*);
			
			template<class U> 
			static NO  Test(...);
			
		public:
			static const bool value = sizeof(Test<T>(0)) == sizeof(YES);
		};
	}
}

#endif // FRONTIER_HAS_COMPONENTS_HPP_INCLUDED
