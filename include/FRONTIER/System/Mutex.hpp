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
#ifndef FRONTIER_MUTEX_HPP_INCLUDED
#define FRONTIER_MUTEX_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/API.h>
#define FRONTIER_MUTEX

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Class used to create and manage thread execution locks
	/// 
	/// @ingroup System
	/// 
	/// Upon error this class prompts to fm::fm_log
	/// 
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Mutex : public fm::NonCopyable
	{
		void *m_impl; ///< Stores a pointer to the undelying implementation
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
		/// Upon internal error the mutex is NOT locked and false is returned
		/// 
		/// @return True iff everything went right
		/// 
		/////////////////////////////////////////////////////////////
		bool lock();

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
		/// @return True iff everything went right
		/// 
		/////////////////////////////////////////////////////////////
		bool unLock();
	};	
}

#endif // FRONTIER_MUTEX_HPP_INCLUDED