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

#define FRONTIER_DRAWDATA
#include <vector>

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
			AssociationPoint type;
			fm::Size components;
			fm::Size stride;
			fm::Size count;
			unsigned long componentType;
			fg::Buffer *buf;

			Attribute(AssociationPoint type = Assoc::Unused,
					  fm::Size components = 0,
					  fm::Size stride = 0,
					  fm::Size count = 0,
					  unsigned long componentType = 0,
					  fg::Buffer *buf = fm::nullPtr);

			template<class T>
			Attribute(AssociationPoint type,T *pointer,fm::Size N,typename fm::Enable_if<fg::Is_GLDataType<T>::value >::type* = fm::nullPtr);

			template<class T,fm::Size N>
			Attribute(AssociationPoint type,const T (&pointer)[N],typename fm::Enable_if<fg::Is_GLDataType<T>::value >::type* = fm::nullPtr);

			template<class T>
			Attribute(AssociationPoint type,T *pointer,fm::Size N,typename fm::Enable_if<!fg::Is_GLDataType<T>::value >::type* = fm::nullPtr);

			template<class T,fm::Size N>
			Attribute(AssociationPoint type,const T (&pointer)[N],typename fm::Enable_if<!fg::Is_GLDataType<T>::value >::type* = fm::nullPtr);

			~Attribute();
		};

		class DrawCall
		{
		public:

			fg::Primitive primitive;
			fm::Size indexCount;
			fm::Size drawBeg;
			fm::Size drawLen;

			unsigned long componentType;
			fg::Buffer *buf;

			template<class T>
			DrawCall(T *pointer,fm::Size N,fg::Primitive primitive,fm::Size drawBeg = 0,fm::Size drawLen = fm::Size(-1));

			template<class T,fm::Size N>
			DrawCall(const T (&pointer)[N],fg::Primitive primitive,fm::Size drawBeg = 0,fm::Size drawLen = fm::Size(-1));

			DrawCall(const IndexArrayHolder &indices,fg::Primitive primitive,fm::Size drawBeg = 0,fm::Size drawLen = fm::Size(-1));

			DrawCall(fg::Primitive primitive,fm::Size drawBeg = 0,fm::Size drawLen = fm::Size(-1));

			~DrawCall();
		};

		std::vector<Attribute*> attrs;
		std::vector<DrawCall*>  drawCalls;

		DrawData();

		explicit DrawData(const Mesh &m);

		virtual ~DrawData();

		Attribute *operator[](AssociationPoint type);
		DrawData &setAttribute(Attribute *ptr);
	};
}

#endif // FRONTIER_DRAWDATA_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Graphics/DrawData.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
