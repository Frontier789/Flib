#include <FRONTIER/Config.hpp>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiWindow.cpp"
#else
	#warning No WindowImpl!
#endif
	   