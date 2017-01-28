////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_WINDOWPREDEF_HPP_INCLUDED
#define FRONTIER_WINDOWPREDEF_HPP_INCLUDED
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/util/OS.h>

#ifdef FRONTIER_OS_WINDOWS

struct HWND__;
struct HGLRC__;

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
			typedef HWND__  *WindowHandle;
			typedef HGLRC__ *ContextHandle;

			typedef bool (*WindowEventCallback)(Window*,unsigned int,fm::UintPtr,fm::IntPtr,fm::IntPtr*);

		}
	}
#elif defined(FRONTIER_OS_LINUX) && !defined(FRONTIER_OS_ANDROID)

struct __GLXcontextRec;
union _XEvent;

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
			typedef __GLXcontextRec *ContextHandle;

			typedef bool (*WindowEventCallback)(Window*,_XEvent&);
		}
	}
#else
	namespace fw
	{
		namespace priv
		{
			typedef void *Window;
			typedef void *GLContext;
			typedef void *WindowHandle;
			typedef void *ContextHandle;

			typedef bool (*WindowEventCallback)(Window*);
		}
	}
	#warning No window nor glcontext!
	#define FRONTIER_NO_WINDOW
	#define FRONTIER_NO_CONTEXT
#endif

#endif // FRONTIER_WINDOWPREDEF_HPP_INCLUDED
