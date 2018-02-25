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

namespace fg
{
    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
	inline Attribute &Attribute::operator=(const T (&data)[N])
    {
        return set(data);
    }
    
    //////////////////////////////////////////////////////////////////////////
    template<class T>
	inline Attribute &Attribute::operator=(std::initializer_list<T> data)
    {
        return set(data.begin(),data.size());
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    inline typename std::enable_if<fg::Is_GLDataType<T>::value,Attribute>::type &Attribute::set(const T (&data)[N],fg::Buffer::Usage bufferUsage,fm::Size instancesPerUpdate)
    {
        return set((const T*)data,N,bufferUsage,instancesPerUpdate);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    inline typename std::enable_if<!fg::Is_GLDataType<T>::value,Attribute>::type &Attribute::set(const T (&data)[N],fg::Buffer::Usage bufferUsage,fm::Size instancesPerUpdate)
    {
        return set((const T*)data,N,bufferUsage,instancesPerUpdate);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline typename std::enable_if<fg::Is_GLDataType<T>::value,Attribute>::type &Attribute::set(const T *pointer,fm::Size N,fg::Buffer::Usage bufferUsage,fm::Size instancesPerUpdate)
    {
        return set(1,0,N,fg::Is_GLDataType<T>::enumVal,pointer,sizeof(T)*N,bufferUsage,instancesPerUpdate);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline typename std::enable_if<!fg::Is_GLDataType<T>::value,Attribute>::type &Attribute::set(const T *pointer,fm::Size N,fg::Buffer::Usage bufferUsage,fm::Size instancesPerUpdate)
    {
		return set(T::components,0,N,fg::Is_GLDataType<typename T::component_type>::enumVal,pointer,sizeof(T)*N,bufferUsage,instancesPerUpdate);
    }
}

#endif // FRONTIER_ATTRIBUTE_INL_INCLUDED
