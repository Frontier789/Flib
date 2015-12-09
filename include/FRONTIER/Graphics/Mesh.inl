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
#ifndef FRONTIER_MESH_INL_INCLUDED
#define FRONTIER_MESH_INL_INCLUDED
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/NullPtr.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
    template<class T>
    Mesh::Attribute::Attribute(AssociationPoint type,
                               T *pointer,
                               fm::Size N,
                               bool genBuf,
                               typename fm::Enable_if<fg::Is_GLDataType<T>::value >::type*) : type(type),
                                                                                              components(1),
                                                                                              stride(0),
                                                                                              count(N),
                                                                                              componentType(fg::Is_GLDataType<T>::enumVal),
                                                                                              ownBuffer(genBuf)
    {
        if (genBuf)
            buf = new fg::Buffer,
            ptr = fm::nullPtr,
            buf->setData(pointer,N*sizeof(T));
        else
            ptr = pointer,
            buf = fm::nullPtr;
    }

	/////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    Mesh::Attribute::Attribute(AssociationPoint type,
                               const T (&pointer)[N],
                               bool genBuf,
                               typename fm::Enable_if<fg::Is_GLDataType<T>::value >::type*) : type(type),
                                                                                              components(T::components),
                                                                                              stride(0),
                                                                                              count(N),
                                                                                              componentType(fg::Is_GLDataType<T>::enumVal),
                                                                                              ownBuffer(genBuf)
    {
        if (genBuf)
            buf = new fg::Buffer,
            ptr = fm::nullPtr,
            buf->setData(pointer,N*sizeof(T));
        else
            ptr = pointer,
            buf = fm::nullPtr;
    }

	/////////////////////////////////////////////////////////////
    template<class T>
    Mesh::Attribute::Attribute(AssociationPoint type,
                               T *pointer,
                               fm::Size N,
                               bool genBuf,
                               typename fm::Enable_if<!fg::Is_GLDataType<T>::value >::type*) : type(type),
                                                                                               components(T::components),
                                                                                               stride(0),
                                                                                               count(N),
                                                                                               componentType(fg::Is_GLDataType<typename T::component_type>::enumVal),
                                                                                               ownBuffer(genBuf)
    {
        if (genBuf)
            buf = new fg::Buffer,
            ptr = fm::nullPtr,
            buf->setData(pointer,N*sizeof(T));
        else
            ptr = pointer,
            buf = fm::nullPtr;
    }

	/////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    Mesh::Attribute::Attribute(AssociationPoint type,
                               const T (&pointer)[N],
                               bool genBuf,
                               typename fm::Enable_if<!fg::Is_GLDataType<T>::value >::type*) : type(type),
                                                                                               components(1),
                                                                                               stride(0),
                                                                                               count(N),
                                                                                               componentType(fg::Is_GLDataType<typename T::component_type>::enumVal),
                                                                                               ownBuffer(genBuf)
    {
        if (genBuf)
            buf = new fg::Buffer,
            ptr = fm::nullPtr,
            buf->setData(pointer,N*sizeof(T));
        else
            ptr = pointer,
            buf = fm::nullPtr;
    }

	/////////////////////////////////////////////////////////////
    template<class T>
    Mesh::IndexData::IndexData(T *pointer,fm::Size N,bool genBuf,fg::Primitive primitive) : indexCount(N),
                                                                                            componentType(fg::Is_GLDataType<T>::enumVal),
                                                                                            ownBuffer(genBuf),
                                                                                            primitive(primitive)
    {
        if (genBuf)
            buf = new fg::Buffer(fg::IndexBuffer),
            ptr = fm::nullPtr,
            buf->setData(pointer,N*sizeof(T));
        else
            ptr = pointer,
            buf = fm::nullPtr;
    }

	/////////////////////////////////////////////////////////////
    template<class T,fm::Size N>
    Mesh::IndexData::IndexData(const T (&pointer)[N],bool genBuf,fg::Primitive primitive) : indexCount(N),
                                                                                            componentType(fg::Is_GLDataType<T>::enumVal),
                                                                                            ownBuffer(genBuf),
                                                                                            primitive(primitive)
    {
        if (genBuf)
            buf = new fg::Buffer(fg::IndexBuffer),
            ptr = fm::nullPtr,
            buf->setData(pointer,N*sizeof(T));
        else
            ptr = pointer,
            buf = fm::nullPtr;
    }
}


#endif // FRONTIER_MESH_HPP_INCLUDED
