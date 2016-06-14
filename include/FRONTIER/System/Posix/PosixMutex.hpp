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
#ifndef FRONTIER_POSIXMUTEX_HPP_INCLUDED
#define FRONTIER_POSIXMUTEX_HPP_INCLUDED
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_POSIXMUTEX
#include <pthread.h>

namespace fm
{
	namespace Posix
	{
		class FRONTIER_API Mutex
		{
			pthread_mutex_t m_pmutex; ///< A pointer to the owned posix mutex
		public:
			typedef Mutex &reference;
			typedef const Mutex &const_reference;

			/////////////////////////////////////////////////////////////
			/// @brief Default constructor
			///
			/////////////////////////////////////////////////////////////
			Mutex();

			/////////////////////////////////////////////////////////////
			/// @brief Default destructor
			///
			/// One should unlock the mutex
			/// before destucting it
			///
			/////////////////////////////////////////////////////////////
			~Mutex();

			/////////////////////////////////////////////////////////////
			/// @brief Locks the mutex
			///
			/// Blocks until the mutex is not available for lock.
			///
			/////////////////////////////////////////////////////////////
			void lock();

			/////////////////////////////////////////////////////////////
			/// @brief Tries to lock the mutex
			///
			/// If the mutex is already locked this function
			/// returns (false) immediately
			///
			/// @return True iff the mutex got locked
			///
			/////////////////////////////////////////////////////////////
			bool attemptLock();

			/////////////////////////////////////////////////////////////
			/// @brief Unlocks the mutex
			///
			/// After this call the mutex will not be locked
			///
			/////////////////////////////////////////////////////////////
			void unLock();
		};
	}
}

#endif // FRONTIER_POSIXMUTEX_HPP_INCLUDED
