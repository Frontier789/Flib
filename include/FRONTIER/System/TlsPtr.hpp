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
#ifndef FRONTIER_TLSPTR_HPP_INCLUDED
#define FRONTIER_TLSPTR_HPP_INCLUDED
#include <FRONTIER/System/NonCopyable.hpp>
#define FRONTIER_TLSPTR

namespace fm
{
	template<class T>
	class TlsPtr : public fm::NonCopyable
	{
		void *m_impl;
	public:
		typedef TlsPtr &reference;
		typedef const TlsPtr &const_reference;
		
		TlsPtr();
		~TlsPtr();
		
		T *operator->();
		const T *operator->() const;
		
		T &operator*();
		const T &operator*() const;
		
		reference operator=(T *pointer);
		
		bool isValid() const;
		
		operator T*();
		operator const T*() const;
	};	
}

#endif // FRONTIER_TLSPTR_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/TlsPtr.inl>
#endif
