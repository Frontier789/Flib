////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_BUFFER_INL_INCLUDED
#define FRONTIER_BUFFER_INL_INCLUDED
#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>

namespace fg
{
	////////////////////////////////////////////////////////////
	template<class T,fm::Size S>
	inline fm::Result Buffer::setData(const T (&data)[S])
	{
		return setData(&data[0],sizeof(data)/sizeof(*data));
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Result Buffer::setData(const T *data,fm::Size dataCount)
	{
		return setData((const void*)data,Is_GLDataType<T>::enumVal,Is_GLDataType<T>::compCount,sizeof(T)*dataCount);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T,fm::Size S>
	inline fm::Result Buffer::updateData(const T (&data)[S],fm::Size byteOffset)
	{
		return updateData(data,S,byteOffset);
	}
	
	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Result Buffer::updateData(const T *data,fm::Size dataCount,fm::Size byteOffset)
	{
		return updateData((const void *)data,dataCount * sizeof(T),byteOffset);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline T *Buffer::map(BufferAccess access)
	{
		void *ptr = nullptr;
		mapData(ptr,access);
		return (T*)ptr;
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	inline fm::Result Buffer::map(T *&ptr,BufferAccess access)
	{
		void *vptr = nullptr;
		fm::Result res = mapData(vptr,access);
		ptr = (T*)vptr;
		return res;
	}
}
#endif //FRONTIER_BUFFER_INL_INCLUDED
