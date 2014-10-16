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
#include <string>

namespace fm
{
	std::string WapiGetLastError()
	{
		std::string ret;
		char *errorText; // FormatMessage allocates memory automatically for us
		DWORD err=GetLastError();
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL,
					   err,
					   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					   (LPTSTR)&errorText,0,NULL);
		if (errorText)
		{
			// if we got any description then we convert it to string
			ret=std::string(errorText);
			::LocalFree(errorText); //we free up the allocated memory
		}
		
		while (ret.length() && (ret[ret.length()-1]=='\t' || 
								ret[ret.length()-1]==' '  || 
								ret[ret.length()-1]=='\r' || 
								ret[ret.length()-1]=='\n') ) ret.resize(ret.length()-1);
		
		return ret;
	}
	
	namespace Wapi
	{

		/////////////////////////////////////////////////////////////
		DWORD __stdcall Thread::startThread(void *param)
		{
			fm::priv::ThreadFuntionCaller *caller = (fm::priv::ThreadFuntionCaller*)param;
			
			if (caller)
			{
				caller->callFunc();
				delete caller;
				caller = NULL;
			}
			return 0;
		}

		/////////////////////////////////////////////////////////////
		Thread::Thread() : m_id(NULL),
						   m_isExiting(0),
						   m_storage(NULL)
		{
			
		}

		void Thread::cleanUp()
		{
			// ask the thread to exit
			requestExit();
			
			// wait for it
			join();
			
			// delete caller struct
			delete m_storage;
			m_storage = NULL;
			m_isExiting = 0;
			
			// delete windows handle
			if (m_id)
			{
				if (!CloseHandle(m_id))
					fm::fm_log<<"CloseHandle failed (LastError=\""<<WapiGetLastError()<<"\")"<<std::endl;
				m_id = NULL;
			}
		}
		/////////////////////////////////////////////////////////////
		bool Thread::create(fm::priv::ThreadFuntionCaller *runner)
		{
			// clean the mess
			cleanUp();
			
			// copy the caller struct
			m_storage = runner;
			
			// create the new handle
			m_id = CreateThread(NULL,0,
								startThread,m_storage,
								CREATE_SUSPENDED,NULL);
			
			if (!m_id)
			{
				delete m_storage;
				m_storage = NULL;
				fm::fm_log<<"CreateThread failed (LastError=\""<<WapiGetLastError()<<"\")"<<std::endl;
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
					
				fm::fm_log<<"ResumeThread failed (LastError=\""<<WapiGetLastError()<<"\")"<<std::endl;
			}
			return false;
		}

		/////////////////////////////////////////////////////////////
		Thread *Thread::getCurrentThread()
		{
			return NULL;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::join()
		{
			if (m_id)
			{
				DWORD result = WaitForSingleObject(m_id,INFINITE);
				if (result == WAIT_FAILED)
				{
					fm::fm_log<<"WaitForSingleObject failed (LastError=\""<<WapiGetLastError()<<"\")"<<std::endl;
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
					fm::fm_log<<"WaitForSingleObject failed (LastError=\""<<WapiGetLastError()<<"\")"<<std::endl;
					return false;
				}
				if (result == WAIT_TIMEOUT)
					return false;
			}
			return true;
		}

		/////////////////////////////////////////////////////////////
		bool Thread::isExiting() const
		{
			return (bool)m_isExiting;
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
					fm::fm_log<<"TerminateThread failed (LastError=\""<<WapiGetLastError()<<"\")"<<std::endl;
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
				
				fm::fm_log<<"SuspendThread failed (LastError=\""<<WapiGetLastError()<<"\")"<<std::endl;
			}
			return false;
		}
	}
}
