#include <FRONTIER/System/Sleep.hpp>
#include <FRONTIER/System/Time.hpp>
#include <FRONTIER/Config.hpp>

#if __cplusplus >= 201103L
	#include <thread>
	#include <chrono>
#elif defined(FRONTIER_OS_WINDOWS)
	#include <windows.h>
#else
	#include <unistd.h>
#endif

namespace fm
{
	/////////////////////////////////////////////////////////////
	void Sleep(const Time &duration)
	{
		if (duration > Time::Zero)
		{
			#if __cplusplus >= 201103L
				std::this_thread::sleep_for(std::chrono::microseconds(duration.asMicroseconds());
			#elif defined(FRONTIER_OS_WINDOWS)
				::Sleep(duration.asMilliseconds());
			#else
				usleep(duration.asMicroseconds());
			#endif
		}

	}
}



