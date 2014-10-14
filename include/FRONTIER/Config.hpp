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
#ifndef FRONTIER_CONFIG_HPP_INCLUDED
#define FRONTIER_CONFIG_HPP_INCLUDED
#define FRONTIER_CONFIG
#include <cstddef>

//////////////////////////////////
/// wrapping
/// for cycles
//////////////////////////////////
#define C(nnn)  for (std::size_t i=0;i<(nnn);i++)
#define Cx(nnn) for (std::size_t x=0;x<(nnn);x++)
#define Cy(nnn) for (std::size_t y=0;y<(nnn);y++)
#define Cz(nnn) for (std::size_t z=0;z<(nnn);z++)
#define Cxy(Xnnn,Ynnn) for (std::size_t x=0;x<(Xnnn);x++)  for (std::size_t y=0;y<(Ynnn);y++)


//////////////////////////////////
/// OS identification
//////////////////////////////////
#ifndef FRONTIER_OS_KNOWN
	#if defined( _WIN16) 	 || \
		defined( _WIN32) 	 || \
		defined( _WIN64) 	 || \
		defined(__WIN32__)   || \
		defined(__TOS_WIN__) || \
		defined(__WINDOWS__) || defined(FRONTIER_OS_WINDOWS)
			
			#ifndef NOMINMAX
				#define NOMINMAX
			#endif
			#ifndef FRONTIER_OS_WINDOWS
				#define FRONTIER_OS_WINDOWS
			#endif
			#ifndef FRONTIER_OS_KNOWN
				#define FRONTIER_OS_KNOWN
			#endif

	#endif

	#if defined(__ANDROID__) || defined(FRONTIER_OS_ANDROID)
			
			#ifndef FRONTIER_OS_ANDROID
				#define FRONTIER_OS_ANDROID
			#endif
			#ifndef FRONTIER_OS_KNOWN
				#define FRONTIER_OS_KNOWN
			#endif

	#endif

	#if defined(sun) || defined(__sun) || defined(FRONTIER_OS_SOLARIS)

			#ifndef FRONTIER_OS_SOLARIS
				#define FRONTIER_OS_SOLARIS
			#endif
			#ifndef FRONTIER_OS_KNOWN
				#define FRONTIER_OS_KNOWN
			#endif

	#endif

	#if defined(sgi) || defined(__sgi) || defined(FRONTIER_OS_IRIX)

			#ifndef FRONTIER_OS_IRIX
				#define FRONTIER_OS_IRIX
			#endif
			#ifndef FRONTIER_OS_KNOWN
				#define FRONTIER_OS_KNOWN
			#endif

	#endif

	#if defined(__linux__) || \
		defined(  linux  ) || \
		defined(__linux  ) || defined(FRONTIER_OS_LINUX)
			
			#ifndef FRONTIER_OS_LINUX
				#define FRONTIER_OS_LINUX
			#endif
			#ifndef FRONTIER_OS_KNOWN
				#define FRONTIER_OS_KNOWN
			#endif

	#endif

	#if defined(macintosh) || \
		defined(Macintosh) || \
	   (defined(__APPLE__) && defined(__MACH__)) || defined(FRONTIER_OS_MACOS)
			
			#ifndef FRONTIER_OS_MACOS
				#define FRONTIER_OS_MACOS
			#endif
			#ifndef FRONTIER_OS_KNOWN
				#define FRONTIER_OS_KNOWN
			#endif

	#endif

	#if defined(  MSDOS  ) || \
		defined(__MSDOS__) || \
		defined( _MSDOS  ) || \
		defined(  __DOS__) || defined(FRONTIER_OS_DOS)
			
			#ifndef FRONTIER_OS_DOS
				#define FRONTIER_OS_DOS
			#endif
			#ifndef FRONTIER_OS_KNOWN
				#define FRONTIER_OS_KNOWN
			#endif

	#endif
#endif

#ifndef FRONTIER_OS_KNOWN
	#ifndef FRONTIER_OS_UNKNOWN
		#define FRONTIER_OS_UNKNOWN
	#endif
#endif

////////////////////////////////////////////////////////////
// Debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG) && !defined(FRONTIER_NO_DEBUG)
	#define FRONTIER_DEBUG
#endif


////////////////////////////////////////////////////////////
// Export api
////////////////////////////////////////////////////////////
#ifndef FRONTIER_API
	#ifdef FRONTIER_DYNAMIC

		#ifdef FRONTIER_OS_WINDOWS
			
			#ifdef FRONTIER_LIBBUILD
				#define FRONTIER_API __declspec(dllexport)
			#else 
				#define FRONTIER_API __declspec(dllimport)
			#endif
			
			#ifdef _MSC_VER
				#pragma warning(disable : 4251)
			#endif

		#else
			
			#if defined(__GNUC__) && __GNUC__ >= 4
				#define FRONTIER_API __attribute__ ((__visibility__ ("default")))
			#else
				#define FRONTIER_API
			#endif

		#endif

	#else
		#define FRONTIER_API
	#endif
#endif

//////////////////////////////////
/// commun types and
/// values
//////////////////////////////////
namespace fm
{
	const float PI = 3.14159265358979f;
	const float euler_e = 2.71828f;
	
	namespace priv
	{
		template <unsigned num> 
		class bits_in_char_helper 
		{
		public:
			/// shift the value to the right (divide by two and floor it) by one and recursively call itself
			enum {     ///floor(num/2)
				value = bits_in_char_helper<(num >> 1)>::value + 1
			};
		};

		template <> 
		class bits_in_char_helper<0>
		{
		public:
			/// end recursion at 0
			enum {
				value = 0
			};
		};

		class bits_in_byte
		{
		public:
			enum {                    ///0x111...11 -> # of 1s == bits in a char (sizeof(char)==1 -> char==1byte)
				value = bits_in_char_helper<(unsigned char)(-1)>::value
			};
		};


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

		/// Compares sizeof(Type::first)*BitsPerByte and BitCount and continues the recursion with the correct TypeSelector
		template<class Types,int BitCount,int MatchesFirst = -1> 
		class TypeSelectorEqual
		{
		public:
			typedef typename TypeSelectorEqual<Types,
											   BitCount,
											   sizeof(typename Types::First)*bits_in_byte::value == BitCount>::type type;
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

#ifdef __GNUC__
	#include <inttypes.h>
	#define FRONTIER_LONG_LONG   int64_t
	#define FRONTIER_U_LONG_LONG uint64_t
#else
	#define FRONTIER_LONG_LONG   signed long long 
	#define FRONTIER_U_LONG_LONG unsigned long long
#endif

namespace fm
{
	namespace priv
	{
		

		/// TypeList containing the fundamental signed integer types 
		typedef TypeList<signed char,
						 signed short,
						 signed int,
						 signed long,
						 FRONTIER_LONG_LONG> Intlist;
						 
		/// TypeList containing the fundamental unsigned integer types 
		typedef TypeList<unsigned char,
						 unsigned short,
						 unsigned int,
						 unsigned long,
						 FRONTIER_U_LONG_LONG> Uintlist;
						 
		/// TypeList containing the fundamental floating point types 
		typedef TypeList<float,
						 double,
						 long double> Floatlist;
	}
	
	#undef FRONTIER_LONG_LONG
	#undef FRONTIER_U_LONG_LONG

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
	
	/////////////////////////////////////////////////////////////
	typedef priv::TypeSelectorAtLeast<priv::Intlist,sizeof((int*)(0)-(int*)(0))*fm::priv::bits_in_byte::value>::type Ptrdiff;
	typedef priv::TypeSelectorAtLeast<priv::Uintlist,sizeof(sizeof(void*))*fm::priv::bits_in_byte::value>::type Size;
	
	#define FRONTIER_BITS_PER_BYTE fm::priv::bits_in_byte::value
}
#endif
