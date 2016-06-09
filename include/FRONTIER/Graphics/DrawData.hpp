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
#ifndef FRONTIER_DRAWDATA_HPP_INCLUDED
#define FRONTIER_DRAWDATA_HPP_INCLUDED

#include <FRONTIER/System/type_traits/Enable_if.hpp>
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/NonCopyable.hpp>
#include <FRONTIER/Graphics/Primitive.hpp>
#include <FRONTIER/GL/Is_GLDataType.hpp>
#include <FRONTIER/System/macros/API.h>
#include <FRONTIER/Graphics/Buffer.hpp>
#include <FRONTIER/System/NullPtr.hpp>
#include <FRONTIER/System/Vertex.hpp>

#define FRONTIER_DRAWDATA
#include <vector>
#include <map>

namespace fg
{
	class IndexArrayHolder;
	class Mesh;

	class Assoc
	{
	public:
		enum Point {
			Unused = 0,
			Position,
			Color,
			Normal,
			TextureUV,
			Tangent,
			Bitangent,
			Custom
		};
	};

	Assoc::Point operator+(const Assoc::Point &pt,int delta);
	Assoc::Point operator-(const Assoc::Point &pt,int delta);

	typedef Assoc::Point AssociationPoint;

	class FRONTIER_API DrawData : public fm::NonCopyable
	{
	public:

		class Attribute
		{
		public:
			fm::Size components;
			fm::Size stride;
			fm::Size count;
			fm::Size componentType;
			fg::Buffer *buf;
			bool ownBuffer;

			Attribute(fm::Size components = 0,
					  fm::Size stride = 0,
					  fm::Size count = 0,
					  fm::Size componentType = 0,
					  fg::Buffer *buffer = fm::nullPtr,
					  bool ownBuffer = true);

			Attribute(const Attribute &attr);

            Attribute &operator=(const Attribute &attr);

            template<class T,fm::Size N>
			inline typename fm::Enable_if<fg::Is_GLDataType<T>::value,Attribute>::type &operator=(const T (&data)[N]);

            template<class T,fm::Size N>
            inline typename fm::Enable_if<!fg::Is_GLDataType<T>::value,Attribute>::type &operator=(const T (&data)[N]);

            template<class T,fm::Size N>
            inline typename fm::Enable_if<fg::Is_GLDataType<T>::value,Attribute>::type &set(const T (&data)[N]);

            template<class T>
            inline typename fm::Enable_if<fg::Is_GLDataType<T>::value,Attribute>::type &set(const T *pointer,fm::Size N);

            template<class T,fm::Size N>
            inline typename fm::Enable_if<!fg::Is_GLDataType<T>::value,Attribute>::type &set(const T (&data)[N]);

            template<class T>
			inline typename fm::Enable_if<!fg::Is_GLDataType<T>::value,Attribute>::type &set(const T *pointer,fm::Size N);

            Attribute &set(fm::Size components,
                           fm::Size stride,
                           fm::Size count,
                           fm::Size componentType,
                           const void *pointer,
                           fm::Size bytesToCopy);

            Attribute &set(fm::Size components,
                           fm::Size stride,
                           fm::Size count,
                           fm::Size componentType,
                           fg::Buffer *buffer = fm::nullPtr,
                           bool ownBuffer = true);

            ~Attribute();
		};

		class DrawCall
		{
		public:

			fg::Primitive primitive;
			fm::Size indexCount;
			fm::Size drawBeg;
			fm::Size drawLen;

			fm::Size componentType;
			fg::Buffer *buf;
			bool ownBuffer;

			DrawCall(fg::Primitive primitive = fg::Triangles,
                     fm::Size indexCount = 0,
                     fm::Size drawBeg = 0,
                     fm::Size drawLen = 0,
                     fm::Size compType = 0,
                     fg::Buffer *buffer = fm::nullPtr,
                     bool ownBuffer = true);

            DrawCall(const DrawCall &drawCall);

            DrawCall &operator=(const DrawCall &drawCall);

            template<class T,fm::Size N>
            inline DrawCall &operator=(const T (&data)[N]);

            DrawCall &operator=(const IndexArrayHolder &indices);

            template<class T,fm::Size N>
            inline DrawCall &set(const T (&data)[N],fg::Primitive primitive);

            template<class T>
            inline DrawCall &set(const T *pointer,fm::Size N,fg::Primitive primitive);

            DrawCall &set(const void *ptr,fm::Size indCount,fm::Size compType,fm::Size bytesToCopy,fg::Primitive primitive);

            DrawCall &set(const IndexArrayHolder &indices,fg::Primitive primitive);

            DrawCall &set(fm::Size beg,fm::Size len,fg::Primitive primitive);

            DrawCall &setRange(fm::Size beg,fm::Size len);

            ~DrawCall();
		};

    protected:
		std::map<AssociationPoint,Attribute> m_attrs;
		std::vector<DrawCall> m_drawCalls;

    public:
		DrawData();

		explicit DrawData(const Mesh &m);

/*
		template<class PT,class CT,class TT,class NT,fm::Size N>
		inline explicit DrawData(const fm::vertex<PT,CT,TT,NT> (&data)[N],fg::Primitive primitive = fg::Triangles);

		template<class PT,class CT,class TT,class NT>
		inline DrawData(const fm::vertex<PT,CT,TT,NT> *pointer,fm::Size N,fg::Primitive primitive = fg::Triangles);


		template<class PT,class CT,class TT,class NT,fm::Size N>
		inline DrawData &set(const fm::vertex<PT,CT,TT,NT> (&data)[N],bool calcDrawCall = true,fg::Primitive primitive = fg::Triangles)

		template<class PT,class CT,class TT,class NT>
		inline DrawData &set(const fm::vertex<PT,CT,TT,NT> *pointer,fm::Size N,bool calcDrawCall = true,fg::Primitive primitive = fg::Triangles)
*/
        /* access attribute */
		Attribute &operator[](AssociationPoint type);
		const Attribute &operator[](AssociationPoint type) const;

		/* assign mesh */
		DrawData &operator=(const Mesh &mesh);


		DrawData &remAttr(AssociationPoint type);
		bool hasAttr(AssociationPoint type) const;

		/* modify draw datas */
		DrawCall &addDraw(fg::Primitive primitive = fg::Triangles);
		DrawCall &addDraw(const fg::IndexArrayHolder &inds,fg::Primitive primitive = fg::Triangles);
        DrawCall &addDraw(fm::Size beg,fm::Size len,fg::Primitive primitive = fg::Triangles);

        template<class T,fm::Size N>
        inline DrawCall &addDraw(const T (&data)[N],fg::Primitive primitive);

        template<class T>
        inline DrawCall &addDraw(const T *pointer,fm::Size N,fg::Primitive primitive);

		/* query */
		DrawCall &getDraw(fm::Size index);
		const DrawCall &getDraw(fm::Size index) const;

		fm::Size getDrawCount() const;

        /* reset */
		void reset();
	};
}

#endif // FRONTIER_DRAWDATA_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/DrawData.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
