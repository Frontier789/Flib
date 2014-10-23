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
#include <FRONTIER/System/macros/OS.h>
#include <FRONTIER/System/Mutex.hpp>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiMutex.cpp"
	namespace fm
	{
		namespace priv
		{
			typedef fm::Wapi::Mutex Mutex;
		}
	}
#else
	#ifndef FRONTIER_NO_MUTEX
		#warning No Mutex!
		#define FRONTIER_NO_MUTEX
	#endif
#endif

#ifndef FRONTIER_NO_MUTEX
namespace fm
{
	/////////////////////////////////////////////////////////////
	Mutex::Mutex() : m_impl(new priv::Mutex)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Mutex::~Mutex()
	{
		delete ((priv::Mutex*)m_impl);
	}

	/////////////////////////////////////////////////////////////
	bool Mutex::lock()
	{
		return ((priv::Mutex*)m_impl)->lock();
	}

	/////////////////////////////////////////////////////////////
	bool Mutex::attemptLock()
	{
		return ((priv::Mutex*)m_impl)->attemptLock();
	}

	/////////////////////////////////////////////////////////////
	bool Mutex::unLock()
	{
		return ((priv::Mutex*)m_impl)->unLock();
	}
}
#endif