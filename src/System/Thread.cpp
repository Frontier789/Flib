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
	#include "Wapi/WapiThread.cpp"
	namespace fm
	{
		namespace priv
		{
			typedef fm::Wapi::Thread Thread;
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
	/////////////////////////////////////////////////////////////
	bool Thread::init(void *m_func)
	{
		return ((priv::Thread*)m_impl)->create((fm::priv::ThreadFuntionCaller*)m_func);
	}
	
	/////////////////////////////////////////////////////////////
	Thread::Thread() : m_impl(new priv::Thread)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Thread::~Thread()
	{
		delete (priv::Thread*)m_impl;
	}

	/////////////////////////////////////////////////////////////
	bool Thread::start()
	{
		return ((priv::Thread*)m_impl)->start();
	}

	/////////////////////////////////////////////////////////////
	Thread *Thread::getCurrentThread()
	{
		return NULL; ////fix me
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
	bool Thread::isExiting(const Thread *thread)
	{
		if (thread)
		{
			return ((priv::Thread*)thread->getImpl())->isExiting();
		}
		return false;
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
}

#endif // FRONTIER_NO_THREAD