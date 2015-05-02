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
#ifndef FRONTIER_GLCHECK_HPP_INCLUDED
#define FRONTIER_GLCHECK_HPP_INCLUDED
#include <FRONTIER/System/Log.hpp>
#include <FRONTIER/Config.hpp>
#define FRONTIER_GLCHECK

#ifdef FRONTIER_DEBUG
	#define glCheck(call) ((call),fg::priv::glCheckError(__FILE__,__LINE__))
#else
	#define glCheck(call) (call)
#endif

namespace fg
{
	FRONTIER_API extern fm::Log glCheck_log;
	
	namespace priv
	{
		//////////////////////////////////
		/// @brief Debug function used to check for OpenGL errors
		/// 
		/// This function should be used indirectly by calling glCheck macro
		/// It outputs to fg::glCheck_log
		/// 
		/// @param file The file where the call was made
		/// @param line The line from the call was made
		/// 
		//////////////////////////////////
		void FRONTIER_API glCheckError(const char *file, unsigned int line);
	}
}
#endif // FRONTIER_GLCHECK_HPP_INCLUDED
