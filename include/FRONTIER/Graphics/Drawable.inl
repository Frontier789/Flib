////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_DRAWABLE_INL_INCLUDED
#define FRONTIER_DRAWABLE_INL_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>
namespace fg
{
	/////////////////////////////////////////////////////////////
	template<class T>
	inline Attribute::Attribute(const T *ptr,unsigned short stride) : m_ptr(ptr),
																	  m_buffer(0),
																	  m_components(T::components),
																	  m_bytesPerComponent(sizeof(typename T::component_type)),
																	  m_bytesPerVertex(stride)
	{

	}

	/////////////////////////////////////////////////////////////
	template <class pt,class ct,class tpt,class nt>
	inline void draw(const fm::vertex<pt,ct,tpt,nt> *vertices,
					 fm::Size vertexCount,
					 fg::Primitive primitive,
					 const fg::RenderStates &states,
					 const IndexPointer &indices)
	{
		if (nt::components)
			draw(Attribute((pt*)((unsigned char*)vertices),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 Attribute((ct*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 Attribute((tpt*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize+fm::vertex<pt,ct,tpt,nt>::clrSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 Attribute((nt*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize+fm::vertex<pt,ct,tpt,nt>::clrSize+fm::vertex<pt,ct,tpt,nt>::texPosSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 vertexCount,primitive,states,indices);
		else if (tpt::components)
			draw(Attribute((pt*)((unsigned char*)vertices),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 Attribute((ct*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 Attribute((tpt*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize+fm::vertex<pt,ct,tpt,nt>::clrSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 Attribute::Unused,
				 vertexCount,primitive,states,indices);
		else
			draw(Attribute((pt*)((unsigned char*)vertices),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 Attribute((ct*)((unsigned char*)vertices+fm::vertex<pt,ct,tpt,nt>::posSize),sizeof(fm::vertex<pt,ct,tpt,nt>)),
				 Attribute::Unused,
				 Attribute::Unused,
				 vertexCount,primitive,states,indices);
	}

	/////////////////////////////////////////////////////////////
	template <class pt,class ct,class tpt,class nt,fm::Size vertexCount>
	inline void draw(const fm::vertex<pt,ct,tpt,nt> (&vertices)[vertexCount],
						   fg::Primitive primitive,
						   const fg::RenderStates &states = fg::RenderStates())
	{
		draw(vertices,vertexCount,primitive,states);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	Attribute Attr(const T *ptr,unsigned short stride)
	{
		return Attribute(ptr,stride);
	}

	/////////////////////////////////////////////////////////////
	template<class T>
	Attribute Attr(const fg::Buffer &buf,fm::Ptrdiff offset,unsigned char stride)
	{
		Attribute ret;
		ret.m_ptr = (const void*)offset;
		ret.m_buffer = &buf;
		ret.m_components = T::components;
		ret.m_bytesPerComponent = sizeof(typename T::component_type);
		ret.m_bytesPerVertex = stride ? stride : sizeof(T);

		return ret;
	}
}
#endif //FRONTIER_DRAWABLE_INL_INCLUDED
