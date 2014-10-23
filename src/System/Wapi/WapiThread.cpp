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
#include <FRONTIER/System/Wapi/WapiThread.hpp>
#include <FRONTIER/System/FmLog.hpp>
#include "fmWapiPrintLastError.hpp"

namespace fm
{
	namespace Wapi
	{
		fm::TlsPtr<fm::Thread> Thread::m_currentThread;

		/////////////////////////////////////////////////////////////
		DWORD __stdcall Thread::startThread(void *param)
		{
			fm::priv::ThreadFuntionCaller *caller = (fm::priv::ThreadFuntionCaller*)param;
			
			if (caller)
			{
				m_currentThread = caller->m_owner;
				caller->callFunc();
			}
			
			return 0;
		}

		/////////////////////////////////////////////////////////////
		Thread::Thread() : m_id(NULL),
						   m_isExiting(0)
		{
			
		}

		void Thread::cleanUp()
		{
			// ask the thread to exit
			requestExit();
			
			// wait for it
			join();
			
			// since the thread is not running at this point
			// we can safely set it by =
			m_isExiting = 0;
			
			// delete windows handle
			if (m_id)
			{
				if (!CloseHandle(m_id))
					fm::WapiPrintLastError(fm::fm_log,CloseHandle);
				m_id = NULL;
			}
		}
		/////////////////////////////////////////////////////////////
		bool Thread::create(fm::priv::ThreadFuntionCaller *runner)
		{
			// clean the mess
			cleanUp();
			
			// create the new handle
			m_id = CreateThread(NULL,0,
								startThread,runner,
								CREATE_SUSPENDED,NULL);
			
			if (!m_id)
			{
				fm::WapiPrintLastError(fm::fm_log,CreateThread);
				return false;
			}
			
			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::start()
		{
			if (m_id)
			{
				if (ResumeThread(m_id)!=(DWORD)(-1))
					return true;
					
				fm::WapiPrintLastError(fm::fm_log,ResumeThread);
			}
			return false;
		}

		/////////////////////////////////////////////////////////////
		Thread *Thread::getCurrentThread()
		{
			return (Thread*)(m_currentThread->getImpl());
		}

		/////////////////////////////////////////////////////////////
		bool Thread::join()
		{
			if (m_id)
			{
				DWORD result = WaitForSingleObject(m_id,INFINITE);
				if (result == WAIT_FAILED)
				{
					fm::WapiPrintLastError(fm::fm_log,WaitForSingleObject);
					return false;
				}
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
				{
					fm::WapiPrintLastError(fm::fm_log,WaitForSingleObject);
					return false;
				}
				if (result == WAIT_TIMEOUT)
					return false;
			}
			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::isExiting(const Thread *thread)
		{
			if (thread)
			{
				LONG value = InterlockedExchangeAdd(&thread->m_isExiting,0);
				return value != 0;
			}
			return false;
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
				{
					fm::WapiPrintLastError(fm::fm_log,TerminateThread);
					return false;
				}
			}
			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::pause()
		{
			if (m_id)
			{
				if (SuspendThread(m_id)!=(DWORD)(-1))
					return true;
				
				fm::WapiPrintLastError(fm::fm_log,SuspendThread);
			}
			return false;
		}
	}
}
