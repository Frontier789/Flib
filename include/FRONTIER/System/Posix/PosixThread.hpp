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
#ifndef FRONTIER_POSIXTHREAD_HPP_INCLUDED
#define FRONTIER_POSIXTHREAD_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/Mutex.hpp>
#define FRONTIER_POSIXTHREAD
#include <pthread.h>

namespace fm
{
	namespace priv
	{
		class ThreadFuntionCaller;
	}
	
	/////////////////////////////////////////////////////////////
	/// @brief Implementation of fm::Thread for posix
	/// 
	/////////////////////////////////////////////////////////////
	namespace Posix
	{
		class FRONTIER_API Thread : public fm::NonCopyable
		{
			static void *startThread(void *param);              ///< The entry point of a new thread
			pthread_t *m_id;                                    ///< The id of the thread
			void cleanUp();                                     ///< Internal cleaning function
			bool create(fm::priv::ThreadFuntionCaller *runner); ///< Internal runner function
			ThreadFuntionCaller *m_caller;
			bool m_running;
			fm::Mutex m_exitMutex;
			bool m_isExiting;
		public:
			typedef Thread &reference;
			typedef const Thread &const_reference;
			
			Thread();
			~Thread();
			bool start();
			
			static Thread *getCurrentThread();
			
			bool join();
			bool join(const fm::Time &timeOut);
			
			static bool isExiting(const Thread *thread);
			void requestExit();
			
			bool forceExit();
			
			friend class fm::Thread;
		};
	}
}

#endif // FRONTIER_POSIXTHREAD_HPP_INCLUDED
