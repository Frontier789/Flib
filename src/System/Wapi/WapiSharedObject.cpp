#include <FRONTIER/System/Wapi/WapiSharedObject.hpp>
#include <FRONTIER/System/FmLog.hpp>
#include "fmWapiPrintLastError.hpp"

namespace fm
{
	namespace Wapi
	{
		////////////////////////////////////////////////////////////
		SharedObject::SharedObject() : m_handle(0)
		{
			
		}

		////////////////////////////////////////////////////////////
		SharedObject::SharedObject(const char *objName) : m_handle(0)
		{
			load(objName);
		}

		////////////////////////////////////////////////////////////
		SharedObject::~SharedObject()
		{
			if (m_handle)
				if (!FreeLibrary(m_handle))
					fm::WapiPrintLastError(fm::fm_log,FreeLibrary);
		}

		////////////////////////////////////////////////////////////
		bool SharedObject::load(const char *objName)
		{
			if (m_handle)
				if (!FreeLibrary(m_handle))
					fm::WapiPrintLastError(fm::fm_log,FreeLibrary);
			
			m_handle = LoadLibraryA(objName);
			
			if (!m_handle)
			{
				fm::WapiPrintLastError(fm::fm_log,LoadLibrary);
				return false;
			}
			
			return true;
		}

		////////////////////////////////////////////////////////////
		bool SharedObject::isValid() const
		{
			return m_handle;
		}

		////////////////////////////////////////////////////////////
		SharedObject::operator bool() const
		{
			return m_handle;
		}

		////////////////////////////////////////////////////////////
		void (*SharedObject::getProcAddress(const char *funcName) const)()
		{
			if (m_handle)
			{
				void (*func)() = (void(*)())GetProcAddress(m_handle, funcName);
				if (!func)
				{
					fm::WapiPrintLastError(fm::fm_log,GetProcAddress);
					return (void(*)())0;
				}
				return func;
			}
			
			return (void(*)())0; 
		}

		////////////////////////////////////////////////////////////
		HINSTANCE SharedObject::getHandle() const
		{
			return m_handle;
		}
	}
}
