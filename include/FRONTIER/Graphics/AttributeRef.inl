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
#ifndef FRONTIER_ATTRIBUTE_REF_INL_INCLUDED
#define FRONTIER_ATTRIBUTE_REF_INL_INCLUDED
#include <FRONTIER/Graphics/Attribute.hpp>

#include <FRONTIER/System/util/dont_include_inl_begin>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/System/util/dont_include_inl_end>

namespace fg
{
    /////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    Attribute &AttributeRef::operator=(const T (&data)[N])
    {
        return m_drawData.getAttribute(m_assoc).set((const T*)data,N);
    }
 
    /////////////////////////////////////////////////////////////
    template<class T>
    Attribute &AttributeRef::operator=(std::initializer_list<T> data)
    {
        return m_drawData.getAttribute(m_assoc).set(data.begin(),data.size());
    }

    /////////////////////////////////////////////////////////////
    template<class T>
    Attribute &AttributeRef::set(const T *pointer,fm::Size N,fm::Size instancesPerUpdate)
    {
        return m_drawData.getAttribute(m_assoc).set(pointer,N,instancesPerUpdate);
    }
}

#endif // FRONTIER_ATTRIBUTE_REF_INL_INCLUDED
