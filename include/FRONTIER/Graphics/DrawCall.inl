////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)		   ///
///																	   ///
/// Flib is licensed under the terms of GNU GPL.					   ///
/// Therefore you may freely use it in your project,				   ///
/// modify it, redistribute it without any warranty on the			   ///
/// condition that this disclaimer is not modified/removed.			   ///
/// You may not misclaim the origin of this software.				   ///
///																	   ///
/// If you use this software in your program/project a				   ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.							   ///
///																	   ///
/// You should have received a copy of GNU GPL with this software	   ///
///																	   ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_DRAW_CALL_INL_INCLUDED
#define FRONTIER_DRAW_CALL_INL_INCLUDED
#include <FRONTIER/GL/Is_GLDataType.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<class T,fm::Size N>
	inline DrawCall &DrawCall::operator=(const T (&data)[N])
	{
		return set(data,fg::Triangles);
	}

	/////////////////////////////////////////////////////////////
	template<class T,fm::Size N>
	inline DrawCall &DrawCall::set(const T (&data)[N],fg::Primitive primitive)
	{
		return set(data,N,primitive);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline DrawCall &DrawCall::set(const T *pointer,fm::Size N,fg::Primitive primitive)
	{
		return set(pointer,N,fg::Is_GLDataType<T>::enumVal,sizeof(T)*N,primitive);
	}
}

#endif // FRONTIER_DRAW_CALL_INL_INCLUDED
