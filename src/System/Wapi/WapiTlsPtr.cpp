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
#include <FRONTIER/System/Wapi/WapiTlsPtr.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/FmLog.hpp>
#include "fmWapiPrintLastError.hpp"
#include <windows.h>

namespace fm
{
	namespace Wapi
	{
		DWORD &getID(void *data)
		{
			return *((DWORD*)data);
		}
		
		const DWORD &getID(const void *data)
		{
			return *((DWORD*)data);
		}
		
		/////////////////////////////////////////////////////////////
		TlsPtr::TlsPtr() : m_id(new DWORD)
		{
			getID(m_id) = TlsAlloc();
			if (!isValid())
			{
				fm::WapiPrintLastError(fm::fm_log,TlsAlloc);
				delete (DWORD*)m_id;
				m_id = NULL;
			}
		}

		/////////////////////////////////////////////////////////////
		TlsPtr::~TlsPtr()
		{
			if (m_id)
				if (!TlsFree(getID(m_id)))
					fm::WapiPrintLastError(fm::fm_log,TlsFree);
			
			delete (DWORD*)m_id;
		}

		/////////////////////////////////////////////////////////////
		bool TlsPtr::setPtr(void *ptr)
		{
			if (!isValid())
				return false;
			
			if (!TlsSetValue(getID(m_id),ptr))
			{
				fm::WapiPrintLastError(fm::fm_log,TlsSetValue);
				return false;
			}
			
			return true;	
		}

		/////////////////////////////////////////////////////////////
		void *TlsPtr::getPtr() const
		{
			if (!isValid())
				return NULL;
			
			return TlsGetValue(getID(m_id));
		}

		/////////////////////////////////////////////////////////////
		bool TlsPtr::isValid() const
		{
			return m_id != NULL;
		}
	}
}

