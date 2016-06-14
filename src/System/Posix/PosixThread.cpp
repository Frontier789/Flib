////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/Posix/PosixThread.hpp>
#include <FRONTIER/System/Clock.hpp>
#include <FRONTIER/System/Sleep.hpp>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

#ifdef PTHREAD_CANCEL_ASYNCHRONOUS
	#define USE_PTHREAD_CANCEL
#endif

namespace fm
{
	namespace Posix
	{

		/////////////////////////////////////////////////////////////
		void *Thread::startThread(void *param)
		{
			priv::DataPass *caller = (priv::DataPass*)param;

		#ifdef USE_PTHREAD_CANCEL
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
		#endif

			if (caller)
			{
			    fm::Thread::setCurrentThread(caller->thread);

				caller->delegate.call(caller->thread);
			}

			return 0;
		}

		/////////////////////////////////////////////////////////////
		Thread::Thread() : m_id(fm::nullPtr),
                           m_caller(fm::nullPtr),
						   m_running(false),
						   m_isExiting(false)
		{

		}

		/////////////////////////////////////////////////////////////
		Thread::~Thread()
		{
			delete m_id;
		}

		/////////////////////////////////////////////////////////////
		void Thread::cleanUp()
		{
			// delete windows handle
			delete m_id;
			m_id = fm::nullPtr;

			m_running = false;
		}

		/////////////////////////////////////////////////////////////
		fm::Result Thread::setEntry(const fm::Delegate<void,fm::Thread *> &runner,fm::Thread *owner)
		{
			// clean the mess
			cleanUp();

			// create the new handle
			m_id = new pthread_t;

			// copy the caller function
			delete m_caller;
			m_caller = new priv::DataPass;
			m_caller->delegate = runner;
			m_caller->thread = owner;

			return fm::Result();
		}

		/////////////////////////////////////////////////////////////
		bool Thread::start()
		{
			if (!m_id && m_caller)
				m_id = new pthread_t;

			if (m_id)
			{
				m_isExiting = false;

				if (pthread_create(m_id,NULL,startThread,m_caller) == 0)
				{
					m_running = true;
					return true;
				}
			}

			return false;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::join()
		{
			if (m_running)
			{
				if (pthread_join(*m_id,NULL) != 0)
					return false;

				cleanUp();
			}

			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::join(const fm::Time &timeOut)
		{
			if (m_running)
			{
				fm::Clock clk;
				while (clk.getTime() < timeOut && pthread_kill(*m_id,0) == 0)
					fm::Sleep(fm::milliseconds(3));

				return (clk.getTime() < timeOut);
			}

			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::isExiting(const Posix::Thread &thread)
		{
			bool ret;

            thread.m_exitMutex.lock();
            ret = thread.m_isExiting;
            thread.m_exitMutex.unLock();

            return ret;
		}

		/////////////////////////////////////////////////////////////
		void Thread::requestExit()
		{
			m_exitMutex.lock();
			m_isExiting = true;
			m_exitMutex.unLock();
		}

		/////////////////////////////////////////////////////////////
		bool Thread::forceExit()
		{
			if (m_running)
			{
			#ifdef USE_PTHREAD_CANCEL
				if (pthread_cancel(*m_id) != 0)
					return false;
			#else
				if (pthread_kill(*m_id,SIGUSR1) != 0)
					return false;
			#endif
			}
			return true;
		}
	}
}
