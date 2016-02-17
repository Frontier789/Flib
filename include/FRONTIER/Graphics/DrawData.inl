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
	template<class T>
	DrawData::Attribute::Attribute(AssociationPoint type,T *pointer,fm::Size N,typename fm::Enable_if<fg::Is_GLDataType<T>::value >::type*) : type(type),
																																			  components(1),
																																			  stride(0),
																																			  count(N),
																																			  componentType(fg::Is_GLDataType<T>::enumVal),
																																			  buf(new fg::Buffer(fg::IndexBuffer))
	{
		buf->setData(pointer,sizeof(*pointer) * N);
	}

	//////////////////////////////////////////////////////////////////////////
	template<class T,fm::Size N>
	DrawData::Attribute::Attribute(AssociationPoint type,const T (&pointer)[N],typename fm::Enable_if<fg::Is_GLDataType<T>::value >::type*) : type(type),
																																			  components(1),
																																			  stride(0),
																																			  count(N),
																																			  componentType(fg::Is_GLDataType<T>::enumVal),
																																			  buf(new fg::Buffer(fg::IndexBuffer))
	{
		buf->setData(pointer,sizeof(*pointer) * N);
	}

	//////////////////////////////////////////////////////////////////////////
	template<class T>
	DrawData::Attribute::Attribute(AssociationPoint type,T *pointer,fm::Size N,typename fm::Enable_if<!fg::Is_GLDataType<T>::value >::type*) : type(type),
																																			   components(T::components),
																																			   stride(0),
																																			   count(N),
																																			   componentType(fg::Is_GLDataType<typename T::component_type>::enumVal),
																																			   buf(new fg::Buffer(fg::IndexBuffer))
	{
		buf->setData(pointer,sizeof(*pointer) * N);
	}

	//////////////////////////////////////////////////////////////////////////
	template<class T,fm::Size N>
	DrawData::Attribute::Attribute(AssociationPoint type,const T (&pointer)[N],typename fm::Enable_if<!fg::Is_GLDataType<T>::value >::type*) : type(type),
																																			   components(T::components),
																																			   stride(0),
																																			   count(N),
																																			   componentType(fg::Is_GLDataType<typename T::component_type>::enumVal),
																																			   buf(new fg::Buffer(fg::IndexBuffer))
	{
		buf->setData(pointer,sizeof(*pointer) * N);
	}
			
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	DrawData::DrawCall::DrawCall(T *pointer,fm::Size N,fg::Primitive primitive,fm::Size drawBeg,fm::Size drawLen) : primitive(primitive),
																													indexCount(N),
																													drawBeg(drawBeg),
																													drawLen(drawLen),
																													componentType(fg::Is_GLDataType<T>::enumVal),
																													buf(new fg::Buffer(fg::IndexBuffer))
	{
		buf->setData(pointer,sizeof(*pointer) * N);
	}

	//////////////////////////////////////////////////////////////////////////
	template<class T,fm::Size N>
	DrawData::DrawCall::DrawCall(const T (&pointer)[N],fg::Primitive primitive,fm::Size drawBeg,fm::Size drawLen) : primitive(primitive),
																													indexCount(N),
																													drawBeg(drawBeg),
																													drawLen(drawLen),
																													componentType(fg::Is_GLDataType<T>::enumVal),
																													buf(new fg::Buffer(fg::IndexBuffer))
	{
		buf->setData(pointer,sizeof(*pointer) * N);
	}
}

#endif // FRONTIER_DRAWDATA_INL_INCLUDED
