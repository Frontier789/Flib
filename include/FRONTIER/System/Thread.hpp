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
#ifndef FRONTIER_THREAD_HPP_INCLUDED
#define FRONTIER_THREAD_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/TlsPtr.hpp>
#include <FRONTIER/System/Result.hpp>
#include <FRONTIER/System/Time.hpp>

#include <FRONTIER/System/macros/dont_include_inl_begin>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_THREAD

namespace fm
{
    /////////////////////////////////////////////////////////////
	/// @brief Class used to launch and manage a new thread (see <a href="http://en.wikipedia.org/wiki/Thread_%28computing%29">this wikipedia article</a> for further information)
	///
	/// @ingroup System
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Thread : public fm::NonCopyable
	{
		static fm::TlsPtr<fm::Thread> m_currentThread; ///< Holds the current thread
		void *m_impl;   ///< Stores a pointer to the undelying implementation
	public:
		typedef Thread &reference;
		typedef const Thread &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/////////////////////////////////////////////////////////////
		Thread();

		/////////////////////////////////////////////////////////////
		/// @brief Constructor with entry point
		///
		/// @param delegate The entry point
		///
		/////////////////////////////////////////////////////////////
		Thread(const fm::Delegate<void,Thread *> &delegate);

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/// Calls fm::Thread::requestExit iff thread is running
		///
		/////////////////////////////////////////////////////////////
		~Thread();

		/////////////////////////////////////////////////////////////
		/// @brief Create the thread but not start it
		///
		/// If the thread is running when this call is made
		/// fm::Thread::requestExit and fm::Thread::join are called.
		///
		/// @param delegate The starting point of the new thread
		///
		/// @return The error-state of the function
		///
		/////////////////////////////////////////////////////////////
		fm::Result create(const fm::Delegate<void,Thread *> &delegate);

		/////////////////////////////////////////////////////////////
		/// @brief Starts running the thread
		///
		/// Starts the thread iff it has an entry point
		/// Success means the thread is running after this call
		///
		/// @return True iff the thread was started
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
		/// @return True iff the thread is not running after this call
		///
		/////////////////////////////////////////////////////////////
		bool join();

		/////////////////////////////////////////////////////////////
		/// @brief Blocks until The thread is finished or @a timeOut amount of time passes
		///
		/// @return True iff the thread is not running after this call
		///
		/////////////////////////////////////////////////////////////
		bool join(const fm::Time &timeOut);

		/////////////////////////////////////////////////////////////
		/// @brief Check if the thread was requested to exit
		///
		/// The identity of the calling thread doesn't matter at this call
		/// To request exit, use fm::Thread::requestExit
		///
		/// @param thread The target thread
		///
		/// @return True iff exiting was requested
		///
		/////////////////////////////////////////////////////////////
		static bool isExiting(const Thread &thread);

		/////////////////////////////////////////////////////////////
		/// @brief Set the current thread
		///
		/// Generally this function is called internally
		///
		/// @param thread The new current thread
		///
		/////////////////////////////////////////////////////////////
		static void setCurrentThread(Thread *thread);

		/////////////////////////////////////////////////////////////
		/// @brief Asks the thread to exit
		///
		/// If thread is not running this function has no effect
		///
		/////////////////////////////////////////////////////////////
		void requestExit();

		/////////////////////////////////////////////////////////////
		/// @brief Get a pointer to the implementation
		///
		/// @return The pointer
		///
		/////////////////////////////////////////////////////////////
		void *getImpl();

		/////////////////////////////////////////////////////////////
		/// @brief Get a pointer to the implementation
		///
		/// @return The pointer
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
		/// @return True iff successfully exited
		///
		/////////////////////////////////////////////////////////////
		bool forceExit(); //// !MAY CAUSE MEMORY LEAK!
	};
}

#endif // FRONTIER_THREAD_HPP_INCLUDED
