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
#include <FRONTIER/System/Thread.hpp>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/fmWapiPrintLastError.cpp"
	#include "Wapi/WapiThread.cpp"
	namespace fm
	{
		namespace priv
		{
			typedef fm::Wapi::Thread Thread;
		}
	}
#elif defined(FRONTIER_OS_LINUX)
	#include "Posix/fmPosixPrintErrno.cpp"
	#include "Posix/PosixThread.cpp"
	namespace fm
	{
		namespace priv
		{
			typedef fm::Posix::Thread Thread;
		}
	}
#else
	#ifndef FRONTIER_NO_THREAD
		#warning No Thread!
		#define FRONTIER_NO_THREAD
	#endif
#endif

#ifndef FRONTIER_NO_THREAD

namespace fm
{
	fm::TlsPtr<fm::Thread> Thread::m_currentThread;

	/////////////////////////////////////////////////////////////
	void Thread::cleanUp()
	{
		// ask the thread to exit
		requestExit();
		
		// wait for it
		join();
		
		// delete the caller data
		delete (fm::priv::ThreadFuntionCaller*)m_storage;
		m_storage = NULL;
		
		// delete the implementation (it's destructor takes care of it)
		delete (priv::Thread*)m_impl;
		m_impl = NULL;
	}
	
	/////////////////////////////////////////////////////////////
	bool Thread::init(void *storage)
	{
		// clean start
		cleanUp();
		
		// save the caller object
		m_storage = storage;
		
		// create new implementation object
		m_impl = new priv::Thread;
		
		if (!((priv::Thread*)m_impl)->create((fm::priv::ThreadFuntionCaller*)m_storage))
		{
			cleanUp();
			return false;
		}
		
		return true;
	}
	
	/////////////////////////////////////////////////////////////
	Thread::Thread() : m_impl(NULL),
					   m_storage(NULL)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Thread::~Thread()
	{
		cleanUp();
	}

	/////////////////////////////////////////////////////////////
	bool Thread::start()
	{
		if (m_impl)
			return ((priv::Thread*)m_impl)->start();
		return false;
	}

	/////////////////////////////////////////////////////////////
	Thread *Thread::getCurrentThread()
	{
		return (Thread*)m_currentThread;
	}

	/////////////////////////////////////////////////////////////
	bool Thread::join()
	{
		if (m_impl)
			return ((priv::Thread*)m_impl)->join();
		return true;
	}

	/////////////////////////////////////////////////////////////
	bool Thread::join(const Time &timeOut)
	{
		if (m_impl)
			return ((priv::Thread*)m_impl)->join(timeOut);
		return true;
	}

	/////////////////////////////////////////////////////////////
	bool Thread::isExiting(const Thread *thread)
	{
		if (thread)
			return priv::Thread::isExiting((priv::Thread*)(thread->getImpl()));
		return false;
	}

	/////////////////////////////////////////////////////////////
	void Thread::requestExit()
	{
		if (m_impl)
			((priv::Thread*)m_impl)->requestExit();
	}
	
	/////////////////////////////////////////////////////////////
	void *Thread::getImpl()
	{
		return m_impl;
	}
	
	/////////////////////////////////////////////////////////////
	const void *Thread::getImpl() const
	{
		return m_impl;
	}

	/////////////////////////////////////////////////////////////
	bool Thread::forceExit()
	{
		if (m_impl)
			return ((priv::Thread*)m_impl)->forceExit();
		return true;
	}
}

#endif // FRONTIER_NO_THREAD