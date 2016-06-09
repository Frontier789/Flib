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
#ifndef FRONTIER_REF_HPP_INCLUDED
#define FRONTIER_REF_HPP_INCLUDED
#include <FRONTIER/System/NullPtr.hpp>
#define FRONTIER_REF

namespace fm
{
    template<class T>
    class Ref
    {
        T *m_ptr;

    public:
		/////////////////////////////////////////////////////////////
        Ref(T *objPtr = fm::nullPtr);

		/////////////////////////////////////////////////////////////
        Ref(fm::NullPtr);

		/////////////////////////////////////////////////////////////
        Ref(T &objRef);

		/////////////////////////////////////////////////////////////
		T *operator->();

		/////////////////////////////////////////////////////////////
		const T *operator->() const;

		/////////////////////////////////////////////////////////////
		T &operator*();

		/////////////////////////////////////////////////////////////
		const T &operator*() const;

		/////////////////////////////////////////////////////////////
		Ref &operator=(T *pointer);

		/////////////////////////////////////////////////////////////
		operator T*();

		/////////////////////////////////////////////////////////////
		operator const T*() const;

		/////////////////////////////////////////////////////////////
		operator bool() const;
    };
}

#endif // FRONTIER_REF_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/System/Ref.inl>
#endif
