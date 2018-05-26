////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/System/util/OS.h>
#include <FRONTIER/System/Clock.hpp>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiClock.cpp"
#elif defined(FRONTIER_OS_LINUX)
	#include "Posix/PosixClock.cpp"
#else
	#include "Generic/GenericClock.cpp"
#endif
	
namespace fm
{
    /// /////////////////////////////////////////////////////////
    Clock::Clock() : m_startTime(getCurrentTime()),
					 m_pauseTime(-1)
    {
    	
    }
	
    /// Constructors /////////////////////////////////////////////////////////
    Clock::Clock(bool paused) : m_startTime(getCurrentTime()),
								m_pauseTime(paused ? m_startTime : -1)
    {
    	
    }
    
    
    ////////////////////////////////////////////////////////////
    Clock::Clock(Clock::const_reference copy) : m_startTime(copy.m_startTime),
                                                m_pauseTime(copy.m_pauseTime)
    {
    	
    }
    
    
    ////////////////////////////////////////////////////////////
    Clock::Clock(double startTime,bool paused) : m_startTime(getCurrentTime()-startTime),
												 m_pauseTime(paused ? m_startTime : -1)
    {
    	
    }
    
    
    ////////////////////////////////////////////////////////////
    double Clock::getSeconds() const
    {
		if (isPaused())
    		return m_pauseTime - m_startTime;
    	
    	return getCurrentTime() - m_startTime;
    }
    
    
    ////////////////////////////////////////////////////////////
    Clock::reference Clock::pause()
    {
		if (!isPaused())
			m_pauseTime = getCurrentTime();
		
		return *this;
    }
    
    
    ////////////////////////////////////////////////////////////
    Clock::reference Clock::unPause()
    {
		if (isPaused())
			m_startTime = getCurrentTime() - getSeconds(),
			m_pauseTime = -1;
		
		return *this;
    }
    
    
    ////////////////////////////////////////////////////////////
    Clock::reference Clock::resume()
    {
		return unPause();
    }
    
	/////////////////////////////////////////////////////////////
	double Clock::restart()
	{
		double ret = getSeconds();
		setTime(0);
		
		return ret;
	}
    
    ////////////////////////////////////////////////////////////
    Clock::reference Clock::setTime(double elapsedSeconds)
    {
		if (isPaused())
			m_startTime = m_pauseTime - elapsedSeconds;
		else
			m_startTime = getCurrentTime() - elapsedSeconds;
		
    	return *this;
    }
    
    ////////////////////////////////////////////////////////////
    Clock::reference Clock::setTime(fm::Time time)
    {
		return setTime(time.asSecs());
    }
    
	/////////////////////////////////////////////////////////////
	fm::Time Clock::getTime() const
	{
		return fm::seconds(getSeconds());
	}
    
    
    ////////////////////////////////////////////////////////////
    bool Clock::isPaused() const
    {
		return m_pauseTime != -1;
    }
    
    
    ////////////////////////////////////////////////////////////
    const Clock Clock::now = Clock();
}
