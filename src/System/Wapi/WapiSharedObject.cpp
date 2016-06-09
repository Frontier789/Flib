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
#include <FRONTIER/System/Wapi/WapiSharedObject.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/String.hpp>
#include "fmWapiGetLastError.hpp"

namespace fm
{
	namespace Wapi
	{
		////////////////////////////////////////////////////////////
		SharedObject::SharedObject() : m_handle(0)
		{
			
		}

		////////////////////////////////////////////////////////////
		SharedObject::SharedObject(const fm::String &objName) : m_handle(0)
		{
			load(objName);
		}

		////////////////////////////////////////////////////////////
		SharedObject::~SharedObject()
		{
			if (isValid())
				FreeLibrary(m_handle);
		}

		////////////////////////////////////////////////////////////
		fm::Result SharedObject::load(const fm::String &objName)
		{
			if (isValid())
				FreeLibrary(m_handle);
			
			m_handle = LoadLibraryA(objName.str().c_str());
			
			if (!isValid())
				return fm::WapiGetLastError(LoadLibrary);
			
			return fm::Result();
		}

		////////////////////////////////////////////////////////////
		bool SharedObject::isValid() const
		{
			return m_handle;
		}

		////////////////////////////////////////////////////////////
		SharedObject::operator bool() const
		{
			return isValid();
		}

		////////////////////////////////////////////////////////////
		void (*SharedObject::getProcAddress(const char *funcName) const)()
		{
			if (isValid())
			{
				void (*func)() = (void(*)())GetProcAddress(m_handle, funcName);
				
				if (!func)
					return fm::nullPtr;
				
				return func;
			}
			
			return fm::nullPtr; 
		}

		////////////////////////////////////////////////////////////
		HINSTANCE SharedObject::getHandle() const
		{
			return m_handle;
		}
	}
}
