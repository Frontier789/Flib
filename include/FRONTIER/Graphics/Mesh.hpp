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

#include <FRONTIER/System/type_traits/Enable_if.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/Primitive.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Graphics/Buffer.hpp>

#define FRONTIER_MESH
#include <vector>

namespace fg
{
    class FRONTIER_API Mesh : public fm::NonCopyable
    {
    public:
    	
		/////////////////////////////////////////////////////////////
		class IndexArrayHolder
		{
		public:
			void *ptr;
			bool use16bits;
			fm::Size N;

			IndexArrayHolder(fm::Size N,bool use16bits);
			IndexArrayHolder(fm::Size N,fm::Size maxIndex);

			~IndexArrayHolder();

			void set(fm::Size i,fm::Uint32 val);

			fm::Uint32 get(fm::Size i);
		};

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
            fm::Size count;
            unsigned long componentType;
            const void *ptr;
            fg::Buffer *buf;
            bool ownBuffer;

            Attribute(AssociationPoint type = Mesh::Unused,
                      fm::Size components = 0,
                      fm::Size stride = 0,
                      fm::Size count = 0,
                      unsigned long componentType = 0,
                      const void *ptr = fm::nullPtr,
                      fg::Buffer *buf = fm::nullPtr,
                      bool ownBuffer = false);

            template<class T>
            Attribute(AssociationPoint type,T *pointer,fm::Size N,bool genBuf = true,typename fm::Enable_if<fg::Is_GLDataType<T>::value >::type* = fm::nullPtr);

            template<class T,fm::Size N>
            Attribute(AssociationPoint type,const T (&pointer)[N],bool genBuf = true,typename fm::Enable_if<fg::Is_GLDataType<T>::value >::type* = fm::nullPtr);

            template<class T>
            Attribute(AssociationPoint type,T *pointer,fm::Size N,bool genBuf = true,typename fm::Enable_if<!fg::Is_GLDataType<T>::value >::type* = fm::nullPtr);

            template<class T,fm::Size N>
            Attribute(AssociationPoint type,const T (&pointer)[N],bool genBuf = true,typename fm::Enable_if<!fg::Is_GLDataType<T>::value >::type* = fm::nullPtr);

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
			IndexData(T *pointer,fm::Size N,fg::Primitive primitive,bool genBuf = true);

            template<class T,fm::Size N>
            IndexData(const T (&pointer)[N],fg::Primitive primitive,bool genBuf = true);

			IndexData(const IndexArrayHolder &indices,fg::Primitive primitive,bool genBuf = true);

            ~IndexData();
        };

        std::vector<Attribute*> attrs;
        std::vector<IndexData*> indices;

        virtual ~Mesh();

        void reset();

        Attribute *operator[](AssociationPoint type);

        static Mesh &getSphere(Mesh &output,float radius = 1,fm::Size W = 20,fm::Size H = 20,float (*radiusModifier)(float &,float &) = fm::nullPtr);
        static Mesh &getTorus(Mesh &output,float majorR = 1,float minorR = .5,fm::Size W = 30,fm::Size H = 15,float (*radiusModifier)(float &,float &) = fm::nullPtr);
        static Mesh &getCube(Mesh &output,float size = 1,fm::Size N = 2,float (*radiusModifier)(float &,float &) = fm::nullPtr);
        static Mesh &getCylinder(Mesh &output,float radius = 1,float height = 1,fm::Size W = 20,fm::Size H = 2,float (*radiusModifier)(float &,float &) = fm::nullPtr);
        static Mesh &getCone(Mesh &output,float radius = 1,float height = 1,fm::Size N = 20,float (*radiusModifier)(float &,float &) = fm::nullPtr);

        static Mesh &calcNormals( Mesh &mesh,bool joinSamePts = true);
        static Mesh &calcTangents(Mesh &mesh,bool joinSamePts = true);
    };

    Mesh::AssociationPoint operator+(const Mesh::AssociationPoint &pt,int delta);
    Mesh::AssociationPoint operator-(const Mesh::AssociationPoint &pt,int delta);
}

#endif // FRONTIER_MESH_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
    #include <FRONTIER/Graphics/Mesh.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
