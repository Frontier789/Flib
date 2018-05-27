////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_GL_API_ENTRY_H_INCLUDED
#define FRONTIER_GL_API_ENTRY_H_INCLUDED
	
	#ifdef __GNUC__
		#ifndef __stdcall
			#define __stdcall __attribute__((stdcall))
		#endif
	#endif
	
	#ifndef APIENTRY
		#if defined(__MINGW32__) || (defined(_MSC_VER) && (_MSC_VER >= 800)) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
			#define APIENTRY __stdcall
		#else
			#define APIENTRY
		#endif
	#endif // APIENTRY
	
	#ifndef API_ENTRY
		#if defined(_WIN32)
			#define API_ENTRY APIENTRY
		#else
			#define API_ENTRY
		#endif
	#endif // API_ENTRY

#endif // FRONTIER_GL_API_ENTRY_H_INCLUDED