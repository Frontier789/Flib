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
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->

// used to lock mutexes if protecting shared variables is enabled
// mutex needn't exist if FRONTIER_PROTECT_SHARED_VARIABLES is not defined
#ifndef FRONTIER_LOCK_MUTEX(mutex)
	#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		#define FRONTIER_LOCK_MUTEX(mutex) (mutex).lock()
	#else
		#define FRONTIER_LOCK_MUTEX(mutex) (void)
#endif

// used to unlock mutexes if protecting shared variables is enabled
// mutex needn't exist if FRONTIER_PROTECT_SHARED_VARIABLES is not defined
#ifndef FRONTIER_UNLOCK_MUTEX(mutex)
	#ifdef FRONTIER_PROTECT_SHARED_VARIABLES
		#define FRONTIER_LOCK_UNMUTEX(mutex) (mutex).unLock()
	#else
		#define FRONTIER_LOCK_UNMUTEX(mutex) (void)
#endif