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
#include <FRONTIER/System/Posix/PosixTlsPtr.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/NullPtr.hpp>
#include <pthread.h>
#include <string.h>
#include <errno.h>

namespace fm
{
	namespace Posix
	{
		pthread_key_t &getID(void *data)
		{
			return *((pthread_key_t*)data);
		}

		const pthread_key_t &getID(const void *data)
		{
			return *((pthread_key_t*)data);
		}

		/////////////////////////////////////////////////////////////
		TlsPtr::TlsPtr() : m_id(new pthread_key_t)
		{
			if (pthread_key_create((pthread_key_t*)m_id,fm::nullPtr) != 0)
			{
				delete (pthread_key_t*)m_id;
				m_id = fm::nullPtr;
			}
		}

		/////////////////////////////////////////////////////////////
		TlsPtr::~TlsPtr()
		{
			if (m_id)
				pthread_key_delete(getID(m_id));

			delete (pthread_key_t*)m_id;
		}

		/////////////////////////////////////////////////////////////
		bool TlsPtr::setPtr(void *ptr)
		{
			if (!isValid())
				return false;

			if (pthread_setspecific(getID(m_id),ptr) != 0)
				return false;

			return true;
		}

		/////////////////////////////////////////////////////////////
		void *TlsPtr::getPtr() const
		{
			if (!isValid())
				return fm::nullPtr;

			return pthread_getspecific(getID(m_id));
		}

		/////////////////////////////////////////////////////////////
		bool TlsPtr::isValid() const
		{
			return m_id != fm::nullPtr;
		}
	}
}

