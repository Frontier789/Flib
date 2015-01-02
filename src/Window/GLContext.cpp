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
	
	/////////////////////////////////////////////////////////////
	void GlContext::Settings::decreaseVersion()
	{
		// set to the highest known if 1.0
		if (majorVersion == 1 && minorVersion == 0)
		{
			majorVersion = 4,
			minorVersion = 5;
			return;
		}

		// decrease as usual
		if (minorVersion == 0)
			majorVersion--,
			minorVersion = 9;
		else
			minorVersion--;

		// round down the minor part
		if (majorVersion == 1 && minorVersion > 5) minorVersion = 5;
		if (majorVersion == 2 && minorVersion > 1) minorVersion = 1;
		if (majorVersion == 3 && minorVersion > 3) minorVersion = 3;
		if (majorVersion == 4 && minorVersion > 5) minorVersion = 5;
	}
}

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiGLContext.cpp"
#else
	#warning No GLContext!
#endif
