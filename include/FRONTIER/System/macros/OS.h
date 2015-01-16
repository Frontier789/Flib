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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
//////////////////////////////////////////////////////////////////////////
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
