#include <FRONTIER/System/NonCopyable.hpp>
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
#include <FRONTIER/Config.hpp>
#include <windows.h>

namespace fm
{
	namespace priv
	{
		class ThreadFuntionCaller;
	}
	namespace Wapi
	{
		class FRONTIER_API Thread : public fm::NonCopyable
		{
			static DWORD __stdcall startThread(void *param);
			HANDLE m_id;
			volatile LONG  m_isExiting;
			fm::priv::ThreadFuntionCaller *m_storage;
			void cleanUp();
			bool create(fm::priv::ThreadFuntionCaller *runner);
		public:
			typedef Thread &reference;
			typedef const Thread &const_reference;
			Thread();
			bool start();
			
			static Thread *getCurrentThread();
			
			bool join();
			bool join(const fm::Time &timeOut);
			
			bool isExiting() const;
			void requestExit();
			
			bool forceExit();
			
			bool pause(); //// Only for windows
			
			friend class fm::Thread;
		};
	}
}
