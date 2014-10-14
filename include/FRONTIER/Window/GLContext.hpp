#ifndef FRONTIER_GLCONTEXT_INCLUDED
#define FRONTIER_GLCONTEXT_INCLUDED
#include <FRONTIER/Config.hpp>
#define FRONTIER_GLCONTEXT

namespace fw
{
	/////////////////////////////////////////////////////////////
	/// @brief GLContext
	/// 
	/// @ingroup Window
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GlContext 
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
					 unsigned char depthBits    = 0,
					 unsigned char stencilBits  = 0,
					 unsigned char majorVersion = 4,
					 unsigned char minorVersion = 4,
					 bool compatiblityProfile   = true);
		};
	};
}

#endif // FRONTIER_GLCONTEXT_INCLUDED