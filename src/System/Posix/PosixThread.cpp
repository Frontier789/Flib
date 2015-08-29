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
#include <FRONTIER/System/Posix/PosixThread.hpp>
#include <FRONTIER/System/Clock.hpp>
#include <FRONTIER/System/Sleep.hpp>
#include <FRONTIER/System/FmLog.hpp>
#include "fmPosixPrintErrno.hpp"
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
			fm::priv::ThreadFuntionCaller *caller = (fm::priv::ThreadFuntionCaller*)param;
			
		#ifdef USE_PTHREAD_CANCEL
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
		#endif
			
			if (caller)
			{
				caller->m_currentThreadPtr->set(caller->m_owner);
				caller->callFunc();
			}
			
			return 0;
		}

		/////////////////////////////////////////////////////////////
		Thread::Thread() : m_id(NULL),
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
			m_id = NULL;
			
			m_running = false;
		}
		
		/////////////////////////////////////////////////////////////
		bool Thread::create(fm::priv::ThreadFuntionCaller *runner)
		{
			// clean the mess
			cleanUp();
			
			// create the new handle
			m_id = new pthread_t;
			
			// copy the caller function
			m_caller = runner;
			
			return true;
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
					
				fm::PosixPrintErrno(fm::fm_log,pthread_create);
			}
			return false;
		}

		/////////////////////////////////////////////////////////////
		Thread *Thread::getCurrentThread()
		{
			return (Thread*)(fm::Thread::getCurrentThread()->getImpl());
		}

		/////////////////////////////////////////////////////////////
		bool Thread::join()
		{
			if (m_running)
			{
				if (pthread_join(*m_id,NULL) != 0)
				{
					fm::PosixPrintErrno(fm::fm_log,pthread_join);
					return false;
				}
				
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
				while (clk.getTime() < timeOut && pthread_kill(*m_id,0)==0)
					fm::Sleep(fm::milliseconds(3));
				
				return (clk.getTime() < timeOut);
			}
			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::isExiting(const Thread *thread)
		{
			if (thread)
			{
				thread->m_exitMutex.lock();
				bool ret = thread->m_isExiting;
				thread->m_exitMutex.unLock();
				
				return ret;
			}

			return false;
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
				{
					fm::PosixPrintErrno(fm::fm_log,pthread_cancel);
					return false;
				}
			#else
				if (pthread_kill(*m_id,SIGUSR1) != 0)
				{
					fm::PosixPrintErrno(fm::fm_log,pthread_kill);
					return false;
				}
			#endif
			}
			return true;
		}
	}
}
