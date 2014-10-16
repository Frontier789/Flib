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

//////////////////////////////////
/// wrapping
/// for cycles
//////////////////////////////////
#ifndef FRONTIER_C_HPP_INCLUDED
#define FRONTIER_C_HPP_INCLUDED
#define FRONTIER_C

	#define C(nnn)  for (fm::Size i=0;i<(nnn);i++)
	#define Cx(nnn) for (fm::Size x=0;x<(nnn);x++)
	#define Cy(nnn) for (fm::Size y=0;y<(nnn);y++)
	#define Cz(nnn) for (fm::Size z=0;z<(nnn);z++)
	#define Cxy(Xnnn,Ynnn) for (fm::Size x=0;x<(Xnnn);x++)  for (fm::Size y=0;y<(Ynnn);y++)

#endif // FRONTIER_C_HPP_INCLUDED



//////////////////////////////////
/// OS identification
//////////////////////////////////
#ifndef FRONTIER_OS_H_INCLUDED
#define FRONTIER_OS_H_INCLUDED
#define FRONTIER_OS

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

	#ifndef FRONTIER_OS_UNKNOWN
		#define FRONTIER_OS_UNKNOWN
	#endif

#endif // FRONTIER_OS_H_INCLUDED

////////////////////////////////////////////////////////////
// Debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG) && !defined(FRONTIER_NO_DEBUG)
	#define FRONTIER_DEBUG
#endif


////////////////////////////////////////////////////////////
// Export api
////////////////////////////////////////////////////////////
#ifndef FRONTIER_API_H_INCLUDED
#define FRONTIER_API_H_INCLUDED
#define FRONTIER_API

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
	
#endif // FRONTIER_API_H_INCLUDED

//////////////////////////////////
/// commun types and
/// values
//////////////////////////////////
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/TYPES.hpp>
namespace fm
{
	const float PI = 3.14159265358979f;
	const float euler_e = 2.71828f;	
}
#endif
