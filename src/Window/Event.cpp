#include <FRONTIER/Window/Event.hpp>
#include <FRONTIER/Config.hpp>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiEvent.cpp"
#else
	#warning No EventImpl!
#endif

namespace fw
{
	////////////////////////////////////////////////////////////
	Event::Event() : type(Empty)
	{
		
	}
	
	////////////////////////////////////////////////////////////
	Event::operator bool() const
	{
		return type != Empty;
	}
}
		 