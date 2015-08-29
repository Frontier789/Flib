////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_CLIPBOARD_HPP_INCLUDED
#define FRONTIER_CLIPBOARD_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/String.hpp>
#define FRONTIER_CLIPBOARD

namespace fm
{
	/////////////////////////////////////////////////////////////
	namespace util
	{
		/////////////////////////////////////////////////////////////
		namespace clipboard
		{
			/////////////////////////////////////////////////////////////
			bool FRONTIER_API openClipboard();

			/////////////////////////////////////////////////////////////
			void FRONTIER_API closeClipboard();

			/////////////////////////////////////////////////////////////
			void FRONTIER_API emptyClipboard();

			/////////////////////////////////////////////////////////////
			bool FRONTIER_API hasTextData();

			/////////////////////////////////////////////////////////////
			fm::String FRONTIER_API getData();

			/////////////////////////////////////////////////////////////
			void FRONTIER_API setData(const fm::String &data);
		}
	}
}

#endif // FRONTIER_CLIPBOARD_HPP_INCLUDED


