////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2019 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/String.hpp>
#include <FRONTIER/System/Result.hpp>
#define FRONTIER_CLIPBOARD

namespace fm
{
	/////////////////////////////////////////////////////////////
	namespace clipboard
	{
		/////////////////////////////////////////////////////////////
		fm::Result FRONTIER_API open();

		/////////////////////////////////////////////////////////////
		fm::Result FRONTIER_API close();

		/////////////////////////////////////////////////////////////
		fm::Result FRONTIER_API clear();

		/////////////////////////////////////////////////////////////
		bool FRONTIER_API hasTextData();

		/////////////////////////////////////////////////////////////
		fm::String FRONTIER_API getTextData();

		/////////////////////////////////////////////////////////////
		fm::Result FRONTIER_API setTextData(const fm::String &data);
	}
}

#endif // FRONTIER_CLIPBOARD_HPP_INCLUDED


