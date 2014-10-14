#include <FRONTIER/Window/GLContext.hpp>
#include <FRONTIER/Config.hpp>

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
