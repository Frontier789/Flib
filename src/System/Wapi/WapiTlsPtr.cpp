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
#include <FRONTIER/System/Wapi/WapiTlsPtr.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/System/FmLog.hpp>
#include "fmWapiPrintLastError.hpp"
#include <windows.h>
#include <string>

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
		TlsPtr::TlsPtr() : m_id(new DWORD(TLS_OUT_OF_INDEXES))
		{
			init();
		}

		/////////////////////////////////////////////////////////////
		TlsPtr::~TlsPtr()
		{
			cleanUp();
			delete (DWORD*)m_id;
		}

		/////////////////////////////////////////////////////////////
		bool TlsPtr::init()
		{
			if (!isValid())
			{
				getID(m_id) = TlsAlloc();
				if (!isValid())
				{
					fm::WapiPrintLastError(fm::fm_log,TlsAlloc);
					return false;
				}
			}
			return true;
		}

		/////////////////////////////////////////////////////////////
		bool TlsPtr::cleanUp()
		{
			if (isValid())
			{
				if (!TlsFree(getID(m_id)))
				{
					fm::WapiPrintLastError(fm::fm_log,TlsFree);
					return false;
				}
				getID(m_id) = TLS_OUT_OF_INDEXES;
			}
			return true;
		}

		/////////////////////////////////////////////////////////////
		bool TlsPtr::setPtr(void *ptr)
		{
			if (!init())
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
			return getID(m_id) != TLS_OUT_OF_INDEXES;
		}
	}
}

