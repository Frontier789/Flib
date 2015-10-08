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
#ifndef FRONTIER_FILESYSTEM_HPP_INCLUDED
#define FRONTIER_FILESYSTEM_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/String.hpp>
#define FRONTIER_FILESYSTEM
#include <deque>

namespace fm
{
	/////////////////////////////////////////////////////////////
	namespace util
	{
		/////////////////////////////////////////////////////////////
		namespace fileSys
		{
			class FRONTIER_API Entry
			{
			public:
				fm::String path;
				fm::String name;
				bool dir;
				
				Entry(const fm::String &fullName,bool dir);
				Entry(const fm::String &path, const fm::String &name,bool dir);
			};
			
			/////////////////////////////////////////////////////////////
			std::deque<Entry> FRONTIER_API ls(const fm::String &pattern,bool recursive = false,bool (*filter)(const Entry &) = fm::nullPtr);
		}
	}
}

#endif // FRONTIER_FILESYSTEM_HPP_INCLUDED


