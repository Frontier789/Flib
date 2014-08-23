#ifndef FRONTIER_GLCHECK_HPP_INCLUDED
#define FRONTIER_GLCHECK_HPP_INCLUDED
#include <FRONTIER/Config.hpp>
#define FRONTIER_GLCHECK

#ifdef FRONTIER_DEBUG
	#define glCheck(call) ((call),fg::priv::glCheckError(__FILE__,__LINE__))
#else
	#define glCheck(call) (call)
#endif

namespace fg
{
	namespace priv
	{
		//////////////////////////////////
		/// @brief Debug function used to check for OpenGL errors
		/// 
		/// This function should be used indirectly by calling glCheck macro
		/// 
		/// @param file The file where the call was made
		/// @param line The line from the call was made
		/// 
		//////////////////////////////////
		void glCheckError(const char *file, unsigned int line);
	}
}
#endif // FRONTIER_GLCHECK_HPP_INCLUDED
