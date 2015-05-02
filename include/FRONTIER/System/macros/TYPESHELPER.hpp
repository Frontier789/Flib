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
#ifndef FRONTIER_TYPESHELPER_HPP_INCLUDED
#define FRONTIER_TYPESHELPER_HPP_INCLUDED
#define FRONTIER_TYPESHELPER
namespace fm
{
	namespace priv
	{
		template <unsigned num> 
		class bits_per_byte_helper 
		{
		public:
			/// shift the value to the right (divide by two and floor it) by one and recursively call itself
			enum {     ///floor(num/2)
				value = bits_per_byte_helper<(num >> 1)>::value + 1
			};
		};

		template <> 
		class bits_per_byte_helper<0>
		{
		public:
			/// end recursion at 0
			enum {
				value = 0
			};
		};

		class bits_per_byte
		{
		public:
			enum {                    ///0x111...11 -> # of 1s == bits in a char (sizeof(char)==1 -> char==1byte)
				value = bits_per_byte_helper<(unsigned char)(-1)>::value
			};
		};
		
		#define FRONTIER_BITS_PER_BYTE fm::priv::bits_per_byte::value


		/// indicates that there are no more types in the list
		class NoType{};

		/// a type holding a list of (9 at most) types
		template<class T0 = NoType,
				 class T1 = NoType,
				 class T2 = NoType,
				 class T3 = NoType,
				 class T4 = NoType,
				 class T5 = NoType,
				 class T6 = NoType,
				 class T7 = NoType,
				 class T8 = NoType,
				 class T9 = NoType>
		class TypeList
		{
		public:
			/// The separated the first type
			typedef T0 First;
			
			/// The rest, shifted left
			typedef TypeList<T1,T2,T3,T4,T5,T6,T7,T8,T9,NoType> WithoutFirst;
		};
		
		/// End the recursion upon having only NoType-s
		template<>
		class TypeList<NoType,NoType,NoType,NoType,NoType,NoType,NoType,NoType,NoType>
		{
			
		};
	}
}

namespace fm
{
	namespace priv
	{
		

		/// TypeList containing the fundamental signed integer types 
		typedef TypeList<signed char,
						 signed short,
						 signed int,
						 signed long,
						 signed long long> Intlist;
						 
		/// TypeList containing the fundamental unsigned integer types 
		typedef TypeList<unsigned char,
						 unsigned short,
						 unsigned int,
						 unsigned long,
						 unsigned long long> Uintlist;
						 
		/// TypeList containing the fundamental floating point types 
		typedef TypeList<float,
						 double,
						 long double> Floatlist;
	}
}

#endif // FRONTIER_TYPESHELPER_HPP_INCLUDED
