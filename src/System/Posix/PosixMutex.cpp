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
#include <pthread.h>
#include <errno.h>

namespace fm
{
	namespace Posix
	{
		/////////////////////////////////////////////////////////////
		Mutex::Mutex()
		{
			pthread_mutex_init(&m_pmutex,NULL);
		}

		/////////////////////////////////////////////////////////////
		Mutex::~Mutex()
		{
			pthread_mutex_destroy(&m_pmutex);
		}

		/////////////////////////////////////////////////////////////
		void Mutex::lock()
		{
			pthread_mutex_lock(&m_pmutex);
		}

		/////////////////////////////////////////////////////////////
		bool Mutex::attemptLock()
		{
		    int result = pthread_mutex_trylock(&m_pmutex);

			if (result)
			{
				if (result == EDEADLK)
					return true;

                return false;
			}

			return true;
		}

		/////////////////////////////////////////////////////////////
		void Mutex::unLock()
		{
			pthread_mutex_unlock(&m_pmutex);
		}
	}
}
