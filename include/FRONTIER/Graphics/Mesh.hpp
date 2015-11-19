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
#ifndef FRONTIER_MESH_HPP_INCLUDED
#define FRONTIER_MESH_HPP_INCLUDED

#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/Graphics/Primitive.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Graphics/Buffer.hpp>

#define FRONTIER_MESH
#include <vector>

namespace fg
{
    class FRONTIER_API Mesh
    {
    public:

        enum AssociationPoint {
            Unused = 0,
            Position,
            Color,
            Normal,
            TextureUV,
            Tangent,
            Bitangent,
            Custom
        };

        class Attribute
        {
        public:

            AssociationPoint type;
            fm::Size components;
            fm::Size stride;
            unsigned long componentType;
            const void *ptr;
            fg::Buffer *buf;
            bool ownBuffer;

            template<class T>
            Attribute(AssociationPoint type,T *pointer,fm::Size N,bool genBuf);

            ~Attribute();
        };
        class IndexData
        {
        public:

            fm::Size indexCount;
            unsigned long componentType;
            const void *ptr;
            fg::Buffer *buf;
            bool ownBuffer;
            fg::Primitive primitive;

            template<class T>
            IndexData(T *pointer,fm::Size N,bool genBuf,fg::Primitive primitive);

            ~IndexData();
        };

        virtual ~Mesh();

        std::vector<Attribute*> attrs;
        std::vector<IndexData*> indices;
    };
}

#endif // FRONTIER_MESH_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
    #include <FRONTIER/Graphics/Mesh.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
