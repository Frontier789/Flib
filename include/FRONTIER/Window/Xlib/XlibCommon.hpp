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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_XLIB_COMMON_INCLUDED
#define FRONTIER_XLIB_COMMON_INCLUDED
#include <FRONTIER/System/Result.hpp>
// #include <FRONTIER/System/util/API.h>
#include <FRONTIER/Window/Event.hpp>
#include <GL/glx.h>
#include <vector>
#include <mutex>

namespace fw
{
	namespace priv
	{
		struct GlobDispHolder {
			Display *disp;
			unsigned int n;
			std::mutex mut;
		
			GlobDispHolder();
			Display *get();
			void free(Display *d);
		};

		extern GlobDispHolder gdisp;

		struct GlobalWinHolder {
			std::mutex mut;
	
			std::vector< ::Window> xwins;
			std::vector< ::Cursor> xCurrCursors;

			void reg(::Window win);
			void del(::Window win);
		};

		extern GlobalWinHolder gwin;
	
		struct GlobalCurHolder
		{
			::Cursor curs[(fm::Size)fw::Mouse::CursorCount];
			::Display *disp;
			
			GlobalCurHolder();
			~GlobalCurHolder();
			
			::Cursor fetch(Mouse::Cursor cursor);
		};

		extern GlobalCurHolder gcur;

		fm::Result FRONTIER_API getFBConfig(Display *disp,void *fbconfptr,bool window);
	}
}

#endif // FRONTIER_XLIB_COMMON_INCLUDED

