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
#ifndef FRONTIER_WAPITHREAD_HPP_INCLUDED
#define FRONTIER_WAPITHREAD_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/Result.hpp>

#include <FRONTIER/System/macros/dont_include_inl_begin>
#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/System/macros/dont_include_inl_end>

#define FRONTIER_WAPITHREAD
#include <windows.h>

namespace fm
{
	/////////////////////////////////////////////////////////////
	/// @brief Implementation of fm::Thread for windows
	///
	/////////////////////////////////////////////////////////////
	namespace Wapi
	{
		class FRONTIER_API Thread : public fm::NonCopyable
		{
			static DWORD __stdcall startThread(void *param);    ///< The entry point of a new thread
			HANDLE m_id;                                        ///< The id of the thread
			mutable volatile LONG m_isExiting;                  ///< Stores whether the thread is are exiting
			void cleanUp();                                     ///< Internal cleaning function
			fm::Result setEntry(const fm::Delegate<void,fm::Thread *> &runner,fm::Thread *owner); ///< Internal runner function
		public:
			typedef Thread &reference;
			typedef const Thread &const_reference;

            /////////////////////////////////////////////////////////////
			Thread();

            /////////////////////////////////////////////////////////////
			bool start();

            /////////////////////////////////////////////////////////////
			bool join();

            /////////////////////////////////////////////////////////////
			bool join(const fm::Time &timeOut);

            /////////////////////////////////////////////////////////////
			static bool isExiting(const Thread &thread);

            /////////////////////////////////////////////////////////////
			void requestExit();

            /////////////////////////////////////////////////////////////
			bool forceExit();

            /////////////////////////////////////////////////////////////
			bool pause(); //// Only for windows

			friend class fm::Thread;
		};
	}
}

#endif // FRONTIER_WAPITHREAD_HPP_INCLUDED
