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
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/macros/OS.h>
#include <FRONTIER/System/Thread.hpp>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/fmWapiGetLastError.cpp"
	#include "Wapi/WapiThread.cpp"
	namespace fm
	{
		namespace priv
		{
			typedef fm::Wapi::Thread Thread;
		}
	}
#elif defined(FRONTIER_OS_LINUX)
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

#include <FRONTIER/System/NullPtr.hpp>

namespace fm
{
	fm::TlsPtr<fm::Thread> Thread::m_currentThread;

	/////////////////////////////////////////////////////////////
	Thread::Thread() : m_impl(new priv::Thread)
	{

	}

    /////////////////////////////////////////////////////////////
    Thread::Thread(const fm::Delegate<void,Thread &> &callback) : m_impl(new priv::Thread)
    {
        create(callback);
    }

	/////////////////////////////////////////////////////////////
	Thread::~Thread()
	{
	    requestExit();
	    delete (priv::Thread*)m_impl;
	}

    /////////////////////////////////////////////////////////////
    fm::Result Thread::create(const fm::Delegate<void,Thread &> &callback)
	{
        requestExit();

        join();

        return ((priv::Thread*)m_impl)->setEntry(callback,this);
    }

	/////////////////////////////////////////////////////////////
	bool Thread::start()
	{
        return ((priv::Thread*)m_impl)->start();
	}

	/////////////////////////////////////////////////////////////
	Thread *Thread::getCurrentThread()
	{
		return (Thread*)m_currentThread;
	}

	/////////////////////////////////////////////////////////////
	bool Thread::join()
	{
		return ((priv::Thread*)m_impl)->join();
	}

	/////////////////////////////////////////////////////////////
	bool Thread::join(const Time &timeOut)
	{
		return ((priv::Thread*)m_impl)->join(timeOut);
	}

	/////////////////////////////////////////////////////////////
	bool Thread::isExiting(const Thread &thread)
	{
		return priv::Thread::isExiting(*(priv::Thread*)(thread.getImpl()));
	}

    /////////////////////////////////////////////////////////////
    void Thread::setCurrentThread(Thread *thread)
    {
        m_currentThread.set(thread);
    }

	/////////////////////////////////////////////////////////////
	void Thread::requestExit()
	{
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
        return ((priv::Thread*)m_impl)->forceExit();
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size Thread::getHarwareConcurrency()
	{
		return priv::Thread::getHarwareConcurrency();
	}
}

#endif // FRONTIER_NO_THREAD
