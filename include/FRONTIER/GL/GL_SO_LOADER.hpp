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
#ifndef FRONTIER_GL_SO_LOADER_HPP_INCLUDED
#define FRONTIER_GL_SO_LOADER_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#include <string>

namespace priv
{
	class FRONTIER_API SO_LOADER
	{
	public:
		void *p1;
		void *p2;
		SO_LOADER();
		~SO_LOADER();
		void (*getProcAddr(const std::string &name))();
	};
	
	FRONTIER_API extern SO_LOADER so_loader;
}

#endif //FRONTIER_GL_SO_LOADER_HPP_INCLUDED
