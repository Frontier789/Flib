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
#ifndef FRONTIER_ATTRIBUTE_INL_INCLUDED
#define FRONTIER_ATTRIBUTE_INL_INCLUDED
#include <FRONTIER/GL/Is_GLDataType.hpp>

namespace fg
{
    /////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    Attribute &Attribute::operator=(const T (&data)[N])
    {
        return set((const T*)data,N);
    }
 
    /////////////////////////////////////////////////////////////
    template<class T>
    Attribute &Attribute::operator=(std::initializer_list<T> data)
    {
        return set(data.begin(),data.size());
    }

    /////////////////////////////////////////////////////////////
    template<class T>
    Attribute &Attribute::set(const T *pointer,fm::Size N,fm::Size instancesPerUpdate)
    {
        if (ownBuffer)
            delete buf;
        
        buf = new Buffer;
        buf->setData(pointer,N);
        
        this->instancesPerUpdate = instancesPerUpdate;
        compCount = buf->getCompCount();
        compType  = buf->getCompType();
        stride    = 0;
        offset    = 0;
        count     = buf->getItemCount();
        ownBuffer = true;
        
        return *this;
    }
    
    /////////////////////////////////////////////////////////////
    template<class T>
    Attribute &Attribute::set(fg::Buffer &buf,fm::Size stride,fm::Size offset,fm::Size instancesPerUpdate)
    {
        if (ownBuffer)
            delete this->buf;
        
        this->buf = &buf;
        
        this->instancesPerUpdate = instancesPerUpdate;
        compCount       = Is_GLDataType<T>::compCount;
        compType        = Is_GLDataType<T>::enumVal;
        this->stride    = stride;
        this->offset    = offset;
        count           = buf.getItemCount() - offset / buf.getItemSize();
        ownBuffer       = false;
        
        return *this;
    }
}

#endif // FRONTIER_ATTRIBUTE_INL_INCLUDED
