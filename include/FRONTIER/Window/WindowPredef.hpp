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
#ifndef FRONTIER_WINDOWPREDEF_HPP_INCLUDED
#define FRONTIER_WINDOWPREDEF_HPP_INCLUDED
#include <FRONTIER/System/macros/TYPES.hpp>
#include <FRONTIER/System/macros/OS.h>

#ifdef FRONTIER_OS_WINDOWS

#ifdef STRICT
	struct HWND__;
#endif

	namespace fw
	{
		namespace Wapi
		{
			class Window;
			class GLContext;
		}
		namespace priv
		{
			typedef fw::Wapi::Window Window;
			typedef fw::Wapi::GLContext GLContext;
			
			#ifdef STRICT
				typedef HWND__ *WindowHandle;
			#else
				typedef void *WindowHandle;
			#endif
			
		}
	}
#elif defined(FRONTIER_OS_LINUX) && !defined(FRONTIER_OS_ANDROID)
	namespace fw
	{
		namespace Xlib
		{
			class Window;
			class GLContext;
		}
		namespace priv
		{
			typedef fw::Xlib::Window Window;
			typedef fw::Xlib::GLContext GLContext;
			typedef fm::Uint32 WindowHandle;
		}
	}
#else
	#warning No window
	#define FRONTIER_NO_WINDOW
#endif

#endif // FRONTIER_WINDOWPREDEF_HPP_INCLUDED
