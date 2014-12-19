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
#include <FRONTIER/Window/GLContext.hpp>
#include <FRONTIER/System/macros/OS.h>

namespace fw
{
	/////////////////////////////////////////////////////////////
	GlContext::Settings::Settings(unsigned char bitsPerPixel,
								  unsigned char depthBits,
								  unsigned char stencilBits,
								  unsigned char majorVersion,
								  unsigned char minorVersion,
								  bool compatiblityProfile) : bitsPerPixel(bitsPerPixel),
															  depthBits(depthBits),
															  stencilBits(stencilBits),
															  majorVersion(majorVersion),
															  minorVersion(minorVersion),
															  compatiblityProfile(compatiblityProfile)
	{
		
	}
}

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiGLContext.cpp"
#else
	#warning No GLContext!
#endif
