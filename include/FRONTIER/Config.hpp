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

#ifndef FRONTIER_OS_KNOWN
		#define FRONTIER_OS_UNKNOWN
#endif

#if defined(FRONTIER_OS_ANDROID) && defined(FRONTIER_OS_LINUX)
	#undef FRONTIER_OS_LINUX
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
/*
#ifdef FRONTIER_DYNAMIC

    #ifdef FRONTIER_OS_WINDOWS

        #define FRONTIER_API_EXPORT __declspec(dllexport)
        #define FRONTIER_API_IMPORT __declspec(dllimport)

        #ifdef _MSC_VER
            #pragma warning(disable : 4251)
        #endif

    #else
		
		#ifdef __GNUC__
			#if __GNUC__ >= 4

				#define FRONTIER_API_EXPORT __attribute__ ((__visibility__ ("default")))
				#define FRONTIER_API_IMPORT __attribute__ ((__visibility__ ("default")))
			
			#else
			
				#define FRONTIER_API_EXPORT
				#define FRONTIER_API_IMPORT
				
			#endif
        #else

            #define FRONTIER_API_EXPORT
            #define FRONTIER_API_IMPORT

        #endif

    #endif

#else

    #define FRONTIER_API_EXPORT
    #define FRONTIER_API_IMPORT

#endif
*/


//////////////////////////////////
/// commun types and
/// values
//////////////////////////////////
namespace fm
{
	const float PI = 3.1415926535f;
	const float euler_e = 2.71828f;
	typedef unsigned int   Uint;
	
	typedef signed   char  Int8;
    typedef signed   short Int16;
    typedef signed   int   Int32;
    typedef unsigned char  Uint8;
    typedef unsigned short Uint16;
    typedef unsigned int   Uint32;
    
}
#endif
