//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
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



