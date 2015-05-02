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
#ifndef FRONTIER_WAPITLSPTR_INL_INCLUDED
#define FRONTIER_WAPITLSPTR_INL_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/macros/OS.h>

#ifdef FRONTIER_OS_WINDOWS
	#include "Wapi/WapiTlsPtr.hpp"
	namespace fm
	{
		namespace priv
		{
			typedef fm::Wapi::TlsPtr TlsPtr;
		}
	}
#elif defined(FRONTIER_OS_LINUX)
	#include "Posix/PosixTlsPtr.hpp"
	namespace fm
	{
		namespace priv
		{
			typedef fm::Posix::TlsPtr TlsPtr;
		}
	}
#else
	#ifndef FRONTIER_NO_TLSPTR
		#warning No TlsPtr!
		#define FRONTIER_NO_TLSPTR
	#endif
#endif

#ifndef FRONTIER_NO_TLSPTR

namespace fm
{
	/////////////////////////////////////////////////////////////
	template<class T>
	TlsPtr<T>::TlsPtr() : m_impl(new fm::priv::TlsPtr)
	{
		
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	TlsPtr<T>::~TlsPtr()
	{
		delete (fm::priv::TlsPtr*)m_impl;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	T *TlsPtr<T>::operator->()
	{
		return (T*)(UintPtr)((fm::priv::TlsPtr*)m_impl)->getPtr();
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	const T *TlsPtr<T>::operator->() const
	{
		return (const T*)(UintPtr)((const fm::priv::TlsPtr*)m_impl)->getPtr();
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	T &TlsPtr<T>::operator*()
	{
		return *((T*)(UintPtr)((fm::priv::TlsPtr*)m_impl)->getPtr());
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	const T &TlsPtr<T>::operator*() const
	{
		return *((const T*)(UintPtr)((const fm::priv::TlsPtr*)m_impl)->getPtr());
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	typename TlsPtr<T>::reference TlsPtr<T>::operator=(T *pointer)
	{
		((fm::priv::TlsPtr*)m_impl)->setPtr((void*)(UintPtr)pointer);
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	typename TlsPtr<T>::reference TlsPtr<T>::set(T *pointer)
	{
		((fm::priv::TlsPtr*)m_impl)->setPtr((void*)(UintPtr)pointer);
		return *this;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	bool TlsPtr<T>::isValid() const
	{
		return ((fm::priv::TlsPtr*)m_impl)->isValid();
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	T *TlsPtr<T>::get()
	{
		return (T*)(UintPtr)((fm::priv::TlsPtr*)m_impl)->getPtr();
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	const T *TlsPtr<T>::get() const
	{
		return (const T*)(UintPtr)((const fm::priv::TlsPtr*)m_impl)->getPtr();
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	TlsPtr<T>::operator T*()
	{
		return (T*)(UintPtr)((fm::priv::TlsPtr*)m_impl)->getPtr();
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	TlsPtr<T>::operator const T*() const
	{
		return (const T*)(UintPtr)((const fm::priv::TlsPtr*)m_impl)->getPtr();
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	TlsPtr<T>::operator bool() const
	{
		return ((const fm::priv::TlsPtr*)m_impl)->getPtr()!=0;
	}
}

#endif

#endif // FRONTIER_WAPITLSPTR_HPP_INCLUDED
