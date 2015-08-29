////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/Posix/PosixMutex.hpp>
#include <FRONTIER/System/FmLog.hpp>
#include "fmPosixPrintErrno.hpp"
#include <pthread.h>
#include <errno.h>

namespace fm
{
	namespace Posix
	{
		/////////////////////////////////////////////////////////////
		Mutex::Mutex()
		{
			if (pthread_mutex_init(&m_pmutex,NULL) != 0)
				fm::PosixPrintErrno(fm::fm_log,pthread_mutex_init);
		}
		
		/////////////////////////////////////////////////////////////
		Mutex::~Mutex()
		{
			if (pthread_mutex_destroy(&m_pmutex) != 0)
				fm::PosixPrintErrno(fm::fm_log,pthread_mutex_destroy);
		}
		
		/////////////////////////////////////////////////////////////
		bool Mutex::lock()
		{
			if (pthread_mutex_lock(&m_pmutex) != 0)
			{
				fm::PosixPrintErrno(fm::fm_log,pthread_mutex_lock);
				return false;
			}
			return true;
		}
		
		/////////////////////////////////////////////////////////////
		bool Mutex::attemptLock()
		{
			int result = pthread_mutex_trylock(&m_pmutex);
			
			if (result != 0)
			{
				if (result != EBUSY)
					fm::PosixPrintErrno(fm::fm_log,pthread_mutex_trylock);
					
				if (result == EDEADLK)
					return true;
					
				return false;
			}
			
			return true;
		}
		
		/////////////////////////////////////////////////////////////
		bool Mutex::unLock()
		{
			if (pthread_mutex_unlock(&m_pmutex) != 0)
			{
				fm::PosixPrintErrno(fm::fm_log,pthread_mutex_unlock);
				return false;
			}
			return true;
		}
	}
}
