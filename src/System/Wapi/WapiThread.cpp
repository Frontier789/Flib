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
#include <FRONTIER/System/Wapi/WapiThread.hpp>
#include "fmWapiGetLastError.hpp"
#include <FRONTIER/System/Delegate.hpp>

namespace fm
{
	namespace Wapi
	{
	    namespace priv
	    {
	        class DataPass
	        {
            public:
                fm::Delegate<void,fm::Thread*> delegate;
                fm::Thread *thread;
	        };
	    }

	    /////////////////////////////////////////////////////////////
		DWORD __stdcall Thread::startThread(void *param)
		{
		    priv::DataPass *data = (priv::DataPass*)param;

			if (data->delegate)
			{
			    fm::Thread::setCurrentThread(data->thread);

				data->delegate.call(data->thread);
			}
			
			delete data;

			return 0;
		}

		/////////////////////////////////////////////////////////////
		Thread::Thread() : m_id(fm::nullPtr),
						   m_isExiting(0)
		{

		}

		/////////////////////////////////////////////////////////////
		void Thread::cleanUp()
		{
			// since the thread is not running at this point
			// operator= is safe
			m_isExiting = 0;

			// delete windows handle
			if (m_id)
			{
				CloseHandle(m_id);
				m_id = fm::nullPtr;
			}
		}
		/////////////////////////////////////////////////////////////
		fm::Result Thread::setEntry(const fm::Delegate<void,fm::Thread *> &runner,fm::Thread *owner)
		{
			// clean the mess
			cleanUp();

			priv::DataPass *data = new priv::DataPass;
            data->delegate = runner;
			data->thread   = owner;

			// create the new handle
			m_id = CreateThread(fm::nullPtr,0,
								startThread,data,
								CREATE_SUSPENDED,NULL);

			if (!m_id)
			{
				return fm::WapiGetLastError(CreateThread);
				delete data;
			}

			return fm::Result();
		}

		/////////////////////////////////////////////////////////////
		bool Thread::start()
		{
			if (m_id)
			{
				if (ResumeThread(m_id) != (DWORD)(-1))
					return true;
			}

			return false;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::join()
		{
			if (m_id)
			{
				DWORD result = WaitForSingleObject(m_id,INFINITE);

				if (result == WAIT_FAILED)
					return false;
			}

			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::join(const fm::Time &timeOut)
		{
			if (m_id)
			{
				DWORD result = WaitForSingleObject(m_id,timeOut.asMilliseconds());

				if (result == WAIT_FAILED)
					return false;

				if (result == WAIT_TIMEOUT)
					return false;
			}

			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::isExiting(const Thread &thread)
		{
			LONG value = InterlockedExchangeAdd(&thread.m_isExiting,0);

            return value != 0;
		}

		/////////////////////////////////////////////////////////////
		void Thread::requestExit()
		{
			InterlockedExchange(&m_isExiting,1);
		}

		/////////////////////////////////////////////////////////////
		bool Thread::forceExit()
		{
			if (m_id)
			{
				if (!TerminateThread(m_id,0))
					return false;
			}

			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::pause()
		{
			if (m_id)
			{
				if (SuspendThread(m_id) != (DWORD)(-1))
					return true;
			}

			return false;
		}
	}
}
