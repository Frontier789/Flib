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
#ifndef FRONTIER_GLCONTEXT_INCLUDED
#define FRONTIER_GLCONTEXT_INCLUDED
#include <FRONTIER/Window/WindowPredef.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_GLCONTEXT

namespace fw
{
	/////////////////////////////////////////////////////////////
	/// @brief GLContext
	/// 
	/// @ingroup Window
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GLContext 
	{
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Holds the attributes of a OpenGL context
		/// 
		/// @ingroup Window
		/// 
		/////////////////////////////////////////////////////////////
		class FRONTIER_API Settings
		{
		public:
			typedef Settings &reference;
			typedef const Settings &const_reference;
			
			unsigned char bitsPerPixel; ///< The number of bits describing a pixel (usually 32)
			unsigned char depthBits;    ///< The number of bits holding the depth value
			unsigned char stencilBits;  ///< The number of stencil bits
			unsigned char majorVersion; ///< The major OpenGL version 
			unsigned char minorVersion; ///< The minor OpenGL version 
			bool compatiblityProfile;   ///< Indicates that the context is backward compatible
			
			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			/// 
			/// If an attribute is 0 it means default value
			/// 
			/// @param bitsPerPixel The number of bits describing a pixel (usually 32)
			/// @param depthBits    The number of bits holding the depth value
			/// @param stencilBits  The number of stencil bits
			/// @param majorVersion The major OpenGL version 
			/// @param minorVersion The minor OpenGL version 
			/// @param compatiblityProfile Indicates that the context is backward compatible
			/// 
			/////////////////////////////////////////////////////////////
			Settings(unsigned char bitsPerPixel = 32,
					 unsigned char depthBits    = 24,
					 unsigned char stencilBits  = 8,
					 unsigned char majorVersion = 4,
					 unsigned char minorVersion = 4,
					 bool compatiblityProfile   = true);

			/////////////////////////////////////////////////////////////
			/// @brief Decreases the OpenGL version AND keeps it valid (max knwown is 4.5)
			/// 
			/// Known versions are:
			/// 	4.5 (2014)
			/// 	4.4 (2013)
			/// 	4.3 (2012)
			/// 	4.2 (2011)
			/// 	4.1 (2010)
			/// 	4.0 (2010)
			/// 	3.3 (2010)
			/// 	3.2 (2009)
			/// 	3.1 (2009)
			/// 	3.0 (2008)
			/// 	2.1 (2006)
			/// 	2.0 (2004)
			/// 	1.5 (2003)
			/// 	1.4 (2002)
			/// 	1.3 (2001)
			/// 	1.2 (1998)
			/// 	1.1 (1997)
			/// 	1.0 (1992)
			/// Above 4.5 every version is considered valid
			/// 
			/////////////////////////////////////////////////////////////
			void decreaseVersion();
		};
	};
}

#endif // FRONTIER_GLCONTEXT_INCLUDED