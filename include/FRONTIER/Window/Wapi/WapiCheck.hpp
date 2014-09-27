#ifndef FRONTIER_WAPICHECK_HPP_INCLUDED
#define FRONTIER_WAPICHECK_HPP_INCLUDED
#include <FRONTIER/Config.hpp>
#define FRONTIER_WAPICHECK

#ifdef FRONTIER_DEBUG
	#define WapiCheck(call) fw::priv::WapiCheckError(__FILE__,__LINE__,(call))
#else
	#define WapiCheck(call) (call)
#endif

namespace fw
{
	namespace priv
	{
		//////////////////////////////////
		/// @brief Debug function used to check for Windows api errors
		/// 
		/// This function should be used indirectly by calling WapiCheck macro
		/// It outputs to fw::fw_log
		/// 
		/// @param file The file where the call was made
		/// @param line The line from the call was made
		/// @param error The result of the last call
		/// 
		//////////////////////////////////
		void WapiCheckError(const char *file, unsigned int line,long error);
	}
}
#endif // FRONTIER_WAPICHECK_HPP_INCLUDED
