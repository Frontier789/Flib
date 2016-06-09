////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)		   ///
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

namespace fg
{
    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
	inline typename fm::Enable_if<fg::Is_GLDataType<T>::value,DrawData::Attribute>::type &DrawData::Attribute::operator=(const T (&data)[N])
    {
        return set(data);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    inline typename fm::Enable_if<!fg::Is_GLDataType<T>::value,DrawData::Attribute>::type &DrawData::Attribute::operator=(const T (&data)[N])
    {
        return set(data);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    inline typename fm::Enable_if<fg::Is_GLDataType<T>::value,DrawData::Attribute>::type &DrawData::Attribute::set(const T (&data)[N])
    {
        return set((const T*)data,N);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline typename fm::Enable_if<fg::Is_GLDataType<T>::value,DrawData::Attribute>::type &DrawData::Attribute::set(const T *pointer,fm::Size N)
    {
        return set(1,0,N,fg::Is_GLDataType<T>::enumVal,pointer,sizeof(T)*N);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    inline typename fm::Enable_if<!fg::Is_GLDataType<T>::value,DrawData::Attribute>::type &DrawData::Attribute::set(const T (&data)[N])
    {
        return set((const T*)data,N);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline typename fm::Enable_if<!fg::Is_GLDataType<T>::value,DrawData::Attribute>::type &DrawData::Attribute::set(const T *pointer,fm::Size N)
    {
		return set(T::components,0,N,fg::Is_GLDataType<typename T::component_type>::enumVal,pointer,sizeof(T)*N);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    inline DrawData::DrawCall &DrawData::DrawCall::operator=(const T (&data)[N])
    {
        return set(data,fg::Triangles);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    inline DrawData::DrawCall &DrawData::DrawCall::set(const T (&data)[N],fg::Primitive primitive)
    {
        return set((const T*)data,N,primitive);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline DrawData::DrawCall &DrawData::DrawCall::set(const T *pointer,fm::Size N,fg::Primitive primitive)
    {
        return set(pointer,N,fg::Is_GLDataType<T>::enumVal,sizeof(T)*N,primitive);
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    inline DrawData::DrawCall &DrawData::addDraw(const T (&data)[N],fg::Primitive primitive)
    {
        m_drawCalls.push_back(DrawCall());
        m_drawCalls.back().set(data,primitive);
        return m_drawCalls.back();
    }

    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline DrawData::DrawCall &DrawData::addDraw(const T *pointer,fm::Size N,fg::Primitive primitive)
    {
        m_drawCalls.push_back(DrawCall());
        m_drawCalls.back().set(pointer,N,primitive);
        return m_drawCalls.back();
    }
/*
    //////////////////////////////////////////////////////////////////////////
	template<class PT,class CT,class TT,class NT,fm::Size N>
	inline DrawData::DrawData(const fm::vertex<PT,CT,TT,NT> (&data)[N],fg::Primitive primitive)
	{
		set((const fm::vertex<PT,CT,TT,NT> *)data,N,true,primitive);
	}

    //////////////////////////////////////////////////////////////////////////
	template<class PT,class CT,class TT,class NT>
	inline DrawData::DrawData(const fm::vertex<PT,CT,TT,NT> *pointer,fm::Size N,fg::Primitive primitive)
	{
		set(pointer,N,true,primitive);
	}

    //////////////////////////////////////////////////////////////////////////
	template<class PT,class CT,class TT,class NT,fm::Size N>
	inline DrawData::DrawData &set(const fm::vertex<PT,CT,TT,NT> (&data)[N],bool calcDrawCall,fg::Primitive primtive)
	{
		return set((const fm::vertex<PT,CT,TT,NT> *)data,N,calcDrawCall,primitive);
	}

    //////////////////////////////////////////////////////////////////////////
	template<class PT,class CT,class TT,class NT>
	inline DrawData::DrawData &set(const fm::vertex<PT,CT,TT,NT> *pointer,fm::Size N,bool calcDrawCall,fg::Primitive primtive)
	{
		???
	}
*/
}

#endif // FRONTIER_DRAWDATA_INL_INCLUDED
