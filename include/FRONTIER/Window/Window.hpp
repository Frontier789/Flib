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
#ifndef FRONTIER_WINDOW_HPP_INCLUDED
#define FRONTIER_WINDOW_HPP_INCLUDED
#include <FRONTIER/Config.hpp>

namespace fw
{
	/////////////////////////////////////////////////////////////
	/// @brief Wrapper class used to manage windows os independently
	/// 
	/// @ingroup Window
	/// 
	/////////////////////////////////////////////////////////////
	class Window
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Enumeration used to indicate window style
		/// 
		/// @ingroup Window
		/// 
		/////////////////////////////////////////////////////////////
		enum WindowStyle {
			None       = 0x0000000,
			Close      = 0x0000001,
			Border     = 0x0000010,
			Resize     = 0x0000100,
			Minimize   = 0x0001000,
			Maximaze   = 0x0010000,
			Titlebar   = 0x0100000,
			Fullscreen = 0x1000000,
			Default    = 0x0111111
		};
	};
}

#ifdef FRONTIER_OS_WINDOWS
	namespace fw
	{
		namespace Wapi
		{
			class Window;
		}
		namespace priv
		{
			typedef fw::Wapi::Window Window;
		}
	}
#else
	#warning No window
#endif

#endif // FRONTIER_WINDOW_HPP_INCLUDED
