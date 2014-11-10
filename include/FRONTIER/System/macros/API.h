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
#ifndef FRONTIER_API_H_INCLUDED
#define FRONTIER_API_H_INCLUDED
#include <FRONTIER/System/macros/OS.h>

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
