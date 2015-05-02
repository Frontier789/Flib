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
#ifndef FRONTIER_THREAD_HPP_INCLUDED
#define FRONTIER_THREAD_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/TlsPtr.hpp>
#include <FRONTIER/System/Time.hpp>
#define FRONTIER_THREAD

namespace fm
{
	namespace priv
	{
		class ThreadFuntionCaller;
	}
	/////////////////////////////////////////////////////////////
	/// @brief Class used to launch and manage a new thread (see <a href="http://en.wikipedia.org/wiki/Thread_%28computing%29">this wikipedia article</a> for further information)
	/// 
	/// @ingroup System
	/// 
	/// Upon error this class prompts to fm::fm_log
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Thread : public fm::NonCopyable
	{
		static fm::TlsPtr<fm::Thread> m_currentThread; ///< Holds the current thread
		void *m_impl;      ///< Stores a pointer to the undelying implementation
		bool init(void *); ///< Internal function
		void cleanUp();    ///< Internal function
		void *m_storage;   ///< Internal value used to store the calling data
	public:
		typedef Thread &reference;
		typedef const Thread &const_reference;
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		Thread();
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		~Thread();
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		///
		/// @return If @a func is NULL or the internal function fails false
		///
		/////////////////////////////////////////////////////////////
		template<class R>
		bool create(R (*func)());
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		///
		/// @return If @a func is NULL or the internal function fails false
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1>
		bool create(R (*func)(P1),P1 param1);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		/// @param param2 The 2nd parameter to pass to the function
		///
		/// @return If @a func is NULL or the internal function fails false
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2>
		bool create(R (*func)(P1,P2),P1 param1,P2 param2);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		/// @param param2 The 2nd parameter to pass to the function
		/// @param param3 The 3rd parameter to pass to the function
		///
		/// @return If @a func is NULL or the internal function fails false
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3>
		bool create(R (*func)(P1,P2,P3),P1 param1,P2 param2,P3 param3);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		/// @param param2 The 2nd parameter to pass to the function
		/// @param param3 The 3rd parameter to pass to the function
		/// @param param4 The 4th parameter to pass to the function
		///
		/// @return If @a func is NULL or the internal function fails false
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class P4>
		bool create(R (*func)(P1,P2,P3,P4),P1 param1,P2 param2,P3 param3,P4 param4);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		/// @param param2 The 2nd parameter to pass to the function
		/// @param param3 The 3rd parameter to pass to the function
		/// @param param4 The 4th parameter to pass to the function
		/// @param param5 The 5th parameter to pass to the function
		///
		/// @return If @a func is NULL or the internal function fails false
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class P4,class P5>
		bool create(R (*func)(P1,P2,P3,P4,P5),P1 param1,P2 param2,P3 param3,P4 param4,P5 param5);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param object The object which member-function is to be called
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		template<class R,class CLASS>
		bool create(CLASS *object,R (CLASS::*func)());
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		/// @param object The object which member-function is to be called
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class CLASS>
		bool create(CLASS *object,R (CLASS::*func)(P1),P1 param1);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		/// @param param2 The 2nd parameter to pass to the function
		/// @param object The object which member-function is to be called
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class CLASS>
		bool create(CLASS *object,R (CLASS::*func)(P1,P2),P1 param1,P2 param2);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		/// @param param2 The 2nd parameter to pass to the function
		/// @param param3 The 3rd parameter to pass to the function
		/// @param object The object which member-function is to be called
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class CLASS>
		bool create(CLASS *object,R (CLASS::*func)(P1,P2,P3),P1 param1,P2 param2,P3 param3);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		/// @param param2 The 2nd parameter to pass to the function
		/// @param param3 The 3rd parameter to pass to the function
		/// @param param4 The 4th parameter to pass to the function
		/// @param object The object which member-function is to be called
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class P4,class CLASS>
		bool create(CLASS *object,R (CLASS::*func)(P1,P2,P3,P4),P1 param1,P2 param2,P3 param3,P4 param4);
		
		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param func The starting point of the new thread
		/// @param param1 The 1st parameter to pass to the function
		/// @param param2 The 2nd parameter to pass to the function
		/// @param param3 The 3rd parameter to pass to the function
		/// @param param4 The 4th parameter to pass to the function
		/// @param param5 The 5th parameter to pass to the function
		/// @param object The object which member-function is to be called
		///
		/// @return True if everything went right
		///
		/////////////////////////////////////////////////////////////
		template<class R,class P1,class P2,class P3,class P4,class P5,class CLASS>
		bool create(CLASS *object,R (CLASS::*func)(P1,P2,P3,P4,P5),P1 param1,P2 param2,P3 param3,P4 param4,P5 param5);
		

		/////////////////////////////////////////////////////////////
		/// @brief Starts running the thread
		/// 
		/// You have to call fm::Thread::create before fm::Thread::start
		/// 
		/// @return True iff the thread is running after this call
		/// 
		/////////////////////////////////////////////////////////////
		bool start();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current thread
		/// 
		/// Returns NULL if the current thread wasn't started by 
		/// fm::Thread or if called from the main thread
		/// 
		/// @return Pointer to the current thread
		/// 
		/////////////////////////////////////////////////////////////
		static Thread *getCurrentThread();
		
		/////////////////////////////////////////////////////////////
		/// @brief Blocks until The thread is finished
		/// 
		/// If the thread wasn't running before this call 
		/// True is returned
		/// 
		/// @return True iff the thread is not running after this call
		/// 
		/////////////////////////////////////////////////////////////
		bool join();
		
		/////////////////////////////////////////////////////////////
		/// @brief Blocks until The thread is finished or @a timeOut amount of time passes
		/// 
		/// If the thread wasn't running before this call 
		/// True is returned
		/// 
		/// @return True iff the thread is not running after this call
		/// 
		/////////////////////////////////////////////////////////////
		bool join(const fm::Time &timeOut);
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if the thread was requested to exit
		/// 
		/// The identity of the calling thread doesn't matter at this call
		/// To request exit please use fm::Thread::requestExit
		/// When calling fm::Thread::create this value is set to false
		/// 
		/// @param thread The target thread (if NULL, false is returned)
		/// 
		/// @return True iff exiting was requested
		/// 
		/////////////////////////////////////////////////////////////
		static bool isExiting(const Thread *thread);
		
		/////////////////////////////////////////////////////////////
		/// @brief Asks the thread to exit
		/// 
		/// If thread is not running this function has no effect
		/// When fm::Thread::create is called exit request is reset to false
		/// 
		/// @return True if exiting was requested
		/// 
		/////////////////////////////////////////////////////////////
		void requestExit();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a pointer to the implementation
		/// 
		/// @return The pointer (never NULL)
		/// 
		/////////////////////////////////////////////////////////////
		void *getImpl();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get a pointer to the implementation
		/// 
		/// @return The pointer (never NULL)
		/// 
		/////////////////////////////////////////////////////////////
		const void *getImpl() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Forcefully stops the thread
		/// 
		/// You should avoid this function because
		/// killing a thread almost always leads to resource leaks
		/// E.g. on windows xp the used function guaranteed not to free the thread's stack
		/// 
		/// @return True iff everything went right
		/// 
		/////////////////////////////////////////////////////////////
		bool forceExit(); //// !MAY CAUSE MEMORY LEAK!
		
		
		friend class fm::priv::ThreadFuntionCaller;
	};	
}

#endif // FRONTIER_THREAD_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Thread.inl>
#endif
