////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)		   ///
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
#ifndef FRONTIER_DRAWDATA_INL_INCLUDED
#define FRONTIER_DRAWDATA_INL_INCLUDED
#include <FRONTIER/Graphics/DrawCall.hpp>

namespace fg
{
    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    inline DrawCall &DrawData::addDraw(const T (&data)[N],fg::Primitive primitive)
    {
		return addDraw(data,N,primitive);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline DrawCall &DrawData::addDraw(const T *pointer,fm::Size N,fg::Primitive primitive)
    {
        m_drawCalls.push_back(DrawCall());
        m_drawCalls.back().set(pointer,N,primitive);
        return m_drawCalls.back();
    }
}

#endif // FRONTIER_DRAWDATA_INL_INCLUDED
